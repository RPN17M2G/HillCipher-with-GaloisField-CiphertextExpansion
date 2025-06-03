#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../StatusCodes.h"
#include "../Math/MatrixUtils.h"

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
 * @brief Deserialize matrix to binary.
 *
 * @param out_matrix - A pointer to an output matrix.
 * @param dimension - The dimension of the matrix.
 * @param data - The data to be deserialized.
 * @param size - The size of the data.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size);

