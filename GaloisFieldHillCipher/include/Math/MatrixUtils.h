#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StatusCodes.h"
#include "log.h"
#include "Math/MathUtils.h"

/**
 * @brief Frees the memory allocated for a matrix.
 *
 * @param matrix - Pointer to the matrix to be freed.
 * @param rows - Number of rows in the matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE free_int64_matrix(int64_t** matrix, const uint32_t rows);

/**
 * @brief Frees the memory allocated for a matrix.
 *
 * @param matrix - Pointer to the matrix to be freed.
 * @param rows - Number of rows in the matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE free_uint8_matrix(uint8_t** matrix, const uint32_t rows);

/**
 * @brief Generates a square matrix with cryptography secure random values.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - Prime field to use for generating random values.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_square_matrix_over_field(int64_t*** out_matrix, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Generates a square matrix with cryptography secure random values.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param rows - Number of rows in the matrix.
 * @param columns - Number of columns in the matrix.
 * @param prime_field - Prime field to use for generating random values.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_matrix_over_field(int64_t*** out_matrix, uint32_t rows, uint32_t columns, uint32_t prime_field);

/**
 * @brief Checks if a matrix is invertible.
 *
 * @param out_is_invertible - Pointer to the output boolean indicating if the matrix is invertible.
 * @param matrix - Pointer to the input matrix.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE is_matrix_invertible(bool* out_is_invertible, int64_t** matrix, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Builds a minor matrix by removing a specified row and column from the input matrix.
 *
 * @param out_matrix - Pointer to the output minor matrix - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param dimension - Dimension of the square matrix.
 * @param row - Row to be removed.
 * @param column - Column to be removed.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE build_minor_matrix(int64_t*** out_matrix, int64_t** matrix, uint32_t dimension, uint32_t row, uint32_t column);

#endif
