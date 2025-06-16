#ifndef ASCIIMAPPING_H
#define ASCIIMAPPING_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "StatusCodes.h"
#include "CSPRNG.h"

#define DIGITS_PER_NUMBER (8)
#define DIGIT_VARIANTS (5)
#define MAX_DIGIT (9)

static const char* digit_to_ascii[] =
{
  "zIMNP", // 0
  "yRFut", // 1
  "sLUOm", // 2
  "kWnwJ", // 3
  "lqEHx", // 4
  "voQcG", // 5
  "XbVBD", // 6
  "KpeAa", // 7
  "gfrCd", // 8
  "jhSZY"  // 9
};

/**
 * @brief Maps an ASCII char into the corresponding digit.
 *
 * @param out_digit - Pointer to the output digit (0-9).
 * @param input - Pointer to the input char.
 * @return STATUS_CODE - Status of the operation.
*/
STATUS_CODE ascii_char_to_digit(uint8_t* out_digit, char input);

/**
 * @brief Maps an int64_t vector into ASCII values for reducing entropy.
 *
 * @param out_ascii - Pointer to the output ascii vector (allocated inside the function).
 * @param out_ascii_size - Pointer to the size of the ascii vector in bytes.
 * @param data - Pointer to the input vector.
 * @param data_size - Size of the input vector in bytes.
 * @return STATUS_CODE - Status of the operation.
*/
STATUS_CODE map_from_int64_to_ascii(uint8_t** out_ascii, uint32_t* out_ascii_size, int64_t* data, uint32_t data_size);

/**
 * @brief Maps an ASCII vector into int64_t values.
 *
 * @param out_int64 - Pointer to the output int64_t vector (allocated inside the function).
 * @param out_int64_size - Pointer to the size of the int64_t vector in bytes.
 * @param data - Pointer to the input vector.
 * @param data_size - Size of the input vector in bytes.
 * @return STATUS_CODE - Status of the operation.
*/
STATUS_CODE map_from_ascii_to_int64(int64_t** out_int64, uint32_t* out_int64_size, uint8_t* data, uint32_t data_size);

#endif
