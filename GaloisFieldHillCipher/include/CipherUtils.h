#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CSPRNG.h"
#include "StatusCodes.h"
#include "MathUtils.h"

#define DEFAULT_PRIME_GALOIS_FIELD (16777619)
#define BYTE_SIZE (8)
#define TWO_BITS_MASK (0x03)
#define NUMBER_OF_RANDOM_BITS_TO_ADD (2)
#define PADDING_MAGIC (0x17)
#define TWO_BITS_MAX_VALUE (3)

#define IS_BIT_SET(byte, bit_number) ((byte & (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)))) != 0)
#define SET_BIT(byte, bit_number) (byte |= (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))
#define CLEAR_BIT(byte, bit_number) (byte &= ~(1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))

/**
 * @brief Adds random bits between bytes of the input vector.
 *
 * @param out - Pointer to the output vector with random bits added - allocated inside the function and memory released if fails.
 * @param out_bit_size - Size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length);

/**
 * @brief Removes random bits between bytes of the input vector and reconstructs the original value.
 *
 * @param out - Pointer to the output vector with random bits removed - allocated inside the function and memory released if fails.
 * @param out_bit_size - Size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length);

/**
 * @brief Pads a uint8_t vector with 0x00 bytes to fit a specific length.
 *
 * @param out - Pointer to the output vector with padding added - allocated inside the function and memory released if fails.
 * @param out_bit_length - Pointer to the size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @param target_bit_length - Target length of the output vector in bits.
 * @param block_bit_size - Size of block in bits for extra padding if lengths are equal
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE pad_to_length(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length, uint32_t target_bit_length, uint32_t block_bit_size);

/**
 * @brief Removes padding from uint8_t vector
 *
 * @param out - Pointer to the output vector without padding - allocated inside the function and memory released if fails.
 * @param out_bit_length - Pointer to the size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE remove_padding(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length);


/**
 * @brief Divides a uint8_t vector into blocks of a specific size.
 *
 * @param out_blocks - Pointer to the output vector of blocks - allocated inside the function and memory released if fails.
 * @param num_blocks - Number of blocks in the output vector.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @param block_bit_size - Size of each block in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE divide_uint8_t_into_blocks(uint8_t*** out_blocks, uint32_t* num_blocks, uint8_t* value, uint32_t value_bit_length, uint32_t block_bit_size);

/**
 * @brief Divides a double vector into blocks of a specific size.
 *
 * @param out_blocks - Pointer to the output vector of blocks - allocated inside the function and memory released if fails.
 * @param num_blocks - Number of blocks in the output vector.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @param block_bit_size - Size of each block in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE divide_double_into_blocks(long double*** out_blocks, uint32_t* num_blocks, long double* value, uint32_t value_bit_length, uint32_t block_bit_size);


/**
 * @brief Generates an encryption matrix with cryptography secure random values.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param dimentation - Dimension of the square matrix.
 * @param prime_field - Prime field to use for generating random values.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_encryption_matrix(long double*** out_matrix, uint32_t dimentation, uint32_t prime_field);

/**
 * @brief Generates a decryption matrix from encryption matrix.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param dimentation - Dimension of the square matrix.
 * @param encryption_matrix - The encryption matrix
 * @param prime_field - Prime field to use for generating random values.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_decryption_matrix(long double*** out_matrix, uint32_t dimentation, long double** encryption_matrix, uint32_t prime_field);
