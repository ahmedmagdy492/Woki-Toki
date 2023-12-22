#include <SDL2/SDL.h>

#ifdef _WIN32
#include "win_net_client.h"
#elif __linux__
#include "lin_net_client.h"
#endif

#define BUF_LEN 1024000

static Uint8 buffer[BUF_LEN];
static Uint8* ptr_buffer = buffer;
static int added_bytes = 0;

void record_callback(void* user_data, Uint8* stream, int len) {
  added_bytes += len;
  if(added_bytes < BUF_LEN) {
    SDL_memcpy(ptr_buffer, stream, len);

    ptr_buffer += len;
  }
  else {
    printf("Quiting cause buffer is already full\n");
    return;
  }
}

int record_audio(const char* ip) {
  int result;
  
  result = SDL_Init(SDL_INIT_AUDIO);
  if(result < 0) {
    printf("Cannot init SDL\n");
    return -1;
  }

  SDL_AudioDeviceID record_device_id;
  SDL_AudioSpec desired_spec;
  desired_spec.freq = 44100;
  desired_spec.channels = 2;
  desired_spec.callback = record_callback;

  SDL_AudioSpec rec_spec;
  SDL_zero(rec_spec);

  record_device_id = SDL_OpenAudioDevice(NULL, SDL_TRUE, &desired_spec, &rec_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);

  if(record_device_id == 0) {
    printf("Cannot open audio device\n");
    return -2;
  }

  SDL_PauseAudioDevice(record_device_id, 0);

  result = send_data(buffer, added_bytes, ip);
  if(result != 0)
    return -3;

  SDL_CloseAudioDevice(record_device_id);
  SDL_Quit();

  return 0;
}
