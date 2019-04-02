#include <linsys.hpp>
#include <components/component.hpp>
#include <iostream>

using std::cout;
using std::endl;

void print_unknowns_map(std::unordered_map<std::string, int> m) {
	for (auto it = m.begin(); it != m.end(); it++) {
		auto key = it->first;
		auto val = it->second;
		cout << key << " -> " << val << endl;
	}
}

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

void LinearSystem::clear() {
	A.setZero();
	x.setZero();
	B.setZero();
	A(ground, ground) = 1.0;
}

std::string LinearSystem::to_string() {
	std::ostringstream sysstream;
	sysstream << "---------------------------------------------------------"
	          << "Linear System: " << endl
	          << "Ground node is: " << ground << endl
		      << "A = " << endl << A << endl << endl
		      << "x = " << endl << x << endl << endl
		      << "B = " << endl << B << endl << endl
		      << "L = " << endl << unknown_labels << endl << endl
		      << "---------------------------------------------------------"
		      << endl;
	return sysstream.str();
}

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
