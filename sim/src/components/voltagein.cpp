#include <components/component.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

VoltageIn::VoltageIn(const vector<string>& tokens) {
	npos = stoi(tokens[2]);
	nneg = stoi(tokens[3]);
	signal_file = tokens[4];
	std::cout << *this << std::endl;
}

string VoltageIn::to_string() {
	std::ostringstream vin_string;
	vin_string << "Voltage Input from node(-): "
	           << nneg
	           << " to node(+): "
	           << npos
	           << " based on the signal '"
	           << signal_file
	           << "'";
	return vin_string.str();
}