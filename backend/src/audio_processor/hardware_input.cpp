// /**
//  * @file hardware_input.cpp
//  *
//  * @brief conatins the implementation for HardwareInput, which retrieves
//  * information from the hardware audio input and provides it to the user.
//  *
//  * @author Joseph Kim
//  *
//  */

// #include <hardware_input.hpp>
// #include <iostream>
// #include <string>

// #include "portaudio.h"
// #include "pa_mac_core.h"

// static int callBack(const void *inputBuffer, void *outputBuffer,
// 					unsigned long framesPerBuffer,
// 					const PaStreamCallbackTimeInfo* timeInfo,
// 					PaStreamCallbackFlags statusFlags,
// 					void *userData) {

// 	HardwareInput x = ()

// 	if (channels == 1) {
// 		buf_queues[0].emplace(inputBuffer);
// 	} else {
// 		return -1;
// 	}

// 	return 0;
// }


// HardwareInput::HardwareInput(int channels) {
// 	this->channels = channels;
// 	num_frames = 0;
// 	cur_index = 0;
// 	samplerate = SAMPLERATE;

// 	/* setup callback */
// }

// /* only thread safe if 1 thread calls this function */
// bool HardwareInput::get_next_value(float *val) {

// 	while (num_frames == 0);

// 	if (channels == 1) {
// 		*val = (float) buf_queues[0].front().buf[cur_index++];
// 	} else {
// 		double inv_chan = 1.f / channels;
// 		for (int i = 0; i < channels; i++) {
// 			*val = (float) (buf_queues[i].front().buf[cur_index] * inv_chan);
// 		}
// 		cur_index++;
// 	}

// 	cur_index = cur_index % FRAMES_PER_BUFFER;

// 	return true;
// }

// int HardwareInput::get_samplerate() {
// 	return samplerate;
// }