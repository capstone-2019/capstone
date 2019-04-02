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
	} filetype_t;

	static constexpr const int OUTPUT_FILE = 1;
	static constexpr const int OUTPUT_HARDWARE = (1 << 1);
	typedef int output_t;

	/** @brief Constructor for hardware input and output */
	AudioManager() {}
	/** @brief Constructor for one input or output file */
	AudioManager(input_t input_mode, output_t output_mode,
				 const char *filename) {}

	/** @brief Constructor for both input and output files */
	AudioManager(input_t input_mode, output_t output_mode,
				 const char *input_filename, const char *output_filename,
				 filetype_t infile_type) {}

	/** @brief gets the next available value and stores it in val. Returns
	    false when no more data is available. */
	bool get_next_value(double *val) { return true; }

	/** @brief sets the next value. */
	void set_next_value(double val) {}

	/** @brief flush the values into a file */
	void finish() {}

private:

};

#endif /* _AUDIO_MANAGER_H_ */