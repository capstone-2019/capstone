/** @file distortion.hpp
 *
 *	@brief interface for the distortion effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#ifndef _FUZZ_H_
#define _FUZZ_H_

#include <effect.hpp>

class Distortion : public Effect
{
public:

	Distortion() { };
	~Distortion() { };

	float apply(float val) override;

private:

	float boost;
	float drive;
	float tone;

};

#endif /* _FUZZ_H_ */