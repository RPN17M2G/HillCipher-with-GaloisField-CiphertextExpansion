#ifndef MATRIXMULTIPLICATION_H
#define MATRIXMULTIPLICATION_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "FieldBasicOperations.h"
#include "log.h"

#define MEMORY_BUFFER_FOR_PLAINTEXT_BLOCK (3)

/**
 * @brief Multiplies a square matrix with a vector.
 *
 * @param out_vector - Pointer to the output vector - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param vector - Pointer to the input vector.
 * @param dimension - Dimension of the square matrix and vector.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE multiply_matrix_with_uint8_t_vector(int64_t** out_vector, int64_t** matrix, uint8_t* vector, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Multiplies a square matrix with a vector for decryption, the result vector is uint8_t.
 *
 * @param out_vector - Pointer to the output vector - allocated inside the function and memory released if fails.
 * @param matrix - Pointer to the input matrix.
 * @param vector - Pointer to the input vector.
 * @param dimension - Dimension of the square matrix and vector.
 * @param prime_field - Prime field to use for calculations.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE multiply_matrix_with_int64_t_vector(uint8_t** out_vector, int64_t** matrix, int64_t* vector, uint32_t dimension, uint32_t prime_field);

#endif //MATRIXMULTIPLICATION_H
