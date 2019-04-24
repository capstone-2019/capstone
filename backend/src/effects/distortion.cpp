/** @file fuzz.cpp
 *
 *	@brief implementation for the distortion effect.
 *	
 *  The mathematical model for the following is based off of the one
 *  found in the following paper:
 *	https://ses.library.usyd.edu.au/bitstream/2123/7624/2/DESC9115_DAS_Assign02_310106370.pdf
 *
 *	@author Joseph Kim (youngchk)
 */

#include <distortion.hpp>
#include <cmath>

// float boost(float val) {

// }

// Distortion::Distortion(int boost, int drive, int tone) {

// }


float Distortion::apply(float val) {
	float k, a;
	float drive = 7000.f;

	a = sin(((drive + 1)/ 101) * (M_PI/2));
	k = 2 * a / (1 - a);

	return (1 + k) * val / (1 + k * abs(val));
}
