#ifndef CIPHER_UTILS_H
#define CIPHER_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CSPRNG.h"
#include "../StatusCodes.h"
#include "../Math/MathUtils.h"

#define DEFAULT_PRIME_GALOIS_FIELD (16777619)
#define BYTE_SIZE (8)
#define PADDING_MAGIC (0x17)

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
 * @param number_of_random_bits_to_add - Number of random bits to add between each byte of the input vector.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length, uint32_t number_of_random_bits_to_add);

/**
 * @brief Removes random bits between bytes of the input vector and reconstructs the original value.
 *
 * @param out - Pointer to the output vector with random bits removed - allocated inside the function and memory released if fails.
 * @param out_bit_size - Size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @param number_of_random_bits_to_remove - Number of random bits to remove between each byte of the input vector.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length, uint32_t number_of_random_bits_to_remove);

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
 * @brief Divides an int64_t vector into blocks of a specific size.
 *
 * @param out_blocks - Pointer to the output vector of blocks - allocated inside the function and memory released if fails.
 * @param num_blocks - Number of blocks in the output vector.
 * @param value - Pointer to the input vector.
 * @param value_bit_length - Length of the input vector in bits.
 * @param block_bit_size - Size of each block in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE divide_int64_t_into_blocks(int64_t*** out_blocks, uint32_t* num_blocks, int64_t* value, uint32_t value_bit_length, uint32_t block_bit_size);

#endif
