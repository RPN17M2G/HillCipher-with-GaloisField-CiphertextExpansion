#ifndef CSPRNG_H
#define CSPRNG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sodium.h>

#include "StatusCodes.h"
#include "log.h"

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

/**
 * @brief Perform a secure Fisher-Yates shuffle on an array.
 *
 * @param array - The array to be shuffled.
 * @param length - The length of the array.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE secure_fisher_yates_shuffle(uint8_t *array, size_t length);

#endif
