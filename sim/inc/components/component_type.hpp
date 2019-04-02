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
#include <vector>
#include <linsys.hpp>

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
	 * @brief Gets the list of unknowns for a component.
	 *
	 * @bug Should be overriden in all derived classes and ultimately made
	 * pure virtual.
	 */
	virtual std::vector<std::string> unknowns() { return {}; }

	/**
	 * @brief Adds the contributions of a component into the system of KCL
	 * equations.
	 *
	 * @param sys The system of equations.
	 * @param prev_soln The solution from the previous newton iteration.
	 * @param dt The signal sampling period.
	 *
	 * @bug This should eventually be made pure virtual.
	 */
	virtual void add_contribution(LinearSystem& sys,
		                          Eigen::VectorXd prev_soln,
		                          double dt)
	{ }

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

	/* Creates a label for an unknown node voltage */
	static std::string unknown_voltage(int node_id);
	/* Creates a label for an unknown branch current */
	static std::string unknown_current(const std::string& name);

protected:
	/* Given a string like "15k" converts it into an appropraite double
	 * (i.e. 15k -> 15000.0) */
	double parse_by_unit(const std::string& value);

private:
	/* maps supported unit types to scale factors */
	double get_unit_scale(const std::string& unit);
};

#endif /* _COMPONENT_TYPE_H_ */
