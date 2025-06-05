#ifndef MATRIXDETERMINANT_H
#define MATRIXDETERMINANT_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "Math/FieldBasicOperations.h"
#include "Math/MatrixUtils.h"
#include "Math/MatrixInverse.h"

/**
 * @brief Calculates the determinant of a square matrix using laplace expansion.
 *
 * @param out_determinant - Pointer to the output determinant value.
 * @param matrix - Pointer to the input matrix.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - The galois prime field.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant_laplace_expansion(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Calculates the determinant of a square matrix over a finite field using laplace expansion.
 *
 * @param out_determinant - Pointer to the output determinant value.
 * @param matrix - Pointer to the input matrix.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - The galois field to calculate on.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant_over_galois_field_laplace_expansion(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Calculates the determinant of a square matrix using Gauss-Jordan elimination.
 *
 * @param out_determinant - Pointer to the output determinant value.
 * @param matrix - Pointer to the input matrix.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - The Galois prime field.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE matrix_determinant_over_galois_field_gauss_jordan(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field);

#endif //MATRIXDETERMINANT_H
