#ifndef CIPHERTEXTEXPANSION_H
#define CIPHERTEXTEXPANSION_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CSPRNG.h"
#include "StatusCodes.h"
#include "Math/MathUtils.h"

#define IS_BIT_SET(byte, bit_number) ((byte & (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)))) != 0)
#define SET_BIT(byte, bit_number) (byte |= (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))
#define CLEAR_BIT(byte, bit_number) (byte &= ~(1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))
#define BYTE_SIZE (8)

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


#endif
