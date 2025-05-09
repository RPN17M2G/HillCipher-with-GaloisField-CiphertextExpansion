#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "FieldBasicOperations.h"

#define MEMORY_BUFFER_FOR_PLAINTEXT_BLOCK (3)

#define IS_EVEN(number) ((number) % 2 == 0)
#define IS_ODD(number) ((number) % 2 != 0)

/**
 * @brief Calculates the determinant of a square matrix.
 *
 * @param out_determinant - Pointer to the output determinant value.
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - The galois prime field.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant(int64_t* out_determinant, int64_t** matrix, uint32_t dimentaion, uint32_t prime_field);

/**
 * @brief Calculates the determinant of a square matrix over a finite field.
 *
 * @param out_determinant - Pointer to the output determinant value.
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - The galois field to calculate on.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant_over_galois_field(int64_t* out_determinant, int64_t** matrix, uint32_t dimentaion, uint32_t prime_field);

/**
 * @brief Calculates the inverse of a square matrix.
 *
 * @param out_inverse_matrix - Pointer to the output inverse matrix - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE inverse_square_matrix(int64_t*** out_inverse_matrix, int64_t** matrix, uint32_t dimentaion, uint32_t prime_field);

/**
 * @brief Multiplies a square matrix with a vector.
 *
 * @param out_vector - Pointer to the output vector - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param vector - Pointer to the input vector.
 * @param dimentaion - Dimension of the square matrix and vector.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE multiply_matrix_with_uint8_t_vector(int64_t** out_vector, int64_t** matrix, uint8_t* vector, uint32_t dimentaion, uint32_t prime_field);

/**
 * @brief Multiplies a square matrix with a vector for decryption, the result vector is uint8_t.
 *
 * @param out_vector - Pointer to the output vector - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param vector - Pointer to the input vector.
 * @param dimentaion - Dimension of the square matrix and vector.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE multiply_matrix_with_int64_t_vector(uint8_t** out_vector, int64_t** matrix, int64_t* vector, uint32_t dimentaion, uint32_t prime_field);

/**
 * @brief Frees the memory allocated for a matrix.
 *
 * @param matrix - Pointer to the matrix to be freed.
 * @param dimentaion - Dimension of the square matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE free_matrix(int64_t** matrix, const uint32_t dimentaion);

/**
 * @brief Builds a minor matrix by removing a specified row and column from the input matrix.
 *
 * @param out_matrix - Pointer to the output minor matrix - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param row - Row to be removed.
 * @param column - Column to be removed.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE build_minor_matrix(int64_t*** out_matrix, int64_t** matrix, uint32_t dimentaion, uint32_t row, uint32_t column);

/**
 * @brief Calculates the greatest common divisor (GCD) of two elements.
 *
 * @param out_gcd - Pointer to the output GCD value.
 * @param first_element - First element.
 * @param second_element - Second element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE gcd(int64_t* out_gcd, int64_t first_element, int64_t second_element);

/**
 * @brief Checks if a matrix is invertible.
 *
 * @param out_is_invertible - Pointer to the output boolean indicating if the matrix is invertible.
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE is_matrix_invertible(bool* out_is_invertible, int64_t** matrix, uint32_t dimentaion, uint32_t prime_field);

/**
 * @brief Generates a square matrix with cryptography secure random values.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - Prime field to use for generating random values.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_square_matrix_over_field(int64_t*** out_matrix, uint32_t dimension, uint32_t prime_field);
