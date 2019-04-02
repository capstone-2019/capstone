/** @file input_interface.hpp
 *
 *	@brief implements the InputInterface class, which serves as the parent
 *	to different implementations for an audio interface.
 *
 *	@author Joseph Kim
 */

#ifndef _INPUT_INTERFACE_H_
#define _INPUT_INTERFACE_H_

#include <iostream>

class InputInterface
{

public:

	InputInterface() { }
	~InputInterface() {}

	virtual bool get_next_value(float *val) = 0;
	virtual int get_samplerate() = 0;

private:

};

#endif /* _INPUT_INTERFACE_H_ */
