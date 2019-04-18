/** @file fuzz.hpp
 *
 *	@brief interface for the fuzz effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#ifndef _FUZZ_H_
#define _FUZZ_H_

#include <effect.hpp>

class Fuzz : public Effect
{
public:

	Fuzz() { };
	~Fuzz() { };

	float apply(float val) override;

};

#endif /* _FUZZ_H_ */