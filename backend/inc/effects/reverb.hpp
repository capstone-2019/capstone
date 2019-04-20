/** @file reverb.hpp
 *
 *	@brief contains the API for the reverb effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#ifndef _REVERB_H_
#define _REVERB_H_

#include <effect.hpp>

#define NUM_COMB 4

class Reverb : public Effect
{

public:

	Reverb();
	~Reverb() { };

	float apply(float val) override;

private:

	float comb_filter(float val);
	float all_pass_filter(float val);

	int delay = 44100/4;
	float decay = 0.2f;

	float **buf;
	int buf_len;
	int index;

};

#endif /* _REVERB_H_ */