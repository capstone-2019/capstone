/**
 *
 * @file sim.cpp
 *
 * @date March 31, 2019
 *
 * @brief This file contains the top-level main routine driving the circuit
 * simulator.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#include <iostream>
#include <stdbool.h>
#include <parser/netparser.hpp>
#include <components/component.hpp>
#include <stdio.h>
#include <getopt.h>
#include <circuit.hpp>
#include <unistd.h>
#include <sys/wait.h>

using Eigen::MatrixXd;
using Eigen::Upper;
using Eigen::Lower;

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

extern char **environ;

/**
 * @brief Struct used to store command line arguments to the simulator.
 */
typedef struct {
	const char *circuit_file;  /**< Path to circuit netlist */
	const char *signal_file;   /**< Input signal source */
	bool plot;                 /**< Whether to plot the output signal */
} simparams_t;

/**
 * @brief Arbitrary integer code used for getopt to indicate that the
 * plotting mode has been chose
 */
#define ENABLE_PLOTTING 0xFF

static void sim_error(const char *fmt, ...) {
	va_list vl;
	va_start(vl, fmt);
	fprintf(stderr, "[FATAL SIMULATOR ERROR]: ");
	vfprintf(stderr, fmt, vl);
	va_end(vl);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}

/**
 * @brief Runs a Python script to plot the results of the circuit simulator
 * using Matplotlib.
 *
 * The script is run in a child process.
 *
 * @return Does not return in the child process. In the parent process, this
 * function returns the pid of the process spawned to run the plotting script.
 * It is the caller's responsibility to call `waitpid()` at some point in the
 * future to reap this child process.
 */
static int launch_plotter() {

	/**
	 * Command line arguments to plotting script.
	 *
	 * @bug Right now, this is hardcoded to plot a sine wave. Later, both the
	 * arguments and the script need to be modified to plot the actual
	 * waveform.
	 */
	static char *plotter_args[] = {
		(char *) "python",
		(char *) "scripts/plot.py",
		(char *) "-f",
		(char *) "sin",
		(char *) "--plot",
	};

	/* run plotter in child process */
	int pid = fork();

	/* child process runs plotter script, parent reaps child */
	if (pid == 0) {
		execvp(plotter_args[0], plotter_args);
		sim_error("Failed to run plotting script.");
	}
	else if (pid < 0) {
		sim_error("Failed to fork() plotting script.");
	}

	return pid;
}

/**
 * @brief Shows the usage instructions for the program and exits, indicating
 * a failure.
 *
 * This should be called if the user does not provide the required arguments
 * or if they specify the -h/--help flags.
 *
 * @param argv The argument vector used to invoke this program.
 */
static void usage(char *argv[]) {
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
		{"help",    no_argument,       0, 'h' },
		{"circuit", required_argument, 0, 'c' },
		{"signal",  required_argument, 0, 's' },
	    {"plot",    no_argument,       0,  ENABLE_PLOTTING },
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
			case ENABLE_PLOTTING:
				params->plot = true;
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
    int plotter_pid;

    parse_command_line(argc, argv, &params);
    NetlistParser parser(params.circuit_file);

    /* launch the plotting script if the user requested it */
    if (params.plot)
    	plotter_pid = launch_plotter();

    /* wait for the plotting script to complete */
    if (params.plot)
    	waitpid(plotter_pid, NULL, 0);

    return 0;
}
