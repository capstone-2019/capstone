#include <components/component.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;
using Eigen::VectorXd;
using std::unordered_map;

Inductor::Inductor(const vector<string>& tokens) {
    name = tokens[1];
    npos = stoi(tokens[2]);
    nneg = stoi(tokens[3]);
    inductance = parse_by_unit(tokens[4]);
}

string Inductor::to_string() {
    std::ostringstream inductor_string;
    inductor_string << "Inductor from node(-): "
                    << nneg
                    << " to node(+): "
                    << npos
                    << " with inductance of "
                    << inductance
                    << "H";
    return inductor_string.str();
}

vector<string> Inductor::unknowns() {
    vector<string> unknown_variables;
	unknown_variables.push_back(unknown_voltage(npos));
	unknown_variables.push_back(unknown_voltage(nneg));
    unknown_variables.push_back(unknown_current(name));
	return unknown_variables;
}

void Inductor::map_unknowns(unordered_map<string, int> mappings) {
	this->n1 = mappings[unknown_voltage(npos)];
	this->n2 = mappings[unknown_voltage(nneg)];
    this->ni = mappings[unknown_current(name)];
}

void Inductor::add_contribution(LinearSystem& sys, VectorXd& soln,
	VectorXd& prev_soln, double dt) {

	float L_div_dt = this->inductance/ dt;

    sys.increment_lhs(n1, ni, +1);
    sys.increment_rhs(n1, -prev_soln(ni));

    sys.increment_lhs(n2, ni, -1);
    sys.increment_rhs(n2, prev_soln(ni));

    sys.increment_lhs(ni, ni, L_div_dt);
    sys.increment_lhs(ni, n1, -1);
    sys.increment_lhs(ni, n2, +1);
}