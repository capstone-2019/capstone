/**
 *
 * @file voltage_out.hpp
 *
 * @brief This file contains the interface for the VoltageOut component
 * type supported by the simulator.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _VOLTAGE_OUT_H_
#define _VOLTAGE_OUT_H_

#include <vector>
#include <string>
#include <linsys.hpp>
#include <audio_manager.hpp>
#include <unordered_map>

/**
 * @brief Class to contain the functionality for the VoltageOut component
 * type supported by the simulator.
 */
class VoltageOut : public Component
{
public:

	/** @brief Idenfitifer that denotes a new voltage output in a netfile */
	static constexpr const char *IDENTIFIER = "VOLTAGE_OUT";

	/* Construct a new VoltageIn */
	VoltageOut(const std::vector<std::string>& tokens, AudioManager *am);

	/**
	 * @brief Destroys a voltage input.
	 */
	~VoltageOut() { }

	/* Convert a voltage output to a string */
	std::string to_string() override;

	/* Measure the potential difference across output terminals */
	double measure(LinearSystem& sys, Eigen::VectorXd& soln);

	std::vector<std::string> unknowns() override;
	/* map unknowns into matrix indices in a linear system */
	void map_unknowns(std::unordered_map<std::string, int> mapping) override;

private:
	int npos;  /**< Positive terminal */
	int nneg;  /**< Negative terminal */

	int npid;
	int nnid;

	AudioManager *am; /**< Where we report output voltages to */
};

#endif /* _VOLTAGE_OUT_H_ */