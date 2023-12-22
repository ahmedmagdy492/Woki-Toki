#include <sndfile.h>

int save_file(void* buffer, int audio_size) {
	const char *filename = "output.wav";
	SNDFILE *sndFile;
	SF_INFO sfInfo;
	SDL_AudioSpec spec;
	spec.freq = 44100;
	spec.channels = 2;

	sfInfo.samplerate = spec.freq;
	sfInfo.channels = spec.channels;
	sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	sndFile = sf_open(filename, SFM_WRITE, &sfInfo);
	if (!sndFile) {
    		printf("Error opening output file: %s\n", sf_strerror(NULL));
    		return -1;
	}

	sf_write_short(sndFile, buffer, audio_size); // Write audio data
	sf_close(sndFile);
	return 0;
}
