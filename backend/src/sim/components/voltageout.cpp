/**
 *
 * @file voltageout.cpp
 *
 * @date April 1, 2019
 *
 * @brief This file contains the implementation of the VoltageOut component,
 * used to indicate where measurements should be taken by the circuit
 * simulator.
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
using std::unordered_map;

/**
 * @brief Constructs a voltage output.
 *
 * @param tokens The tokens found in the netlist file description
 * of this component.
 */
VoltageOut::VoltageOut(const vector<string>& tokens, AudioManager *am) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	this->am = am;
}

/**
 * @brief Converts a voltage output to a string.
 *
 * @return String representation of the voltage output.
 */
string VoltageOut::to_string() {
	std::ostringstream vout_string;
	vout_string << "Voltage Output measured across node(+): "
	            << npos
	            << " to node(-): "
	            << nneg;
	return vout_string.str();
}

vector<string> VoltageOut::unknowns() {
	vector<string> unknown_variables = {
		unknown_voltage(npos),
		unknown_voltage(nneg)
	};
	return unknown_variables;
}

void VoltageOut::map_unknowns(unordered_map<string, int> mappings) {
	this->npid = mappings[unknown_voltage(npos)];
	this->nnid = mappings[unknown_voltage(nneg)];
}

/**
 * @brief Measures the voltage across the output terminals.
 *
 * @param sys The (solved) system of linear equations.
 *
 * @return The potential difference across the output terminals of
 * the circuit in volts.
 */
double VoltageOut::measure(LinearSystem& sys, VectorXd& soln) {
	double npos_voltage = soln(npid);
	double nneg_voltage = soln(nnid);
	double vout = npos_voltage - nneg_voltage;
	am->set_next_value(vout);
	return vout;
}
