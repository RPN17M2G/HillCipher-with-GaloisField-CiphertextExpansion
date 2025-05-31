#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sodium.h>

#include "../StatusCodes.h"

/**
 * @brief Initialize sodium.
 *
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE initialize_sodium_library();

/**
 * @brief Generate a cryptography secure random number in a given range.
 *
 * @param out_number - A pointer to the output variables.
 * @param minimum_value - The minimum value for the random number.
 * @param maximum_value - The maximum value for the random number.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_secure_random_number(uint32_t* out_number, uint32_t minimum_value, uint32_t maximum_value);
