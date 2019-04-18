/** @file sim.hpp
 *
 *  @brief contains data structures used in sim.cpp
 *
 */

#ifndef _SIM_H_
#define _SIM_H_


/**
 * @brief Struct used to store command line arguments to the simulator.
 */
typedef struct {
    const char *circuit_file;  /**< Path to circuit netlist */
    const char *signal_file;   /**< Input signal source */
    bool plot;                 /**< Whether to plot the output signal */
    bool live_output;          /**< Whether to play out the signal live */
    bool live_input;           /**< whether to use live audio input */
    const char *outfile;
} simparams_t;


#endif /* _SIM_H_ */