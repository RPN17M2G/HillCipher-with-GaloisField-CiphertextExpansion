#ifndef BLOCK_DIVIDING_H
#define BLOCK_DIVIDING_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CSPRNG.h"
#include "StatusCodes.h"
#include "Math/MathUtils.h"

#define DEFAULT_PRIME_GALOIS_FIELD (16777619)
#define BYTE_SIZE (8)

/**
 * @brief Divides an uint8_t vector into blocks of a specific size.
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
