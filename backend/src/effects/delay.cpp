/** @file delay.cpp
 *
 *	@brief implementation for the delay effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#include <delay.hpp>
#include <stdio.h>
Delay::Delay(int delay_len, int delay_iterations, float wet, float decay) {
	this->delay_len = delay_len;
	this->delay_iterations = delay_iterations;
	this->wet = wet;	
	this->decay = decay;
	buf_len = delay_len * delay_iterations * 2;
	buf = new float[buf_len]();
	index = 0;
}



float Delay::apply(float val) {

	float accum = buf[index];
	buf[index] = 0.f;

	float val_contrib = (val / delay_iterations);

	/* add current val's delay */
	for (int i = 1; i < delay_iterations + 1; i++) {
		buf[(index + delay_len*i) % buf_len] += val_contrib;
		val_contrib /= decay;
	}

	index = (index + 1) % buf_len;

	return val_contrib*wet + accum*(1.f - wet);
}