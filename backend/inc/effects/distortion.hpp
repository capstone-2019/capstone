/** @file distortion.hpp
 *
 *	@brief interface for the distortion effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#ifndef _DISTORTION_H_
#define _DISTORTION_H_

#include <effect.hpp>

class Distortion : public Effect
{
public:

	Distortion() { };
	~Distortion() { };

	float apply(float val) override;

private:

	float gain;
	float drive;
	float tone;

};

#endif /* _DISTORTION_H_ */