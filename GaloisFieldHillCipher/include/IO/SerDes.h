#ifndef MATRIX_SERDES_H
#define MATRIX_SERDES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StatusCodes.h"
#include "Math/MatrixUtils.h"
#include "Cipher/CipherUtils.h"

#define NUMBER_OF_BYTES_PER_ELEMENT (3)

/**
 * @brief Serialize matrix to binary.
 *
 * @param out_data - A pointer to an output vector.
 * @param out_size - A pointer to the size of the output vector.
 * @param matrix - The matrix to be serialized.
 * @param dimension - The dimension of the matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension);

/**
 * @brief Deserialize matrix from binary.
 *
 * @param out_matrix - A pointer to an output matrix.
 * @param dimension - The dimension of the matrix.
 * @param data - The data to be deserialized.
 * @param size - The size of the data.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size);

/**
 * @brief Serialize vector to binary.
 *
 * @param out_data - A pointer to an output vector.
 * @param out_size - A pointer to the size of the output vector.
 * @param vector - The vector to be serialized.
 * @param size - The vector size.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE serialize_vector(uint8_t** out_data, uint32_t* out_size, int64_t* vector, uint32_t size);

/**
 * @brief Deserialize vector from binary.
 *
 * @param out_vector - A pointer to an output vector.
 * @param out_size - A pointer to the output vector size.
 * @param data - The data to be deserialized.
 * @param data_size - The size of the input data.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_vector(int64_t** out_vector, uint32_t* out_size, const uint8_t* data, uint32_t data_size);

#endif
