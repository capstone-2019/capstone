#include <components/component.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;

VoltageOut::VoltageOut(const vector<string>& tokens) {
	npos = stod(tokens[2]);
	nneg = stod(tokens[3]);
	std::cout << *this << std::endl;
}

string VoltageOut::to_string() {
	std::ostringstream vout_string;
	vout_string << "Voltage Output measured across node(+): "
	            << npos
	            << " to node(-): "
	            << nneg;
	return vout_string.str();
}