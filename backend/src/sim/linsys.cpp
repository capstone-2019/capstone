/**
 *
 * @file linsys.cpp
 *
 * @brief This file contains the implementation of a solver for linear
 * system of equations based on top of Eigen.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#include <linsys.hpp>
#include <components/component.hpp>
#include <iostream>

using std::endl;

/**
 * @brief Constructs a new linear system.
 *
 * @param ground_id The ground node identifier.
 */
LinearSystem::LinearSystem(int num_unknowns, int ground_id,
	std::unordered_map<std::string, int> unknowns) {

	ground = unknowns[Component::unknown_voltage(ground_id)];

	A = Eigen::MatrixXd(num_unknowns, num_unknowns);
	A.setZero();
	A(ground, ground) = 1.0;
	x = Eigen::VectorXd(num_unknowns);
	x.setZero();

	B = Eigen::VectorXd(num_unknowns);
	B.setZero();

	unknowns_map = unknowns;
	unknown_labels = VectorXs(num_unknowns);

	for (auto it = unknowns.begin(); it != unknowns.end(); it++) {
		std::string name = it->first;
		int id = it->second;
		unknown_labels(id) = name;
	}
}

/**
 * @brief Zeros out a linear system, and reinitializes the first equation:
 * setting the ground voltage to zero.
 */
void LinearSystem::clear() {
	A.setZero();
	x.setZero();
	B.setZero();
	A(ground, ground) = 1.0;
}

/**
 * @brief Produces a string representation of a system of equations.
 *
 * @return Text representation of the linear system of equations.
 */
std::string LinearSystem::to_string() {
	std::ostringstream sysstream;
	sysstream << "---------------------------------------------------------"
	          << "Linear System: " << endl
	          << "Ground node is: " << ground << endl
		      << "A = "      << endl << A << endl << endl
		      << "x = "      << endl << x << endl << endl
		      << "B = "      << endl << B << endl << endl
		      << "Labels = " << endl << unknown_labels << endl << endl
		      << "---------------------------------------------------------"
		      << endl;
	return sysstream.str();
}

/**
 * @brief Solves the system of equations. After calling this function,
 * the `x` vector will contain the solution.
 *
 * @return The solution vector `x` to the system Ax = B.
 */
Eigen::VectorXd& LinearSystem::solve() {
	x = A.colPivHouseholderQr().solve(B);
	return x;
}

/**
 * @brief Increments the LHS of the system of equations at a given
 * position by a provided delta.
 *
 * @param r The row to update.
 * @param c The column to update.
 * @param delta The value to increment by.
 */
void LinearSystem::increment_lhs(int r, int c, double delta) {
	if (r != ground)
		A(r, c) += delta;
}

/**
 * @brief Increments the RHS of the system of equations at a given
 * position by a provided delta.
 *
 * @param r The row to update.	 * @param delta The value to increment by.
 */
void LinearSystem::increment_rhs(int r, double delta) {
	if (r != ground)
		B(r) += delta;
}
