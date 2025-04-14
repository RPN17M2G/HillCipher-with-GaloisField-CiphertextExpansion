#pragma once

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"

#define IS_EVEN(number) ((number) % 2 == 0)
#define IS_ODD(number) ((number) % 2 != 0)

/**
 * @brief Calculates the determinant of a square matrix.
 *
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - The galois prime field
 * @param out_determinant - Pointer to the output determinant value.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, int64_t* out_determinant);

/**
 * @brief Calculates the determinant of a square matrix over a finite field.
 *
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - The galois field to calculate on.
 * @param out_determinant - Pointer to the output determinant value.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant_over_galois_field(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, int64_t* out_determinant);

/**
 * @brief Calculates the inverse of a square matrix.
 *
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @param out_inverse_matrix - Pointer to the output inverse matrix - allocated inside the function and memory released if fails.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE inverse_square_matrix(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, int64_t*** out_inverse_matrix);

/**
 * @brief Calculates the inverse of an element.
 *
 * @param out_element - Pointer to the output inverse element.
 * @param prime_field - Prime field to use for calculations.
 * @param element_to_inverse - The element to inverse.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE inverse_element(int64_t* out_element, uint32_t prime_field, int64_t element_to_inverse);

/**
 * @brief Multiplies a square matrix with a vector.
 *
 * @param matrix - Pointer to the input matrix.
 * @param vector - Pointer to the input vector.
 * @param dimentaion - Dimension of the square matrix and vector.
 * @param prime_field - Prime field to use for calculations.
 * @param out_vector - Pointer to the output vector - allocated inside the function and memory released if fails.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE multiply_matrix_with_uint8_t_vector(int64_t** out_vector, int64_t** matrix, uint8_t* vector, uint32_t dimentaion, uint32_t prime_field);

/**
 * @brief Multiplies a square matrix with a vector for decryption, the result vector is uint8_t.
 *
 * @param matrix - Pointer to the input matrix.
 * @param vector - Pointer to the input vector.
 * @param dimentaion - Dimension of the square matrix and vector.
 * @param prime_field - Prime field to use for calculations.
 * @param out_vector - Pointer to the output vector - allocated inside the function and memory released if fails.
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
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param row - Row to be removed.
 * @param column - Column to be removed.
 * @param out_matrix - Pointer to the output minor matrix - allocated inside the function and memory released if fails.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE build_minor_matrix(int64_t** matrix, uint32_t dimentaion, uint32_t row, uint32_t column, int64_t*** out_matrix);

/**
 * @brief Calculates the greatest common divisor (GCD) of two elements.
 *
 * @param first_element - First element.
 * @param second_element - Second element.
 * @param out_gcd - Pointer to the output GCD value.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE gcd(int64_t first_element, int64_t second_element, int64_t* out_gcd);

/**
 * @brief Checks if a matrix is invertible.
 *
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @param out_is_invertible - Pointer to the output boolean indicating if the matrix is invertible.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE is_matrix_invertible(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, bool* out_is_invertible);

