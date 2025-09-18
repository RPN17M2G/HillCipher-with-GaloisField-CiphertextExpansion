#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"
#include "IO/VerbosityControl.h"

#define UINT8_HEX_CHARS_PER_ELEMENT 4
#define INT64_HEX_CHARS_PER_ELEMENT 24
#define MATRIX_HEX_CHARS_PER_ELEMENT 24
#define PRINT_BUFFER_EXTRA 128

/**
 * @brief Print a uint8_t vector to the console with optional verbose output
 *
 * @param data Pointer to the uint8_t vector to be printed
 * @param size Size of the vector in bytes
 * @param prefix Prefix string to be printed before the vector
 * @param is_verbose_only If true, prints only in verbose mode
 */
void print_uint8_vector(const uint8_t* data, size_t size, const char* prefix, bool is_verbose_only);

/**
 * @brief Print a int64_t vector to the console with optional verbose output
 *
 * @param data Pointer to the int64_t vector to be printed
 * @param size Size of the vector in bytes
 * @param prefix Prefix string to be printed before the vector
 * @param is_verbose_only If true, prints only in verbose mode
 */
void print_int64_vector(const int64_t* data, size_t size, const char* prefix, bool is_verbose_only);

/**
 * @brief Print a matrix to the console with optional verbose output
 *
 * @param matrix Pointer to the matrix to be printed
 * @param dimension Dimension of the square matrix
 * @param prefix Prefix string to be printed before the matrix
 * @param is_verbose_only If true, prints only in verbose mode
 */
void print_matrix(int64_t** matrix, uint32_t dimension, const char* prefix, bool is_verbose_only);

#endif
