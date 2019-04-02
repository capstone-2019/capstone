/** @file input_interface.hpp
 *
 *	@brief implements the InputInterface class, which serves as the parent
 *	to different implementations for an audio interface.
 *
 *	@author Joseph Kim
 */

#ifndef _INPUT_INTERFACE_H_
#define _INPUT_INTERFACE_H_

class InputInterface
{

public:

	InputInterface() { }
	~InputInterface() {}

	virtual bool get_next_value(float *val) { return false; }

	virtual int get_samplerate() { return -1; }

private:

};

#endif /* _INPUT_INTERFACE_H_ */
