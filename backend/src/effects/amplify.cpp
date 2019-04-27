/** @file fuzz.cpp
 *
 *	@brief implementation for the amplify effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#include <amplify.hpp>

// float boost(float val) {

// }

// Distortion::Distortion(int boost, int drive, int tone) {

// }


float Amplify::apply(float val) {
	return val * 3;
}
