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

AudioManager::AudioManager(input_t input_mode, output_t output_mode,
			 const char *input_filename, const char *output_filename,
			 filetype_t infile_type) {


	/* TODO: Support hardware modes things */

	/* initialize input */
	if (input_mode == INPUT_FILE) {
		FileInput fi(input_filename, infile_type);
		input_samplerate = fi.get_samplerate();
		input_num_frames = fi.get_num_frames();
		in = fi;
	} else {
		std::cerr << "mode not yet supported\n";
		assert(false);
	}
	fout = NULL;
	/* initialize outputs */
	if (output_mode & OUTPUT_FILE) {
		fout = new FileOutput(output_filename, input_samplerate);
	}
	if (output_mode & OUTPUT_FILE) {
		std::cerr << "mode not yet supported\n";
		assert(false);
	}

	input_index = 0;

}

bool AudioManager::get_next_value(double *val) {

	float x;
	bool ret;

	ret = in.get_next_value(&x);
	*val = (double) x;

	return ret;	
}

void AudioManager::set_next_value(double val) {
	if (fout != NULL)
		fout->set_next_value(val);
}

void AudioManager::finish() {
	if (fout != NULL) {
		fout->finish();
	}
}

