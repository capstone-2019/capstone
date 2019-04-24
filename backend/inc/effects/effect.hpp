/** @file effect.hpp
 *
 *  @brief contains the interface for a generic effect.
 *  This class should be used as a template for other classes
 *  that implement various effects.
 *
 *	@author Joseph Kim (youngchk)
 */

#ifndef _EFFECT_H_
#define _EFFECT_H_

/* @bug this shouldn't be here, but temporary placement so a lot of
 * other files can use it */
#define HW_SAMPLERATE 44100


class Effect 
{
public:

	Effect() { };
	~Effect() { };

	/* applies the effect */
	virtual float apply(float val) = 0;

};

#endif /* _EFFECT_H_ */