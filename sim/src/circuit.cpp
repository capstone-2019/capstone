#include <circuit.hpp>
#include <parser/netparser.hpp>
#include <iostream>

using std::vector;
using std::string;
using Eigen::MatrixXd;
using Eigen::VectorXd;

void Circuit::register_unknowns(const vector<string>& new_unknown_variables) {
	for (const auto& unknown : new_unknown_variables) {
		if (unknowns.find(unknown) == unknowns.end()) {
			unknowns[unknown] = next_unknown_id++;
		}
	}
	total_unknowns = next_unknown_id;
}

/**
 * @brief Assigns a given node identifier to be the ground node for the
 * circuit.
 *
 * @param node_id The identifier of the ground node.
 */
void Circuit::register_ground(int node_id) {
	ground_id = node_id;
}

/**
 * @brief Adds a new resistor to the circuit.
 *
 * @param r The resistor.
 *
 * @bug Not yet implemented.
 */
void Circuit::register_resistor(Resistor *r) {
	register_unknowns(r->unknowns());
}

/**
 * @brief Adds a new capacitor to the circuit.
 *
 * @param c The capacitor.
 *
 * @bug Not yet implemented.
 */
void Circuit::register_capacitor(Capacitor *c) {}

/**
 * @brief Adds a new voltage input to the circuit.
 *
 * @param vin The voltage input.
 *
 * @bug Not yet implemented.
 */
void Circuit::register_vin(VoltageIn *vin) {
	this->vin = vin;
	register_unknowns(vin->unknowns());
}

/**
 * @brief Adds a new voltage output to the circuit.
 *
 * @param vout The voltage output.
 *
 * @bug Not yet implemented.
 */
void Circuit::register_vout(VoltageOut *vout) {
	this->vout = vout;
}
