/** @file reverb.cpp
 *
 *	@brief contains the implementation for the reverb effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#include <reverb.hpp>
#include <stdio.h>

Reverb::Reverb() {
		
	buf_len = 2 * delay;
	buf = new float*[NUM_COMB];
	for (int i = 0; i < NUM_COMB; ++i) {
		buf[i] = new float[buf_len]();
	}

	delay = HW_SAMPLERATE/4;
	decay = 0.7f;

	index = 0;

}

float Reverb::apply(float val) {

	return comb_filter(val);
}


float comb_filter_sub(float val, int delay, float decay, float* buf, int index, int buf_len) {
	buf[(index + delay) % buf_len] = (val + buf[(index + delay) % buf_len]) * decay;
	return (val + buf[index]) / 2;
}


float Reverb::comb_filter(float val) {
	val = (comb_filter_sub(val, delay, decay, buf[0], index, buf_len) + 
		   comb_filter_sub(val, delay - 517, decay - 0.1313f, buf[1], index, buf_len) + 
		   comb_filter_sub(val, delay + 851, decay - 0.2743f, buf[2], index, buf_len) + 
		   comb_filter_sub(val, delay - 351, decay - 0.31f, buf[3], index, buf_len));

	if (++index == buf_len) {
		index = 0;
	}

	return val;
}

float Reverb::all_pass_filter(float val) {

	return -1.f;
}