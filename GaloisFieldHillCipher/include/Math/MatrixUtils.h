#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../StatusCodes.h"

/**
 * @brief Frees the memory allocated for a matrix.
 *
 * @param matrix - Pointer to the matrix to be freed.
 * @param dimentaion - Dimension of the square matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE free_matrix(int64_t** matrix, const uint32_t dimentaion);
