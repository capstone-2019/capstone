/**
 *
 * @file capacitor.hpp
 *
 * @data April 1, 2019
 *
 * @brief This file contains the interface to the capacitor, a type of
 * energy storage component supported by our simulator.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _CAPACITOR_H_
#define _CAPACITOR_H_

#include <vector>
#include <string>
#include <unordered_map>

/**
 * @brief Class to represent capacitors - a passive energy storage device
 * supported by our simulator.
 */
class Capacitor : public Component
{
public:

	/** @brief String which denotes a new capacitor in a netlist file */
	static constexpr const char *IDENTIFIER = "CAPACITOR";

	/* create a capacitor */
	Capacitor(const std::vector<std::string>& tokens);

	/**
	 * @brief Destroys a capacitor.
	 */
	~Capacitor() { }

	/* Convert a capacitor to its string representation */
	std::string to_string() override;

	/* get the unknown quantities for this component */
	std::vector<std::string> unknowns() override;

	/* map unknowns into matrix indices in a linear system */
	void map_unknowns(std::unordered_map<std::string, int> mapping) override;

	/* Adds resistor current contributions into system of KCL equations */
	void add_contribution(LinearSystem& sys,
			              Eigen::VectorXd& soln,
		                  Eigen::VectorXd& prev_soln,
		                  double dt) override;

private:
	int npos;            /**< Positive terminal */
	int nneg;            /**< Negative terminal */
	double capacitance;  /**< Capacitance in farads */

	int n1; /**< Matrix index for unknown npos voltage */
	int n2; /**< Matrix index for unknown nneg voltage */
};

#endif /* _CAPACITOR_H_ */
