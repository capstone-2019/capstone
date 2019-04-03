/**
 *
 * @file circuit.hpp
 *
 * @date April 1, 2019
 *
 * @brief This file contains the interface to the circuit module, which is
 * used to drive the simulation.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 * @bug The way unknowns are tracked is pretty inefficient.
 *
 */

#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include <components/component.hpp>
#include <unordered_map>
#include <vector>

/**
 * @brief Circuit class that is used as the primary driver of
 * the simulation.
 */
class Circuit
{
public:

	/**
	 * @brief Constructs a circuit.
	 */
	Circuit() : next_unknown_id(0) { }

	/**
	 * @brief Destroys a circuit.
	 */
	~Circuit() { }

	/*
	 * Insert various components into the circuit.
	 */
	void register_ground(int ground_id);
	void register_resistor(Resistor *r);
	void register_capacitor(Capacitor *c);
	void register_vin(VoltageIn *vin);
	void register_vout(VoltageOut *vout);

	void transient(std::vector<double>& timescale,
		           std::vector<double>& input_signal,
		           std::vector<double>& output_signal);

private:

	/**@brief Max number of newton iterations for a round of analysis */
	static constexpr const int MAX_ITERATIONS = 100;

	/** @brief maps human readable unknowns to their integer identifiers */
	std::unordered_map<std::string, int> unknowns;
	/** @brief id that will be assigned to the next unknown */
	int next_unknown_id;
	/** @brief Total number of unknowns in the circuit */
	int total_unknowns;

	/** @brief vector of components in the circuit */
	std::vector<Component*> components;

	/** @brief ground node */
	int ground_id;
	/** @brief Circuit's input voltage source */
	VoltageIn *vin;
	/** @brief Circuit's output signal */
	VoltageOut *vout;

	bool process_deltas(const Eigen::VectorXd& deltas,
		                      Eigen::VectorXd& prev_soln,
		                      double tolerance = 1.0e-3);

	/* Register a set of unknowns to be tracked while solving the circuit */
	void register_unknowns(const std::vector<std::string>& unknowns);

	/* Build system of equations from KCL at each node */
	void run_kcl(double dt, Eigen::VectorXd& soln, Eigen::VectorXd& prev_soln,
		LinearSystem& sys);
};

#endif /* _CIRCUIT_H_ */
