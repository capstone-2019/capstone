/** @file amplify.hpp
 *
 *	@brief interface for the amplify effect.
 *
 *	@author Joseph Kim (youngchk)
 */

#ifndef _AMPLIFY_H_
#define _AMPLIFY_H_

#include <effect.hpp>

class Amplify : public Effect
{
public:

	Amplify() { };
	~Amplify() { };

	float apply(float val) override;

private:

};

#endif /* _AMPLIFY_H_ */