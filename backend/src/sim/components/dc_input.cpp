/**
 *
 * @file dc_input.cpp
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 * @brief This file contains the implementation of DC voltage sources.
 * These types of sources differ a bit from VoltageIn instances, because
 * there can be several of these in the same circuit.
 *
 * @date 04-16-2019
 *
 */

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <components/component.hpp>

using std::string;
using std::vector;
using std::unordered_map;
using Eigen::VectorXd;

/**
 * @brief Construct a new DC voltage source
 *
 * @param tokens The netlist file tokens describing the DC source.
 */
DCInput::DCInput(const vector<string>& tokens) {
    name = tokens[1];
    npos = stoi(tokens[2]);
    nneg = stoi(tokens[3]);
    V = stod(tokens[4]);
}

/**
 * @brief Converts a DCInput object to a string.
 */
string DCInput::to_string() {
    std::ostringstream dc_src_string;
    dc_src_string << "DC Voltage Source of "
                  << V
                  << " Volts from node(-): "
                  << nneg
                  << " to node(+): "
                  << npos;
    return dc_src_string.str();
}

/**
 * @brief Gets the unknowns associated with a DCInput object.
 *
 * There are three assocaited unknowns:
 * - The unknown node voltage at the positive terminal.
 * - The unknown node voltage at the negative terminal.
 * - The unknown source current through the device.
 *
 * @return A vector of strings describing the unknowns.
 */
vector<string> DCInput::unknowns() {
    vector<string> unknown_variables;
    unknown_variables.push_back(unknown_voltage(npos));
    unknown_variables.push_back(unknown_voltage(nneg));
    unknown_variables.push_back(unknown_current(this->name));
    return unknown_variables;
}

/**
 * @brief Converts unknowns from their string representations to their
 * indices into the unknown vector using during transient analysis.
 *
 * @param mappings Dictionary mapping unknown names to their unknown vector
 * indices.
 */
void DCInput::map_unknowns(unordered_map<string, int> mappings) {
    this->n1 = mappings[unknown_voltage(npos)];
    this->n2 = mappings[unknown_voltage(nneg)];
    this->ni = mappings[unknown_current(this->name)];
}

/**
 * @brief Adds the KCL contributions of a DC voltage source to the system
 * of KCL equations.
 *
 * @param sys The system of equations.
 * @param soln The solution to the system from the last timestep.
 * @param prev_soln The solution to the system from the last newton iteration.
 * @param dt The sampling period.
 */
void DCInput::add_contribution(LinearSystem& sys, VectorXd& soln,
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
