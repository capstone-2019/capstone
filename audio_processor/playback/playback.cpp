/**
 * @file playback.cpp
 * 
 * @brief plays back output generated by the circuit simulator.
 * 
 */

#include <stdio.h>
#include <string.h>
#include "portaudio.h"

#define FRAMES_PER_BUFFER (512)


typedef struct {
	int samplerate;
	int num_frames;
	int frames_read;
	float* frames;
} playData_t;


static int playCallBack(const void *inputBuffer, void *outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlags,
					void *userData) {

	playData_t* data = (playData_t*) userData;


	int frames_to_read = data->frames_read + framesPerBuffer < data->num_frames ? 
						 framesPerBuffer : 
						 data->num_frames - data->frames_read;

	memcpy(outputBuffer, &data->frames[data->frames_read], sizeof(float) * frames_to_read);
	// for (int i =0; i < frames_to_read; i++) {
	// 	((float*)outputBuffer)[i] = data->frames[i + data->frames_read];
	// }
	
	data->frames_read += frames_to_read;

	if (data->frames_read >= data->num_frames) {
		return paComplete;
	}

	return 0;
}

PaStream* portaudio_init_output(playData_t *data) {

	// parameters for the output
	PaStream *stream;
	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;

	int err = Pa_OpenStream(
				&stream,
				NULL, /* no input */
				&outputParameters,
				data->samplerate,
				FRAMES_PER_BUFFER,
				paClipOff,
				playCallBack,
				data);

	if (err != paNoError) {
		printf("Error while opening stream [%s]\n", Pa_GetErrorText(err));
		return NULL;
	}

	return stream;
}

float* parse_input(const char* filename, int* samplerate, int* num_frames) {

	FILE* f = fopen(filename, "r");

	if (f == NULL) {
		return NULL;
	}

	fscanf(f, "%d\n", samplerate);
	fscanf(f, "%d\n", num_frames);

	float* frames = new float[*num_frames];
	int i = 0;
	while (fscanf(f, "%f,", &frames[i++]) > 0);

	printf("read input with %d frames at a samplerate of %d\n", *num_frames, *samplerate);

	fclose(f);

	return frames;

}


int main(int argc, char* argv[]) {

	/* check arguments */
	if (argc == 1) { 
		printf("please provide a filename\n");
	} else if (argc > 2) {
		printf("too many arguments\n");
	}

	/* parse arguments */
	playData_t data;
	data.frames = parse_input(argv[1], &data.samplerate, &data.num_frames);
	if (data.frames == NULL) {
		printf("file not found\n");
		return -1;
	}
	data.frames_read = 0;

	/* initalize portaudio */
	if (Pa_Initialize() != paNoError) {
		printf("Error while initializing portaudio\n");
		return -1;
	}

	/* setup output and callback function */
	PaStream* stream = portaudio_init_output(&data);
	if (Pa_StartStream(stream) != paNoError) {
		printf("Error while starting stream\n");
		return -1;
	}

	/* wait until output is finished */
	int err;
	while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        Pa_Sleep(1000);
        printf("index = %d\n", data.frames_read); fflush(stdout);
    }

    Pa_Terminate();
    
	return 0;
}

