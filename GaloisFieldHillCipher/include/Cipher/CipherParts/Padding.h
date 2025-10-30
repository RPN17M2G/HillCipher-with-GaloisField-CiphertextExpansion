#ifndef PADDING_H
#define PADDING_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "Math/MathUtils.h"
#include "log.h"

#define PADDING_MAGIC (0x17)
#define BYTE_SIZE (8)

/**
 * @brief Pads an uint8_t vector with 0x00 bytes to fit a specific length.
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

#endif
