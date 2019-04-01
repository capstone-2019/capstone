/**
 *
 * @file resistor.hpp
 *
 * @date April 1, 2019
 *
 * @brief This file provides the interface to the resistor component for
 * the circuit simulator.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _RESISTOR_H_
#define _RESISTOR_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <linsys.hpp>

/**
 * @brief Class to capture the basic functionality of the resistor
 * component model supported by the circuit simulator.
 */
class Resistor : public Component
{
public:

	/** @brief String appearing in netlist to identify a new resistor */
	static constexpr const char *IDENTIFIER = "RESISTOR";

	/* Construct resistor from line in a netlist */
	Resistor(const std::vector<std::string>& tokens);

	/**
	 * @brief Destroys a resistor
	 */
	~Resistor() { }

	/* Converts a resistor to a string representation */
	std::string to_string() override;
	/* Gets a vector of unknowns that resistor contributes to */
	std::vector<std::string> unknowns() override;
	/* Adds resistor current contributions into system of KCL equations */
	void add_contribution(LinearSystem& sys, Eigen::VectorXd prev_soln,
		double dt);

private:
	int npos;             /**< Positive terminal of the resistor */
	int nneg;             /**< Negative terminal of the resistor */
	double resistance;    /**< Resistance in ohms */
};

#endif /* _RESISTOR_H_ */
