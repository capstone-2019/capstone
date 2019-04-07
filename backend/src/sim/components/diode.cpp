/**
 *
 * @file diode.cpp
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 * @brief This file definesthe implementation of the diode class, a type
 * of non-linear element supported by the simulator which cuts off current
 * flow when the voltage across the device dips below a threshold.
 *
 * Only uni-directional diodes are supported. Zener diodes are not likely
 * to be included in V0 of the simulator.
 *
 * @date 2019-04-07
 *
 */

#include <vector>
#include <string>
#include <unordered_map>
#include <components/component.hpp>
#include <iostream>
#include <sstream>

using std::vector;
using std::string;
using std::unordered_map;
using Eigen::VectorXd;

/**
 * @brief Construct a new diode object
 *
 * @param tokens Space delimited tokens from the line in the netfile
 * describing this diode instance.
 */
Diode::Diode(const std::vector<std::string>& tokens) {
    npos = stoi(tokens[2]);
    nneg = stoi(tokens[3]);
    N = 1.5;
    IS = 1e-12;
    VT = 0.026;
}

/**
 * @brief Converts a diode object to a string.
 * 
 * @return String representation of the diode.
 */
string Diode::to_string() {
    std::ostringstream diode_string;
    diode_string << "Diode from node(-): "
                 << nneg
                 << " to node(+): "
                 << npos
                 << " with { N = "
                 << N
                 << ", Is = "
                 << IS
                 << ", Vt = "
                 << VT
                 << " }";
    return diode_string.str();
}

/**
 * @brief Get a vector of unknowns that this component contributes to.
 *
 * @return Vector of unknowns this component contributes to.
 */
vector<string> Diode::unknowns() {
    vector<string> unknown_variables;
    unknown_variables.push_back(unknown_voltage(npos));
    unknown_variables.push_back(unknown_voltage(nneg));
    return unknown_variables;
}

/**
 * @brief Pre-computes the mappings from unknown quantities associated with
 * this component to indices that will be used to construct the KCL matrix.
 *
 * @param mappings Hash map which maps string unknown identifiers to
 * integer unknown IDs.
 */
void Diode::map_unknowns(unordered_map<string, int> mappings) {
    this->n1 = mappings[unknown_voltage(npos)];
    this->n2 = mappings[unknown_voltage(nneg)];
}

/**
 * @brief Adds the contributions of this diode to the KCL equations at
 * the appropriate places in the system matrix.
 *
 * @param sys The linear system to manipulate.
 * @param soln System solution from previous timestep.
 * @param prev_soln System solution form previous newton iteration.
 * @param dt Time step for the simulator.
 */
void Diode::add_contribution(LinearSystem& sys, VectorXd& soln,
    VectorXd& prev_soln, double dt) {

    double denom_inv = 1.0 / (this->N * this->VT);
    double prev_soln_delta = prev_soln(n1) - prev_soln(n2);
    double current_val = IS * (exp(prev_soln_delta * denom_inv) - 1);
    double current_deriv = IS * exp(prev_soln_delta * denom_inv) * denom_inv;
    sys.increment_lhs(n1, n1, +current_deriv);
    sys.increment_lhs(n1, n2, -current_deriv);
    sys.increment_lhs(n2, n1, -current_deriv);
    sys.increment_lhs(n2, n2, +current_deriv);
    sys.increment_rhs(n1, -current_val);
    sys.increment_rhs(n2, +current_val);
}

