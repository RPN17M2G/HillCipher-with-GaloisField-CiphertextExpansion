#ifndef MATRIX_SERDES_H
#define MATRIX_SERDES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "StatusCodes.h"
#include "Math/MatrixUtils.h"
#include "Cipher/CipherParts/BlockDividing.h"
#include "log.h"
#include "Cipher/Cipher.h"
#include "Secrets.h"

#define NUMBER_OF_DIGITS (10)
#define BYTE_MASK (0xFF)

/**
 * @brief Calculate the number of bytes per element on the prime field.
 *
 * @param prime_field - The prime field used.
 * @return The number of bytes per element based on the prime field.
 */
uint32_t calculate_bytes_per_element(uint32_t prime_field);

/**
 * @brief Calculate the number of digits per element on the prime field.
 *
 * @param prime_field - The prime field used.
 * @return The number of digits per element based on the prime field.
 */
uint32_t calculate_digits_per_element(uint32_t prime_field);

/**
 * @brief Serialize secrets to binary.
 *
 * @param out_data - A pointer to an output vector.
 * @param out_size - A pointer to the size of the output vector.
 * @param secrets - The secrets to be serialized.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE serialize_secrets(uint8_t** out_data, uint32_t* out_size, Secrets secrets);

/**
 * @brief Deserialize secrets from binary.
 *
 * @param out_secrets - A pointer to an output Secrets.
 * @param data - The data to be deserialized.
 * @param size - The size of the data.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_secrets(Secrets* out_secrets, uint8_t* data, uint32_t size);

/**
 * @brief Serialize matrix to binary.
 *
 * @param out_data - A pointer to an output vector.
 * @param out_size - A pointer to the size of the output vector.
 * @param matrix - The matrix to be serialized.
 * @param rows - The number of rows in the matrix.
 * @param columns - The number of columns in the matrix.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t rows, uint32_t columns, uint32_t prime_field);

/**
 * @brief Deserialize matrix from binary.
 *
 * @param out_matrix - A pointer to an output matrix.
 * @param rows - The number of rows in the matrix.
 * @param columns - The number of columns in the matrix.
 * @param data - The data to be deserialized.
 * @param size - The size of the data.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t rows, uint32_t columns, const uint8_t* data, uint32_t size, uint32_t prime_field);

/**
 * @brief Serialize square matrix to binary.
 *
 * @param out_data - A pointer to an output vector.
 * @param out_size - A pointer to the size of the output vector.
 * @param matrix - The matrix to be serialized.
 * @param dimension - The dimension of the matrix.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE serialize_square_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Deserialize square matrix from binary.
 *
 * @param out_matrix - A pointer to an output matrix.
 * @param dimension - The dimension of the matrix.
 * @param data - The data to be deserialized.
 * @param size - The size of the data.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_square_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size, uint32_t prime_field);

/**
 * @brief Serialize vector to binary.
 *
 * @param out_data - A pointer to an output vector.
 * @param out_size - A pointer to the size of the output vector.
 * @param vector - The vector to be serialized.
 * @param size - The vector size.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE serialize_vector(uint8_t** out_data, uint32_t* out_size, int64_t* vector, uint32_t size, uint32_t prime_field);

/**
 * @brief Deserialize vector from binary.
 *
 * @param out_vector - A pointer to an output vector.
 * @param out_size - A pointer to the output vector size.
 * @param data - The data to be deserialized.
 * @param data_size - The size of the input data.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_vector(int64_t** out_vector, uint32_t* out_size, const uint8_t* data, uint32_t data_size, uint32_t prime_field);

/**
 * @brief Serialize a uint8_t matrix to binary.
 *
 * @param out_data - A pointer to an output vector.
 * @param out_size - A pointer to the size of the output vector.
 * @param matrix - The matrix to be serialized.
 * @param rows - The number of rows in the matrix.
 * @param columns - The number of columns in the matrix.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE serialize_uint8_matrix(uint8_t** out_data, uint32_t* out_size, uint8_t** matrix, uint32_t rows, uint32_t columns, uint32_t prime_field);

/**
 * @brief Deserialize a uint8_t matrix from binary.
 *
 * @param out_matrix - A pointer to an output matrix.
 * @param rows - The number of rows in the matrix.
 * @param columns - The number of columns in the matrix.
 * @param data - The data to be deserialized.
 * @param size - The size of the data.
 * @param prime_field - The prime field used to calculate bytes per element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE deserialize_uint8_matrix(uint8_t*** out_matrix, uint32_t rows, uint32_t columns, const uint8_t* data, uint32_t size, uint32_t prime_field);

/**
 * @brief Copy a uint8_t matrix into an int64_t matrix.
 *
 * @param out_matrix - A pointer to an output int64_t matrix.
 * @param matrix - The uint8_t matrix to be copied.
 * @param rows - The number of rows in the matrix.
 * @param columns - The number of columns in the matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE copy_uint8_matrix_to_int64_matrix(int64_t*** out_matrix, uint8_t** matrix, uint32_t rows, uint32_t columns);

#endif
