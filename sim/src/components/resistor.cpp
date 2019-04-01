/**
 *
 * @file resistor.cpp
 *
 * @date April 1, 2019
 *
 * @brief This file provides the implementation of the resistor component
 * for the circuit simulator.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#include <components/component.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;
using Eigen::VectorXd;

/**
 * @brief Constructs a resistor from a vector of tokens found in a netlist.
 *
 * @param tokens Space delimited tokens found in the netlist description
 * of the resistor.
 */
Resistor::Resistor(const vector<string>& tokens) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	resistance = parse_by_unit(tokens[4]);
}

/**
 * @brief Converts this component to a human readable string format.
 *
 * @return String representation of the resistor.
 */
string Resistor::to_string() {
	std::ostringstream resistor_string;
	resistor_string << "Resistor from node(-): "
	                << nneg
	                << " to node(+) "
	                << npos
	                << " with resistance of "
	                << resistance
	                << " Ohms";
	return resistor_string.str();
}

/**
 * @brief Get a vector of unknowns that this component contributes to.
 *
 * @return Vector of unknowns this component contributes to.
 */
vector<string> Resistor::unknowns() {
	vector<string> unknown_variables;
	unknown_variables.push_back(unknown_voltage(npos));
	unknown_variables.push_back(unknown_voltage(nneg));
	return unknown_variables;
}

/**
 * @brief Adds the contributions of this resistor to the KCL equations
 * to the approprate places in the system matrix.
 *
 * @param sys The linear system to update.
 * @param prev_soln The previous solution to the system.
 * @param dt The time step for the system.
 *
 * @bug Right now this is a stand-alone function, but ultimately it is
 * designed to override a virtual `add_contribution` method in the component
 * class.
 */
void Resistor::add_contribution(LinearSystem& sys, VectorXd prev_soln,
	double dt) {

	/* find the indices of the unknowns this resistor impacts */
	double conductance = 1.0 / resistance;
	vector<string> unknown_variables = unknowns();
	int n1 = sys.unknowns_map[unknown_variables[0]];
	int n2 = sys.unknowns_map[unknown_variables[1]];

	/* adjust LHS of the system */
	sys.increment_lhs(n1, n1, +conductance);
	sys.increment_lhs(n2, n2, +conductance);
	sys.increment_lhs(n1, n2, -conductance);
	sys.increment_lhs(n2, n2, -conductance);

	/* adjust RHS of the system */
	double rhs_delta = conductance * (prev_soln(n2) - prev_soln(n1));
	sys.increment_rhs(n1, +rhs_delta);
	sys.increment_rhs(n2, -rhs_delta);
}
