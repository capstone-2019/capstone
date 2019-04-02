/**
 *
 * @file errors.hpp
 *
 * @date April 2, 2019
 *
 * @brief Defines useful functions/macros for debugging and error handling.
 *
 * @author Matthew Kasper (mkasper@andrew.cmu.edu)
 *
 */

#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdio.h>

/** @brief Macro to log function/line numbers in the code for debugging */
#define DBG_LOG fprintf(stderr, "%s:%d\n", __FUNCTION__, __LINE__);

/* handles fatal simulator errors by printing a message and exiting */
void sim_error(const char *fmt, ...);

#endif /* _ERRORS_H_ */
