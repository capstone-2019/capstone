/** @file delay.hpp
 *
 *	@brief API for the delay effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#ifndef _DELAY_H_
#define _DELAY_H_

#include <effect.hpp>

#define DEFAULT_DELAY_LEN (44100/4)
#define DEFAULT_DELAY_ITERATIONS 6

class Delay: public Effect {

public:
	Delay(int delay_len = DEFAULT_DELAY_LEN, 
		  int delay_iterations = DEFAULT_DELAY_ITERATIONS, 
		  float wet = 0.7, 
		  float decay = 2.f);

	~Delay() { };

	float apply(float val) override;

private:

	int delay_len; /* in frames */
	int delay_iterations;

	int buf_len;
	float *buf;
	int index;
	float wet;
	float decay;
};

#endif /* _DELAY_H_ */