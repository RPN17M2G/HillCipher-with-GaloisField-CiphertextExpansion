#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "StatusCodes.h"

/**
 * @brief Generates an encryption matrix with cryptography secure random values.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param dimentation - Dimension of the square matrix.
 * @param prime_field - Prime field to use for generating random values.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_encryption_matrix(double*** out_matrix, uint32_t dimentation, uint32_t prime_field);
