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

/**
 * @brief Constructs a voltage output.
 *
 * @param tokens The tokens found in the netlist file description
 * of this component.
 */
VoltageOut::VoltageOut(const vector<string>& tokens, AudioManager *am) {
	npos = stod(tokens[2]);
	nneg = stod(tokens[3]);
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

/**
 * @brief Measures the voltage across the output terminals.
 *
 * @param sys The (solved) system of linear equations.
 *
 * @return The potential difference across the output terminals of
 * the circuit in volts.
 */
double VoltageOut::measure(LinearSystem& sys, VectorXd& soln) {
	int npid = sys.unknowns_map[unknown_voltage(npos)];
	int nnid = sys.unknowns_map[unknown_voltage(nneg)];
	double npos_voltage = soln(npid);
	double nneg_voltage = soln(nnid);
	double vout = npos_voltage - nneg_voltage;
	am->set_next_value(vout);
	return vout;
}
