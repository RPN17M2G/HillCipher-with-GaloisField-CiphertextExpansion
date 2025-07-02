
#ifndef ASCIIMAPPING_H
#define ASCIIMAPPING_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "StatusCodes.h"
#include "CSPRNG.h"

#define MAX_DIGIT (9)

/**
 * @brief Maps an ASCII char into the corresponding digit.
 *
 * @param out_digit - Pointer to the output digit (0-9).
 * @param input - Pointer to the input char.
 * @param digit_to_ascii - The digit-to-ASCII mapping matrix.
 * @param number_of_letters - Number of letters for each digit.
 * @return STATUS_CODE - Status of the operation.
*/
STATUS_CODE ascii_char_to_digit(uint8_t* out_digit, char input, uint8_t** digit_to_ascii, uint32_t number_of_letters);

/**
 * @brief Maps an int64_t vector into ASCII values for reducing entropy.
 *
 * @param out_ascii - Pointer to the output ASCII vector (allocated inside the function).
 * @param out_ascii_size - Pointer to the size of the ASCII vector in bytes.
 * @param data - Pointer to the input vector.
 * @param data_size - Size of the input vector in bytes.
 * @param digit_to_ascii - The digit-to-ASCII mapping matrix.
 * @param number_of_letters - Number of letters for each digit.
 * @param number_of_digits_per_field_element - Number of digits per field element.
 * @return STATUS_CODE - Status of the operation.
*/
STATUS_CODE map_from_int64_to_ascii(uint8_t** out_ascii, uint32_t* out_ascii_size, int64_t* data, uint32_t data_size, uint8_t** digit_to_ascii, uint32_t number_of_letters, uint32_t number_of_digits_per_field_element);

/**
 * @brief Maps an ASCII vector into int64_t values.
 *
 * @param out_int64 - Pointer to the output int64_t vector (allocated inside the function).
 * @param out_int64_size - Pointer to the size of the int64_t vector in bytes.
 * @param data - Pointer to the input vector.
 * @param data_size - Size of the input vector in bytes.
 * @param digit_to_ascii - The digit-to-ASCII mapping matrix.
 * @param number_of_letters - Number of letters for each digit.
 * @param number_of_digits_per_field_element - Number of digits per field element.
 * @return STATUS_CODE - Status of the operation.
*/
STATUS_CODE map_from_ascii_to_int64(int64_t** out_int64, uint32_t* out_int64_size, uint8_t* data, uint32_t data_size, uint8_t** digit_to_ascii, uint32_t number_of_letters, uint32_t number_of_digits_per_field_element);

#endif
