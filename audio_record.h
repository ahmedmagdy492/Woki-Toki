#pragma once
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define MAX_RECORDING_SECONDS 60

static SDL_AudioDeviceID recording_device_id = 0;
static Uint8* record_buffer = NULL;
static Uint32 buffer_byte_pos = 0;
static Uint32 buffer_byte_size = 0;

static void audio_record_callback(void* userdata, Uint8* stream, int len)
{
	//Copy audio from stream
	memcpy(&record_buffer[buffer_byte_pos], stream, len);

	//Move along buffer
	buffer_byte_pos += len;
}

int record_audio() {
  if(SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("Error: Unable to init SDL2\n");
    return -1;
  }

  SDL_AudioSpec desired_spec, rec_spec;
  SDL_zero(desired_spec);
  SDL_zero(rec_spec);
  desired_spec.freq = 22050;
  desired_spec.format = AUDIO_S16;
  desired_spec.channels = 2;
  desired_spec.samples = 4096;
  desired_spec.callback = audio_record_callback;

  recording_device_id = SDL_OpenAudioDevice(NULL, SDL_TRUE, &desired_spec, &rec_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
  if(recording_device_id == 0) {
    printf("Error: Unable to find recording device\n");
    return -2;
  }

  int byte_per_sample = rec_spec.channels * (SDL_AUDIO_BITSIZE(rec_spec.format / 8));
  int bytes_per_second = rec_spec.freq * byte_per_sample;
  buffer_byte_size = bytes_per_second * MAX_RECORDING_SECONDS;

  record_buffer = (Uint8*)malloc(sizeof(Uint8)*buffer_byte_size);

  return 0;
}

void* get_record_buf() {
  return record_buffer;
}

void stop_record_audio() {
  SDL_CloseAudioDevice(recording_device_id);
  SDL_Quit();
}

void clear_buffer() {
  if(record_buffer != NULL)
    free(record_buffer);
}
