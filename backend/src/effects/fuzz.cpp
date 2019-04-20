/** @file fuzz.cpp
 *
 *	@brief implementation for the fuzz effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#include <fuzz.hpp>

static float cubic_amplifier(float x) {
	if (x < 0.f) {
		x += 1.f;
		x = x * x * x - 1.f;
	} else {
		x -= 1.f;
		x = x * x * x + 1.f;
	}
	return x;
}

float Fuzz::apply(float val) {
	return cubic_amplifier(cubic_amplifier(cubic_amplifier(cubic_amplifier(val))));
}
