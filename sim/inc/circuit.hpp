#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include <components/component.hpp>
#include <unordered_map>
#include <vector>

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

private:

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

	void register_unknowns(const std::vector<std::string>& unknowns);
};

#endif /* _CIRCUIT_H_ */
