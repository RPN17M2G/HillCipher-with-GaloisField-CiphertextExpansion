#ifndef VERBOSITY_CONTROL_H
#define VERBOSITY_CONTROL_H

#include <stdbool.h>

extern bool g_verbose_mode;

/**
 * @brief Sets the verbosity mode for logging.
 *
 * @param verbose If true, enables verbose mode
 */
void set_verbose_mode(bool verbose);

/**
 * @brief Checks if the verbose mode is enabled.
 *
 * @return true if verbose mode is enabled, false otherwise
 */
bool is_verbose_mode();

#endif
