#include <circuit.hpp>
#include <parser/netparser.hpp>
#include <iostream>
#include <math.h>

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
	// std::cout << "REGISTERED GROUND! " << node_id << std::endl;
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
	components.push_back(r);
}

/**
 * @brief Adds a new capacitor to the circuit.
 *
 * @param c The capacitor.
 *
 * @bug Not yet implemented.
 */
void Circuit::register_capacitor(Capacitor *c) {
	components.push_back(c);
}

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
	components.push_back(vin);
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


bool Circuit::process_deltas(const VectorXd& deltas, VectorXd& prev_soln,
	double tolerance) {

	double max_delta = fabs(deltas(0));

	for (int r = 0; r < total_unknowns; r++)
		max_delta = fmax(fabs(deltas(r)), max_delta);

	// std::cout << "max_delta = " << max_delta << std::endl;
	// std::cout << deltas << std::endl;

	if (max_delta < tolerance)
		return true;

	for (int r = 0; r < total_unknowns; r++)
		prev_soln(r) += deltas(r);

	return false;
}

void Circuit::run_kcl(double dt, VectorXd& prev_soln, LinearSystem& sys) {
	sys.clear();
	for (auto c : components) {
		c->add_contribution(sys, prev_soln, dt);
	}
}

/**
 * @brief Runs transient analysis on a circuit agains the signal provided
 * by the VoltageIn circuit component.
 *
 * @param timescale Vector to be filled with all the time values produced
 * during analysis.
 *
 * @param input_signal The input signal provided by the VoltageIn component.
 *
 * @param output_signal The output signal measured across the nodes specified
 * by the VoltageOut component.
 *
 * This function destructively modifies all three vectors by populating them
 * with the simulation results.
 */
void Circuit::transient(vector<double>& timescale,
	                    vector<double>& input_signal,
	                    vector<double>& output_signal) {

	// std::cout << "Ground (here) is " << ground_id << std::endl;
	print_unknowns_map(unknowns);
	double dt = vin->get_sampling_period();
	double t = 0;
	double voltage;

	LinearSystem sys(total_unknowns, ground_id, unknowns);
	VectorXd prev_soln(total_unknowns);
	VectorXd soln(total_unknowns);
	soln.setZero();

	while(vin->next_voltage(&voltage)) {
		timescale.push_back(t);
		input_signal.push_back(voltage);
		bool converged = false;

		/* save solution from previous timestep */
		prev_soln = soln;

		/* run at most MAX_ITERATIONS iterations of newton's method */
		for (int iter = 0; iter < MAX_ITERATIONS && !converged; iter++) {
			run_kcl(dt, prev_soln, sys);
			auto& deltas = sys.solve();
			// printf("Iter = %d, VIN = %lf\n", iter, voltage);
			// std::cout << sys.to_string() << std::endl;
			// std::cout << "-----------------------------" << std::endl << std::endl;
			converged = process_deltas(deltas, prev_soln);
		}

		/* record solution for this timestep and advance simulation time */
		soln = prev_soln;
		output_signal.push_back(vout->measure(sys, soln));
		t += dt;
	}
}
