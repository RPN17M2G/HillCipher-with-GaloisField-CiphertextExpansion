#pragma once

#include "StatusCodes.h"
#include <stdbool.h>

/**
 * @brief Validates if the given file exists and is readable.
 *
 * @param path - The path to the file to be validated.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE validate_file_readable(const char* path);

/**
 * @brief Validates if the given file is readable binary.
 *
 * @param path - The path to the file to be validated.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE validate_file_is_binary(const char* path);

/**
 * @brief Validates if the given file exists, readable and binary.
 *
 * @param path - The path to the file to be validated.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE validate_file_readable_and_binary(const char* path);
