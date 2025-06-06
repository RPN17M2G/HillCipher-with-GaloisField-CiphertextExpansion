#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileValidation.h"
#include "../StatusCodes.h"
#include "../Cipher/CipherUtils.h"

/**
 * @brief Write uint8_t vector to a file.
 *
 * @param filepath - The path to the file.
 * @param data - Pointer to the uint8_t vector to be written.
 * @param size - Size of the vector in bytes.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE write_uint8_to_file(const char* filepath, const uint8_t* data, uint32_t size);

/**
 * @brief Read uint8_t vector from a file.
 *
 * @param filepath - The path to the file.
 * @param out_data - Pointer to the uint8_t vector to be read to.
 * @param out_size - Size of the vector in bytes.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE read_uint8_from_file(uint8_t** out_data, uint32_t* out_size, const char* filepath);

/**
 * @brief Write int64_t vector to a file.
 *
 * @param filepath - The path to the file.
 * @param data - Pointer to the int64_t vector to be written.
 * @param size - Size of the vector in bytes.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE write_int64_to_file(const char* filepath, const int64_t* data, uint32_t size);

/**
 * @brief Read int64_t vector from a file.
 *
 * @param filepath - The path to the file.
 * @param out_data - Pointer to the int64_t vector to be read to.
 * @param out_size - Size of the vector in bytes.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE read_int64_from_file(int64_t** out_data, uint32_t* out_size, const char* filepath);
