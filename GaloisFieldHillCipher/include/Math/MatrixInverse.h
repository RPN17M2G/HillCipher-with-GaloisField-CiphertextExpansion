#ifndef MATRIXINVERSE_H
#define MATRIXINVERSE_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "Math/FieldBasicOperations.h"
#include "Math/MatrixDeterminant.h"

#define IS_ODD(x) ((x) % 2 != 0)
#define IS_EVEN(x) ((x) % 2 == 0)

/**
 * @brief Calculates the inverse of a square matrix using Gauss-Jordan elimination.
 *
 * @param out_inverse_matrix - Pointer to the output inverse matrix (allocated inside function).
 * @param matrix - Pointer to the input matrix.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - The Galois prime field.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE inverse_square_matrix_gauss_jordan(int64_t*** out_inverse_matrix, int64_t** matrix, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Calculates the inverse of a square matrix using adjugate method.
 *
 * @param out_inverse_matrix - Pointer to the output inverse matrix - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE inverse_square_matrix_adjugate_method(int64_t*** out_inverse_matrix, int64_t** matrix, uint32_t dimension, uint32_t prime_field);

#endif //MATRIXINVERSE_H
