/**
 *
 * @file voltagein.cpp
 *
 * @date April 2, 2019
 *
 * @brief This file contains the implementation of the voltage input
 * component, which feeds the input signal through the simulated circuit.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#include <components/component.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <errors.hpp>
#include <stdio.h>

using std::vector;
using std::string;
using Eigen::VectorXd;
using std::unordered_map;

/**
 * @brief Constructs a new voltage input.
 *
 * @param tokens The netlist file tokens associated with the voltage
 * input's netlist description.
 */
VoltageIn::VoltageIn(const vector<string>& tokens, AudioManager *am) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	sample_period = am->get_sampling_period();
	this->am = am;
}

/**
 * @brief Converts the voltage input to a string representation.
 */
string VoltageIn::to_string() {
	std::ostringstream vin_string;
	vin_string << "Voltage Input from node(-): "
	           << nneg
	           << " to node(+): "
	           << npos
	           << " based on the signal '"
	           << signal_file
	           << "'";
	return vin_string.str();
}

/**
 * @brief Gets the unknown variables associated with this component.
 */
vector<string> VoltageIn::unknowns() {
	vector<string> unknown_variables = {
		unknown_voltage(npos),
		unknown_current("vin")
	};
	return unknown_variables;
}

/**
 * @brief Pre-computes the mappings from unknown quantities associated with
 * this component to indices that will be used to construct the KCL matrix.
 *
 * @param mappings Hash map which maps string unknown identifiers to
 * integer unknown IDs.
 */
void VoltageIn::map_unknowns(unordered_map<string, int> mappings) {
	this->n1 = mappings[unknown_voltage(npos)];
	this->n2 = mappings[unknown_voltage(nneg)];
	this->ni = mappings[unknown_current("vin")];
}

/**
 * @brief Fetches the next voltage reading in the input signal.
 *
 * @param voltage A pointer to be filled in with the next voltage, if one
 * exists.
 *
 * @return True if there was a new voltage in the input signal and false
 * otherwise. If this function returns false, the value of `*voltage` is
 * undefined.
 */
bool VoltageIn::next_voltage(double *voltage) {
	bool ret = am->get_next_value(voltage);
	this->V = *voltage;
	return ret;
}

/**
 * @brief Gets the sampling period for the input signal.
 */
double VoltageIn::get_sampling_period() {
	return sample_period;
}

/**
 * @brief Adds the KCL contributions of a voltage source to the system
 * of KCL equations.
 *
 * @param sys The system of equations.
 * @param soln The solution to the system from the last timestep.
 * @param prev_soln The solution to the system from the last newton iteration.
 * @param dt The sampling period.
 */
void VoltageIn::add_contribution(LinearSystem& sys, VectorXd& soln,
	VectorXd& prev_soln, double dt) {

	/* add LHS contribution */
	sys.increment_lhs(ni, n1, +1);
	sys.increment_lhs(ni, n2, -1);
	sys.increment_lhs(n1, ni, -1);
	sys.increment_lhs(n2, ni, +1);

	/* add RHS contribution */
	sys.increment_rhs(ni, V - (prev_soln(n1) - prev_soln(n2)));
	sys.increment_rhs(n1, +prev_soln(ni));
	sys.increment_rhs(n2, -prev_soln(ni));
}
