#pragma once

#include <cstdlib>
#include <cstdint>
#include <math.h>

#include "StatusCodes.h"

#define IS_EVEN(number) (number % 2 == 0)
#define IS_ODD(number) (number % 2 != 0)

/**
 * @brief Calculates the determinant of a square matrix.
 *
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param out_determinant - Pointer to the output determinant value.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant(double** matrix, uint32_t dimentaion, double* out_determinant);

/**
 * @brief Calculates the inverse of a square matrix using .
 *
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @param out_inverse_matrix - Pointer to the output inverse matrix - allocated inside the function and memory released if fails.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE square_matrix_inverse(double** matrix, uint32_t dimentaion, uint32_t prime_field, double*** out_inverse_matrix);

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
STATUS_CODE matrix_multipication_with_vector(double** out_vector, double** matrix, uint8_t* vector, uint32_t dimentaion, uint32_t prime_field);

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
STATUS_CODE matrix_multipication_with_vector(uint8_t** out_vector, double** matrix, double* vector, uint32_t dimentaion, uint32_t prime_field);


/**
 * @brief Frees the memory allocated for a matrix.
 *
 * @param matrix - Pointer to the matrix to be freed.
 * @param dimentaion - Dimension of the square matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE free_matrix(double** matrix, const uint32_t dimentaion);

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
STATUS_CODE build_minor_matrix(double** matrix, uint32_t dimentaion, uint32_t row, uint32_t column, double*** out_matrix);

/**
 * @brief Calculates the greatest common divisor (GCD) of two elements.
 *
 * @param first_element - First element.
 * @param second_element - Second element.
 * @param out_gcd - Pointer to the output GCD value.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE gcd(double first_element, double second_element, double* out_gcd);

/**
 * @brief Checks if a matrix is invertible.
 *
 * @param matrix - Pointer to the input matrix.
 * @param dimentaion - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @param out_is_invertible - Pointer to the output boolean indicating if the matrix is invertible.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE is_matrix_invertible(double** matrix, uint32_t dimentaion, uint32_t prime_field, bool* out_is_invertible);

