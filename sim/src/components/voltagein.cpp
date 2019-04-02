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

/**
 * @brief Constructs a new voltage input.
 *
 * @param tokens The netlist file tokens associated with the voltage
 * input's netlist description.
 */
VoltageIn::VoltageIn(const vector<string>& tokens) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	signal_file = tokens[4];
	std::ifstream sigstream(signal_file);
	int iter = 0;
	string line;

	/* open signal file for reading */
	if (!sigstream.good())
		sim_error("Could not open signal file %s", signal_file.c_str());

	/* parse line by line */
	while (getline(sigstream, line)) {

		std::istringstream iss(line);
		double voltage;

		/* first line contains sampling period, rest contain voltages */
		if (iter == 0 && !(iss >> sample_period))
			sim_error("Could not read sampling period from signal file");

		else if (iter > 0 && !(iss >> voltage))
			sim_error("Bad voltage on line %d - %s", iter+1, line.c_str());

		/* record the voltage */
		if (iter > 0)
			voltages.push_back(voltage);

		iter++;
	}

	vit = voltages.begin();
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
	if (vit == voltages.end())
		return false;

	*voltage = *vit;
	this->V = *vit;
	vit++;
	return true;
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

	/* find the indices into the matrix for our unknown quantities */
	vector<string> unknown_variables = unknowns();
	int ni = sys.unknowns_map[unknown_variables[1]];
	int n1 = sys.unknowns_map[unknown_voltage(npos)];
	int n2 = sys.unknowns_map[unknown_voltage(nneg)];

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
