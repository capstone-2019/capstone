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
#include <vector>
#include <string>

using std::string;
using std::vector;

using std::vector;
using std::string;

static std::mutex m;
static std::condition_variable cv;

extern volatile bool stop_simulation;

/****************************************************************************
 *                    audio hardware callback function                      *
 ****************************************************************************/

static int callBack(const void *inputBuffer, void *outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlags,
					void *userData) {

	AudioManager::callback_data *data = (AudioManager::callback_data*) userData;

	float total;


	if (data->in) {
		AudioManager::buffer b;
		// memcpy(b.buf, inputBuffer, framesPerBuffer * sizeof(float));
		for (int i = 0; i < framesPerBuffer; i++) {
			total = 0.f;
			for (int j = 0; j < NUM_CHANNELS; j++) {
				total += ((float*)inputBuffer)[NUM_CHANNELS*i];
			}
			b.buf[i] = (total / ((float) NUM_CHANNELS));
		}
		data->hw_input_buf.push(b); //TODO: Emplace? push?
		data->num_frames += framesPerBuffer;
		cv.notify_one();
	}

	if (data->out) {
		if (data->hw_output_buf.size() >= 2) {
			// for (int i = 0; i < framesPerBuffer; i++) {
			// 	((float *)outputBuffer)[i] = data->hw_output_buf.front().buf[i];
			// }
			memcpy(outputBuffer, data->hw_output_buf.front().buf, framesPerBuffer * sizeof(float));
			data->hw_output_buf.pop();
		}
	}

	return !data->done  ? 0 : paComplete;
}

/****************************************************************************
 *                             helper functions                             *
 ****************************************************************************/


static PaStream* portaudio_init_hw(AudioManager::callback_data *data) {

	// parameters for the output
	PaStream *stream;
	PaStreamParameters inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    // printf("latency : %lf\n", Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency);

    PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	int err = Pa_OpenStream(
				&stream,
				data->in ? &inputParameters : NULL,
				data->out ? &outputParameters : NULL,
				data->samplerate,
				HW_FRAMES_PER_BUFFER,
				paClipOff,
				callBack,
				data);

	if (err != paNoError) {
		printf("Error while opening stream (%d)\n", err);
		return NULL;
	}

	return stream;

}


/****************************************************************************
 *                              API functions                               *
 ****************************************************************************/

float AudioManager::apply_effects(float val, vector<string> effects) {

	for (auto it = effects.begin(); it < effects.end(); ++it) {
		if (*it == "REVERB") {
			val = reverb.apply(val);
		} else if (*it == "FUZZ") {
			val = fuzz.apply(val);
		} else if (*it == "DELAY" ) {
			val = delay.apply(val);
		} else if (*it == "DISTORTION" ) {
			val = distortion.apply(val);
		} else {
			sim_error("invalid pre-effect\n");
		}
	}

	return val;
}

AudioManager::AudioManager(input_t input_mode, output_t output_mode,
			 const char *input_filename, const char *output_filename,
			 filetype_t infile_type,
			 vector<string> effect_blocks) {

	this->input_mode = input_mode;
	data = new callback_data;
	data->in = false;
	data->out = false;
	data->done = false;
	this->output_mode = output_mode;
	effects = effect_blocks;

	/* initialize input */
	/* TODO: Support hardware modes things */
	if (input_mode == INPUT_HARDWARE) {
		/* initialize hw_data */
		data->samplerate = HW_SAMPLERATE;
		data->num_frames = 0;
		// TODO: support multi channel?
		data->in = true;
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
		data->out = true;
		data->samplerate = HW_SAMPLERATE;
		output_index = 0;
		// std::cerr << "mode not yet supported\n";
		// assert(false);
	}

	data->input_index = 0;
	data->num_frames_read = 0;

	/* initialize portaudio */
	if (input_mode == INPUT_HARDWARE || output_mode & OUTPUT_HARDWARE) {
		/* initialize portaudio */
		if (Pa_Initialize() != paNoError) {
			printf("Error while initializing portaudio\n");
		}
		stream = portaudio_init_hw(data);
		if (Pa_StartStream(stream) != paNoError) {
			printf("Error while starting stream\n");
		}
	}
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

	bool ret;

	if (input_mode == INPUT_FILE)
		ret = file_get_next_value(val);
	else if (input_mode == INPUT_HARDWARE) {
		ret = hw_get_next_value(val);
	} else {
		assert(false);
	}

	*val = (float) apply_effects((float) *val, effects);

	if (stop_simulation) return false;
	return ret;
}

void AudioManager::file_set_next_value(double val) {
	if (fout != NULL)
		fout->set_next_value(val);
}

void AudioManager::hw_set_next_value(double val) {
	temp_out_buffer.buf[output_index++] = (float) val;

	if (output_index == HW_FRAMES_PER_BUFFER) {
		data->hw_output_buf.emplace(temp_out_buffer);
		output_index = 0;
	}
}

void AudioManager::set_next_value(double val) {
	if (output_mode & OUTPUT_FILE) file_set_next_value(val);
	if (output_mode & OUTPUT_HARDWARE) hw_set_next_value(val);
}

void AudioManager::finish() {
	printf("in finish\n");
	printf("fout is %p\n", fout);
	if (fout != NULL) {
		fout->finish();
	}

	if (input_mode == INPUT_HARDWARE || output_mode == OUTPUT_HARDWARE) {
		data->done = true;
		Pa_StopStream(stream);
		Pa_CloseStream(stream);
		Pa_Terminate();
	}
}

