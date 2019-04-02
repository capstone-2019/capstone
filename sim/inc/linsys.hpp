/**
 *
 * @file linsys.hpp
 *
 * @date April 1, 2019
 *
 * @brief Provides the interface to linear systems of equations derived
 * from performing KCLs at various nodes in the circuit.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _LINEAR_SYSTEM_H_
#define _LINEAR_SYSTEM_H_

#include <Eigen/Dense>
#include <unordered_map>
#include <stdio.h>
#include <errors.hpp>
#include <sstream>

/**
 * @brief Represents the system of linear equations Ax=B derived from
 * KCL that will be used to solve for unknowns in the circuit.
 */
struct LinearSystem {
	int ground;         /**< Circuit's ground node */
	Eigen::MatrixXd A;  /**< LHS matrix of system of linear equations */
	Eigen::VectorXd x;  /**< Solution vector */
	Eigen::VectorXd B;  /**< RHS vector of system of linear equations */

	/** @brief Maps the string representations of unknowns to their ids */
	std::unordered_map<std::string, int> unknowns_map;

	/** @brief Type representing unknown sized vector of strings */
	typedef Eigen::Matrix<std::string, Eigen::Dynamic, 1> VectorXs;
	VectorXs unknown_labels; /**< Vector of unknown labels mapping to `x` */

	/* construct a linear system */
	LinearSystem(int num_unknowns, int ground_id,
		std::unordered_map<std::string, int> unknowns);

	/* destroy a linear system */
	~LinearSystem() { }

	/* Zero our a linear system */
	void clear();

	/* get a string representation of the system */
	std::string to_string();

	/* solve the system of linear equations */
	Eigen::VectorXd& solve();

	/* add component contributions to the LHS/RHS of the system */
	void increment_lhs(int r, int c, double delta);
	void increment_rhs(int r, double delta);

	/**
	 * @brief Writes a linear system to an output stream.
	 *
	 * @param out The output stream.
	 * @param sys The linear system.
	 *
	 * @return The updated output stream.
	 */
	friend std::ostream& operator<<(std::ostream& out, LinearSystem& sys) {
		out << sys.to_string();
		return out;
	}
};

#endif
