/**
 *
 * @file sim.cpp
 *
 * @date March 31, 2019
 *
 * @brief This file contains the top-level main routine driving the circuit
 * simulator.
 *
 * @bug No known bugs.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#include <iostream>
#include <parser/netparser.hpp>
#include <components/component.hpp>
#include <stdio.h>
#include <getopt.h>
#include <circuit.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

/**
 * @brief Struct used to store command line arguments to the simulator.
 */
typedef struct {
	const char *circuit_file;  /**< Path to circuit netlist */
	const char *signal_file;   /**< Input signal source */
} simparams_t;


/**
 * @brief Shows the usage instructions for the program and exits, indicating
 * a failure.
 *
 * This should be called if the user does not provide the required arguments
 * or if they specify the -h/--help flags.
 *
 * @param argv The argument vector used to invoke this program.
 */
void usage(char *argv[]) {
	const char *usage_string = "-c CIRCUIT_NETLIST -s SIGNAL_FILE";
	fprintf(stderr, "Usage: %s %s\n", argv[0], usage_string);
	fprintf(stderr, "\t-c [--circuit]   Circuit netlist file to simulate\n");
	fprintf(stderr, "\t-s [--signal]    Input signal source\n");
	exit(EXIT_FAILURE);
}

/**
 * @brief Parses the command line arguments and fills the results into
 * a `simparams_t` struct.
 *
 * @param argc The number of command line arguments provided.
 * @param argv The vector of command line arguments.
 * @param params Pointer to `simparams_t` object to be filled in with the
 * results of parsing the command line arguments.
 *
 * @note If this function detects an error in the command line argument vector,
 * or if the user specified the -h/--help flag, this function will print
 * the usage instructions to stderr and exit the simulator.
 */
void parse_command_line(int argc, char *argv[], simparams_t *params) {

	/* options to be parsed by getopt_long() */
	static struct option options[] = {
		{"help",    no_argument,       0, 'h'},
		{"circuit", required_argument, 0, 'c'},
		{"signal",  required_argument, 0, 's'},
	};

	int c;  /* Command line option identifier */

	/* zero out all of the simulator parameters */
	memset(params, 0, sizeof(*params));

	/* parse all command line options */
	while ((c = getopt_long(argc, argv, "c:s:h", options, NULL)) != -1) {
		switch(c) {
			case 'c':
				params->circuit_file = optarg;
				break;
			case 's':
				params->signal_file = optarg;
				break;
			case 'h':
				usage(argv);
				break;
			default:
				usage(argv);
				break;
		}
	}

	/* user must specify these options */
	if (params->circuit_file == NULL || params->signal_file == NULL) {
		usage(argv);
	}
}

/**
 * @brief Top-level main routine to launch the simulator.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of command line arguments.
 *
 * @return 0 on success or a negative error code on failure.
 */
int main(int argc, char *argv[]) {
    simparams_t params;
    parse_command_line(argc, argv, &params);
    NetlistParser parser(params.circuit_file);
    return 0;
}
