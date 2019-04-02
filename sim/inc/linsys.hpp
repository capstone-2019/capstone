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

void print_unknowns_map(std::unordered_map<std::string, int> m);

/**
 * @brief Represents the system of linear equations Ax=B derived from
 * KCL that will be used to solve for unknowns in the circuit.
 */
struct LinearSystem {
	int ground;         /**< Circuit's ground node */
	Eigen::MatrixXd A;  /**< LHS matrix of system of linear equations */
	Eigen::VectorXd x;  /**< Solution vector */
	Eigen::VectorXd B;  /**< RHS vector of system of linear equations */
	std::unordered_map<std::string, int> unknowns_map;

	typedef Eigen::Matrix<std::string, Eigen::Dynamic, 1> VectorXs;
	VectorXs unknown_labels;


	LinearSystem(int num_unknowns, int ground_id,
		std::unordered_map<std::string, int> unknowns);

	~LinearSystem() { }

	void clear();

	std::string to_string();

	/**
	 * @brief Solves the system of equations. After calling this function,
	 * the `x` vector will contain the solution.
	 */
	Eigen::VectorXd& solve();

	void increment_lhs(int r, int c, double delta);
	void increment_rhs(int r, double delta);

	friend std::ostream& operator<<(std::ostream& out, LinearSystem& sys) {
		out << sys.to_string();
		return out;
	}
};

#endif
