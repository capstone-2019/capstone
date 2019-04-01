#ifndef _LINEAR_SYSTEM_H_
#define _LINEAR_SYSTEM_H_

#include <Eigen/Dense>

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

	/**
	 * @brief Constructs a new linear system.
	 *
	 * @param ground_id The ground node identifier.
	 */
	LinearSystem(int num_unknowns, int ground_id,
		std::unordered_map<std::string, int> unknowns_map) {

		A = Eigen::MatrixXd(num_unknowns, num_unknowns);
		x = Eigen::VectorXd(num_unknowns);
		B = Eigen::VectorXd(num_unknowns);
		A(ground_id, ground_id) = 1.0;
	}

	/**
	 * @brief Destroys the linear system.
	 */
	~LinearSystem() { }

	/**
	 * @brief Solves the system of equations. After calling this function,
	 * the `x` vector will contain the solution.
	 */
	void solve() {
		x = A.colPivHouseholderQr().solve(B);
	}

	/**
	 * @brief Increments the LHS of the system of equations at a given
	 * position by a provided delta.
	 *
	 * @param r The row to update.
	 * @param c The column to update.
	 * @param delta The value to increment by.
	 */
	void increment_lhs(int r, int c, double delta) {
		if (r != ground)
			A(r, c) += delta;
	}

	/**
	 * @brief Increments the RHS of the system of equations at a given
	 * position by a provided delta.
	 *
	 * @param r The row to update.
	 * @param delta The value to increment by.
	 */
	void increment_rhs(int r, double delta) {
		if (r != ground)
			x(r) += delta;
	}
};

#endif