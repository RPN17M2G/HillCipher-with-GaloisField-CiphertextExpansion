#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"

#define UINT8_HEX_CHARS_PER_ELEMENT 4
#define INT64_HEX_CHARS_PER_ELEMENT 24
#define MATRIX_HEX_CHARS_PER_ELEMENT 24
#define PRINT_BUFFER_EXTRA 128

/**
 * @brief Print a uint8_t vector to the console.
 *
 * @param data - Pointer to the uint8_t vector to be printed.
 * @param size - Size of the vector in bytes.
 * @param prefix - Prefix string to be printed before the vector.
 * @return STATUS_CODE - Status of the operation.
 */
void print_uint8_vector(const uint8_t* data, size_t size, const char* prefix);

/**
 * @brief Print a int64_t vector to the console.
 *
 * @param data - Pointer to the int64_t vector to be printed.
 * @param size - Size of the vector in bytes.
 * @param prefix - Prefix string to be printed before the vector.
 * @return STATUS_CODE - Status of the operation.
 */
void print_int64_vector(const int64_t* data, size_t size, const char* prefix);

/**
 * @brief Print a matrix to the console.
 *
 * @param matrix - Pointer to the matrix to be printed.
 * @param dimension - Dimension of the square matrix.
 */
void print_matrix(int64_t** matrix, uint32_t dimension);

#endif
