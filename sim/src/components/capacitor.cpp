/**
 *
 * @file capacitor.cpp
 *
 * @date April 2, 2019
 *
 * @brief This file contains the implementation for the capacitor component
 * supported by this simulator.
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
 * @brief Constructs a new capacitor.
 *
 * @param tokens The tokens found in the netlist file line describing
 * this circuit element.
 */
Capacitor::Capacitor(const vector<string>& tokens) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	capacitance = parse_by_unit(tokens[4]);
}

/**
 * @brief Converts a capacitor to a string representation
 */
string Capacitor::to_string() {
	std::ostringstream capacitor_string;
	capacitor_string << "Capacitor from node(-): "
	                 << nneg
	                 << " to node(+): "
	                 << npos
	                 << " with capacitance of "
	                 << capacitance
	                 << " Farads";
	return capacitor_string.str();
}

/**
 * @brief Gets the unknown quantities associated with this device.
 */
vector<string> Capacitor::unknowns() {
	vector<string> unknown_variables;
	unknown_variables.push_back(unknown_voltage(npos));
	unknown_variables.push_back(unknown_voltage(nneg));
	return unknown_variables;
}

/**
 * @brief Adds the contributions of this capacitor to the system of KCL
 * equations.
 *
 * @param sys The system of equations.
 * @param soln The solution from the previous timestep.
 * @param prev_soln The solution from the previous newton iteration.
 * @param dt The sampling period.
 */
void Capacitor::add_contribution(LinearSystem& sys, VectorXd& soln,
	VectorXd& prev_soln, double dt) {

	vector<string> unknown_variables = unknowns();
	int n1 = sys.unknowns_map[unknown_variables[0]];
	int n2 = sys.unknowns_map[unknown_variables[1]];

	float C = capacitance;
	sys.increment_lhs(n1, n1, +C/dt);
	sys.increment_lhs(n2, n2, +C/dt);
	sys.increment_lhs(n1, n2, -C/dt);
	sys.increment_lhs(n2, n1, -C/dt);

	double vt0 = prev_soln(n1) - prev_soln(n2);
	double vt1 = soln(n1) - soln(n2);
	double di = (vt1 - vt0) * (C / dt);

	sys.increment_rhs(n1, +di);
	sys.increment_rhs(n2, -di);
}
