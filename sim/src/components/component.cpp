/**
 *
 * @file component.cpp
 *
 * @date March 31, 2019
 *
 * @brief This file contains functions that are common to all component
 * types.
 *
 * @bug No known bugs.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#include <iostream>
#include <ctype.h>
#include <string>
#include <components/component.hpp>

using std::string;

/**
 * @brief Maps a unit suffix to the corresponding scaling factor.
 *
 * @param unit The unit suffix
 *
 * @return The amount by which a base value should be scaled based on this
 * unit suffix (i.e. k -> 1000)
 */
double Component::get_unit_scale(const std::string& unit) {
	if (unit == "meg")
		return 1.0e+6;
	else if (unit == "f")
		return 1.0e-15;
	else if (unit == "p")
		return 1.0e-12;
	else if (unit == "n")
		return 1.0e-9;
	else if (unit == "u")
		return 1.0e-6;
	else if (unit == "m")
		return 1.0e-3;
	else if (unit == "k")
		return 1.0e+3;
	else if (unit == "g")
		return 1.0e+9;
	else if (unit == "t")
		return 1.0e+12;
	else {
		std::cerr << "Parser Error: Unrecognized unit - " << unit << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Parses a value, which potentially contains a unit suffix into
 * a double.
 *
 * @param value The string to parse.
 *
 * @return A double representing the number that was parsed out from the
 * input string.
 */
double Component::parse_by_unit(const string& value) {
	/* get the base value (without accounting for units) */
	std::size_t unit_index;
	double base = stod(value, &unit_index);

	/* no extra unit specified */
	if (unit_index == value.size())
		return base;

	/* scale based on specified unit */
	std::size_t len = value.size() - unit_index;
	string unit = value.substr(unit_index, len);
	double unit_scale = get_unit_scale(unit);
	return base * unit_scale;
}
