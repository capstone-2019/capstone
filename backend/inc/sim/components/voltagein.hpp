/**
 *
 * @file voltage_in.hpp
 *
 * @brief This file contains the interface for the VoltageIn component
 * type supported by the simulator.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 * @bug Right now, the simulator assumes there is only one voltage input.
 * I think this is an easy fix, but it may involve restructuring some code.
 *
 */

#ifndef _VOLTAGE_IN_H_
#define _VOLTAGE_IN_H_

#include <vector>
#include <string>
#include <stdbool.h>
#include <linsys.hpp>
#include <unordered_map>
#include <audio_manager.hpp>

/**
 * @brief Class to contain the functionality for the VoltageIn component
 * type supported by the simulator.
 */
class VoltageIn : public Component
{
public:

	/** @brief Idenfitifer that denotes a new voltage input in a netfile */
	static constexpr const char *IDENTIFIER = "VOLTAGE_IN";

	/* Construct a new VoltageIn */
	VoltageIn(const std::vector<std::string>& tokens,
		      AudioManager *am);

	/**
	 * @brief Destroys a voltage input.
	 */
	~VoltageIn() { }

	/* Passes back the next voltage in the input signal */
	bool next_voltage(double *V);

	/* Convert a voltage input to a string */
	std::string to_string() override;
	/* Get the unknonws associated with the voltage input */
	std::vector<std::string> unknowns() override;

	/* map unknowns into matrix indices in a linear system */
	void map_unknowns(std::unordered_map<std::string, int> mapping) override;

	/* Adds resistor current contributions into system of KCL equations */
	void add_contribution(LinearSystem& sys,
		                  Eigen::VectorXd& soln,
		                  Eigen::VectorXd& prev_soln,
		                  double dt) override;

	/* Gets the sampling period of input signal */
	double get_sampling_period();

private:
	int npos;                  /**< positive terminal */
	int nneg;                  /**< negative terminal */
	std::string signal_file;   /**< which file the signal is read from */

	std::vector<double> voltages;       /**< the voltage in signal */
	std::vector<double>::iterator vit;  /**< iterator over voltages */
	double V;                           /**< current voltage */
	double sample_period;               /**< signal's samping period */

	int n1;  /**< Matrix index for unknown voltage at (+) terminal */
	int n2;  /**< Matrix index for unknown voltage at (-) terminal */
	int ni;  /**< Matrix index for unknown branch current through source */

	AudioManager *am; /**< AudioManager provides the signals we use */
};

#endif /* _VOLTAGE_IN_H_ */
