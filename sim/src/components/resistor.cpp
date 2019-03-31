#include <components/component.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

Resistor::Resistor(const vector<string>& tokens) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	resistance = parse_by_unit(tokens[4]);
	std::cout << *this << std::endl;
}

string Resistor::to_string() {
	std::ostringstream resistor_string;
	resistor_string << "Resistor from node(-): "
	                << nneg
	                << " to node(+) "
	                << npos
	                << " with resistance of "
	                << resistance
	                << " Ohms";
	return resistor_string.str();
}