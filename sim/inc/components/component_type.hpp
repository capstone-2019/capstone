/**
 *
 * @file component_type.hpp
 *
 * @date March 31, 2019
 *
 * @brief Defines the base component type from which all other component types
 * are derived from.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _COMPONENT_TYPE_H_
#define _COMPONENT_TYPE_H_

#include <string>
#include <iostream>

class Component
{
public:
	/**
	 * @bug These should either be private/protected/pure virtual later on
	 * since this is really meant to be an abstract class. Same applies to
	 * other default constructors.
	 */
	~Component() { }
	Component() { }

	/**
	 * Convers a component to a string.
	 *
	 * @bug This should be overriden in all derived classes and ultimately
	 * made pure virtual.
	 */
	virtual std::string to_string() { return "Unspecified Component"; }

	/**
	 * @brief Writes the string representation of a component to a stream.
	 *
	 * @param out The stream the component should be written to.
	 * @param c The component.
	 *
	 * @return The updated stream object.
	 */
	friend std::ostream& operator<<(std::ostream& out, Component& c) {
		out << c.to_string();
		return out;
	}

protected:
	/* Given a string like "15k" converts it into an appropraite double
	 * (i.e. 15k -> 15000.0) */
	double parse_by_unit(const std::string& value);

private:
	/* maps supported unit types to scale factors */
	double get_unit_scale(const std::string& unit);
};

#endif /* _COMPONENT_TYPE_H_ */
