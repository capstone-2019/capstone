/**
 * @file audio_manager.hpp
 *
 * @brief contains the API for the audio manager class, that is responsible for
 * managing audio input and output.
 *
 * @author Joseph Kim (younghck)
 */

#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <input_interface.hpp>
#include <file_output.hpp>
#include <queue>

#include "portaudio.h"

#define HW_FRAMES_PER_BUFFER 512
#define HW_SAMPLERATE 44100
#define NUM_CHANNELS 2

class AudioManager
{
public:

	typedef enum {
		INPUT_FILE,
		INPUT_HARDWARE,
	} input_t;

	typedef enum {
		FILETYPE_WAV,
		FILETYPE_TXT,
		FILETYPE_NONE,
	} filetype_t;

	static constexpr const int OUTPUT_FILE = 1;
	static constexpr const int OUTPUT_HARDWARE = (1 << 1);
	typedef int output_t;

	/** @brief Constructor for hardware input and output */
	// AudioManager() {}
	// /** @brief Constructor for one input or output file */
	// AudioManager(input_t input_mode, output_t output_mode,
	// 			 const char *filename) {}

	/** @brief Constructor for both input and output files */
	AudioManager(input_t input_mode, output_t output_mode,
				 const char *input_filename, const char *output_filename,
				 filetype_t infile_type);

	/** @brief gets the next available value and stores it in val. Returns
	    false when no more data is available. */
	bool get_next_value(double *val);

	/** @brief sets the next value. */
	void set_next_value(double val);

	double get_sampling_period() { return 1.0 / data->samplerate; }

	/** @brief flush the values into a file */
	void finish();

	struct buffer {
		float buf[HW_FRAMES_PER_BUFFER];
	};
	typedef struct {
		std::queue<buffer> hw_input_buf;
		std::queue<buffer> hw_output_buf;
		// std::vector<std::queue<buffer>> hw_input_buf;
		// std::vector<std::queue<buffer>> hw_output_buf;
		int input_index;
		int num_frames;
		int samplerate;
		volatile int num_frames_read; // only used by hw
		bool in;
		bool out;
		bool done;
	} callback_data;

private:

	InputInterface *in;
	FileOutput *fout;

	/** @brief used for file inputs */
	// int input_samplerate;

	callback_data *data;
	input_t input_mode;
	PaStream *stream;

	bool hw_get_next_value(double *val);
	bool file_get_next_value(double *val);
	void hw_set_next_value(double val);	
	void file_set_next_value(double val);

	buffer temp_out_buffer;
	int output_index;
	output_t output_mode;


};

#endif /* _AUDIO_MANAGER_H_ */