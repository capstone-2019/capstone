#include <circuit.hpp>
#include <parser/netparser.hpp>
#include <iostream>
#include <string>

using std::string;

Circuit::Circuit(const char *netfile) {
	NetlistIterator ni(netfile);
	for (auto it = ni.cbegin(); it != ni.cend(); it++) {
		const std::string& line = *it;
	}
}