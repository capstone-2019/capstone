/** @file hardware_input.hpp
 *
 *	@brief implements the HardwareInput class, which retrieves audio via the
 *	system's hardware.
 *
 *	@author Joseph Kim
 */

#ifndef _HARDWARE_INPUT_H_
#define _HARDWARE_INPUT_H_

#include <iostream>
#include <queue>
#include <vector>
#include <input_interface.hpp>

#define FRAMES_PER_BUFFER 512
#define SAMPLERATE 44100

class HardwareInput : public InputInterface
{

public:

	HardwareInput(int channels);
	// ~HardwareInput() {}

	bool get_next_value(float *val) override;

	int get_samplerate() override;

private:
	struct buffer {
		double buf[FRAMES_PER_BUFFER];
	};
	/** @brief queue of buffers containing audio */
	std::vector<std::queue<buffer>> buf_queues;
	/** @brief samplerate of the file */
	int samplerate;
	/** @brief number of frames the file contains */
	int num_frames;
	/** @brief index get_next_value will serve */
	int cur_index;
	/** @brief the number of channels the input file had */
	int channels;
	/** @brief whether to continue or stop processing input */
	bool done;
};

#endif /* _HARDWARE_INPUT_H_ */
