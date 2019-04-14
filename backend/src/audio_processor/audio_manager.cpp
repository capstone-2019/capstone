/**
 * @file audio_manager.cpp
 *
 * @brief contains the implementation for the audio manager class, that is
 * responsible for managing audio input and output.
 *
 * @author Joseph Kim (younghck)
 */

#include <audio_manager.hpp>
#include <fileInput.hpp>
#include <iostream>
#include <assert.h>
#include <errors.hpp>

static std::mutex m;
static std::condition_variable cv;


static int callBack(const void *inputBuffer, void *outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlags,
					void *userData) {

	AudioManager::callback_data *data = (AudioManager::callback_data*) userData;

	// printf("callback. data->num_frames %d\n", data->num_frames);

	if (data->in) {
		AudioManager::buffer b;
		memcpy(b.buf, inputBuffer, framesPerBuffer * sizeof(float));
		data->hw_input_buf.push(b); //TODO: Emplace? push?
		data->num_frames += framesPerBuffer;
		cv.notify_one();
	}

	return !data->done ? 0 : paComplete;
}

static PaStream* portaudio_init_input(AudioManager::callback_data *data) {

	// parameters for the output
	PaStream *stream;
	PaStreamParameters inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowOutputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

	int err = Pa_OpenStream(
				&stream,
				&inputParameters,
				NULL, /* no output */
				data->samplerate,
				HW_FRAMES_PER_BUFFER,
				paClipOff,
				callBack,
				data);

	if (err != paNoError) {
		printf("Error while opening stream\n");
		return NULL;
	}

	return stream;

}


AudioManager::AudioManager(input_t input_mode, output_t output_mode,
			 const char *input_filename, const char *output_filename,
			 filetype_t infile_type) {

	this->input_mode = input_mode;
	data = new callback_data;
	data->in = false;
	data->out = false;
	data->done = false;

	/* initialize input */
	/* TODO: Support hardware modes things */
	if (input_mode == INPUT_HARDWARE) {
		/* initialize portaudio */
		if (Pa_Initialize() != paNoError) {
			printf("Error while initializing portaudio\n");
		}
		/* initialize hw_data */
		data->samplerate = HW_SAMPLERATE;
		data->num_frames = 0;
		// TODO: support multi channel?
		data->in = true;
		stream = portaudio_init_input(data);
		if (Pa_StartStream(stream) != paNoError) {
			printf("Error while starting stream\n");
		}
	}
	else if (input_mode == INPUT_FILE) {
		FileInput *fi = new FileInput(input_filename, infile_type);
		data->samplerate = fi->get_samplerate();
		data->num_frames = fi->get_num_frames();
		in = fi;
	} else {
		std::cerr << "mode not yet supported\n";
		assert(false);
	}



	fout = NULL;
	/* initialize outputs */
	if (output_mode & OUTPUT_FILE) {
		fout = new FileOutput(output_filename, data->samplerate);
	}
	if (output_mode & OUTPUT_HARDWARE) {
		std::cerr << "mode not yet supported\n";
		assert(false);
	}

	data->input_index = 0;
	data->num_frames_read = 0;
}

bool AudioManager::file_get_next_value(double *val) {
	float x;
	bool ret;

	ret = in->get_next_value(&x);
	*val = (double) x;

	return ret;
}

bool AudioManager::hw_get_next_value(double *val) {

	/* wait until there is data to read */
	static std::unique_lock<std::mutex> lk(m);

	cv.wait(lk, [this]{return !(data->num_frames_read >= data->num_frames);});
	// std::cout << "Hey\n";
	// while (data->num_frames_read >= data->num_frames) {
	// 	printf(".\n");
	// }

	*val = (double) data->hw_input_buf.front().buf[data->input_index++];

	data->input_index = data->input_index % HW_FRAMES_PER_BUFFER;

	if (data->input_index == 0) {
		data->hw_input_buf.pop();
	}

	data->num_frames_read++;


	return true;
}

bool AudioManager::get_next_value(double *val) {

	if (input_mode == INPUT_FILE)
		return file_get_next_value(val);
	else if (input_mode == INPUT_HARDWARE) {
		return hw_get_next_value(val);
	} else {
		assert(false);
	}
	return false;
}

void AudioManager::set_next_value(double val) {
	if (fout != NULL)
		fout->set_next_value(val);
}

void AudioManager::finish() {
	if (fout != NULL) {
		fout->finish();
	}

	if (input_mode == INPUT_HARDWARE) {
		data->done = true;
		Pa_StopStream(stream);
		Pa_CloseStream(stream);
		Pa_Terminate();
	}
}

