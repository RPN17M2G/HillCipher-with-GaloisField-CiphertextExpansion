#pragma once

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <stdio.h>

#include "StatusCodes.h"

#define DEFAULT_PRIME_GALOIS_FIELD 16777619
#define BYTE_SIZE 8
#define TWO_BITS_MASK 0x03
#define NUMBER_OF_RANDOM_BITS_TO_ADD 2

#define IS_BIT_SET(byte, bit_number) ((byte & (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)))) != 0)
#define SET_BIT(byte, bit_number) (byte |= (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))
#define CLEAR_BIT(byte, bit_number) (byte &= ~(1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))

/**
 * @brief Adds random bits between bytes of the input vector.
 *
 * @param out - Pointer to the output vector with random bits added - allocated inside the function and memory released if fails.
 * @param out_size - Size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_length - Length of the input vector in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length);

/**
 * @brief Removes random bits between bytes of the input vector and reconstructs the original value.
 *
 * @param out - Pointer to the output vector with random bits removed - allocated inside the function and memory released if fails.
 * @param out_size - Size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_length - Length of the input vector in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length);

/**
 * @brief Pads a uint8_t vector with 0x00 bytes to fit a specific length.
 *
 * @param out - Pointer to the output vector with padding added - allocated inside the function and memory released if fails.
 * @param out_size - Size of the output vector in bits.
 * @param value - Pointer to the input vector.
 * @param value_length - Length of the input vector in bits.
 * @param target_length - Target length of the output vector in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE pad_to_length(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length, uint32_t target_length);

/**
 * @brief Divides a uint8_t vector into blocks of a specific size.
 *
 * @param out_blocks - Pointer to the output vector of blocks - allocated inside the function and memory released if fails.
 * @param num_blocks - Number of blocks in the output vector.
 * @param value - Pointer to the input vector.
 * @param value_length - Length of the input vector in bits.
 * @param block_size - Size of each block in bits.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE divide_into_blocks(uint8_t*** out_blocks, uint32_t* num_blocks, uint8_t* value, uint32_t value_length, uint32_t block_size);

