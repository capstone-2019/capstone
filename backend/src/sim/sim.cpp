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
#include <chrono>
#include <sim.hpp>
#include  <signal.h>

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
 * @brief File descriptors for pipe used to communicate with plotting
 * script if plotting is enabled.
 */
static int plotter_fd[2];
#define PIPE_SIDE_READ 0    /**< Index of read descriptor for the pipe */
#define PIPE_SIDE_WRITE 1   /**< Index of write descriptor for the pipe */

/**
 * @brief Arbitrary integer code used for getopt to indicate that the
 * plotting mode has been chose
 */
#define ENABLE_PLOTTING 0xff
#define LIVE_INPUT 0x13
#define LIVE_OUTPUT 0x69

/** @brief Ratio to convert milliseconds to seconds */
#define MS_TO_S 1000

static FILE *_fp;
volatile bool stop_simulation = false;

/**
 * @brief Handles fatal errors by printing a mesaage and exiting.
 *
 * @param fmt A format string for the message.
 * @param ... Format string arguments.
 */
void sim_error(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    fprintf(stderr, "[FATAL SIMULATOR ERROR]: ");
    vfprintf(stderr, fmt, vl);
    va_end(vl);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

static void sigusr1_handler(int signo) {
    fprintf(_fp, "Process received signal %d\n", signo);
    fclose(_fp);
    stop_simulation = true;
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
        NULL,
    };

    if (pipe(plotter_fd) < 0)
        sim_error("Failed to setup pipe with plotter process.");

    /* run plotter in child process */
    int pid = fork();

    /* child process runs plotter script, parent reaps child */
    if (pid == 0) {

        /* no need for the write fd in child process */
        if (close(plotter_fd[PIPE_SIDE_WRITE]) < 0)
            sim_error("Failed to close pipe's write fd in child process.");

        /* redirect IO in child such that stdin maps to a temp file
         * used to pass data between simulator and plotter */
        if (dup2(plotter_fd[PIPE_SIDE_READ], STDIN_FILENO) < 0)
            sim_error("Failed to redirect IO in child process");

        /* we can use STDIN_FILENO now, so we can close the read descriptor */
        if (close(plotter_fd[PIPE_SIDE_READ]) < 0)
            sim_error("Failed to close pipe's read fd in child process");

        /* run the plotter */
        execvp(plotter_args[0], plotter_args);

        /* SHOULD NEVER REACH HERE! */
        sim_error("%s:%d - execvp() failed with error %s",
            __FUNCTION__, __LINE__, strerror(errno));

    }
    else if (pid < 0) {
        sim_error("Failed to fork() plotting script.");
    }

    if (close(plotter_fd[PIPE_SIDE_READ]) < 0)
        sim_error("Failed to close pipe's read fd in parent process");

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
    const char *usage_string = "-c CIRCUIT_NETLIST -s SIGNAL_FILE -o OUTFILE";
    fprintf(stderr, "Usage: %s %s\n", argv[0], usage_string);
    fprintf(stderr, "\t-c [--circuit]     Circuit netlist file to simulate\n");
    fprintf(stderr, "\t-s [--signal]      Input signal source\n");
    fprintf(stderr, "\t-o [--outfile]   Output audio file\n");
    fprintf(stderr, "\t   [--live-input]  Use live input\n");
    fprintf(stderr, "\t   [--live-output] Play signal as it's being processed\n");
    fprintf(stderr, "\t[--plot]         Plot the results after simulation\n");

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
        {"live-input",     no_argument,       0, LIVE_INPUT },
        {"live-output",    no_argument,       0, LIVE_OUTPUT },
        {"outfile", required_argument, 0, 'o' },
        {"plot",    no_argument,       0, ENABLE_PLOTTING },
    };

    int c;  /* Command line option identifier */

    /* zero out all of the simulator parameters */
    memset(params, 0, sizeof(*params));

    /* parse all command line options */
    while ((c = getopt_long(argc, argv, "c:s:o:h", options, NULL)) != -1) {
        switch(c) {
            case 'c':
                params->circuit_file = optarg;
                break;
            case 's':
                params->signal_file = optarg;
                break;
            case 'o':
                params->outfile = optarg;
                break;
            case ENABLE_PLOTTING:
                params->plot = true;
                break;
            case LIVE_INPUT:
                params->live_input = true;
                break;
            case LIVE_OUTPUT:
                params->live_output = true;
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
    if (params->circuit_file == NULL ||
        (params->signal_file == NULL && !params->live_input)) {
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
    vector<double> timescale;
    vector<double> input_signal;
    vector<double> output_signal;

    _fp = fopen("outfile.log", "w");

    /* install sigusr1 handler for comm. with frontend */
    signal(SIGUSR1, sigusr1_handler);
    fprintf(_fp, "%s:%d Installed SIGUSR1 Handler\n", __FUNCTION__, __LINE__);
    fprintf(_fp, "My PID is %d, sigusr1 is %d\n", getpid(), SIGUSR1);
    fflush(_fp);

    parse_command_line(argc, argv, &params);
    NetlistParser parser(&params);

    /* launch the plotting script if the user requested it */
    if (params.plot)
        plotter_pid = launch_plotter();

    /* read circuit description from netlist */
    Circuit& c = parser.as_circuit();

    /* get starting time */
    auto t0 = std::chrono::high_resolution_clock::now();

    /* run transient analysis */
    c.transient(timescale, input_signal, output_signal);
    fprintf(_fp, "we here\n");

    /* get ending time and print timing summary */
    auto t1 = std::chrono::high_resolution_clock::now();
    auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    double elapsed_s = static_cast<float>(elapsed_ms) / MS_TO_S;
    cout << "Transient analysis finished in " << elapsed_s << " secs." << endl;

    /* Pass data into plotting script, wait for plotter to complete */
    if (params.plot) {

        /* Write data into pipe to communicate with plotter */
        for (int i = 0; i < timescale.size(); i++) {
            dprintf(plotter_fd[PIPE_SIDE_WRITE], "%f %f %f\n",
                timescale[i], input_signal[i], output_signal[i]);
        }

        /* close the write side of the PIPE, child should see EOF now */
        if (close(plotter_fd[PIPE_SIDE_WRITE]) < 0)
            sim_error("Failed to close pipe's read fd in parent process.");

        /* wait for child to exit */
        waitpid(plotter_pid, NULL, 0);
    }

    while (1);
    fclose(_fp);

    return 0;
}
