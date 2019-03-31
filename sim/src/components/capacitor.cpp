#include <components/component.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

Capacitor::Capacitor(const vector<string>& tokens) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	capacitance = parse_by_unit(tokens[4]);
	std::cout << *this << std::endl;
}

string Capacitor::to_string() {
	std::ostringstream capacitor_string;
	capacitor_string << "Capacitor from node(-): "
	                 << nneg
	                 << " to node(+): "
	                 << npos
	                 << " with capacitance of "
	                 << capacitance
	                 << " Farads";
	return capacitor_string.str();
}
