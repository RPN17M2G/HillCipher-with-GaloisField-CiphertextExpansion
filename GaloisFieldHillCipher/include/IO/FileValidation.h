#pragma once

#include "../StatusCodes.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BINARY_FILE_ENDING ".bin"

/**
 * @brief Validates if the given file exists and is readable.
 *
 * @param path - The path to the file to be validated.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE validate_file_is_readable(const char* path);

/**
 * @brief Validates if the given file is a readable binary.
 *
 * @param path - The path to the file to be validated.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE validate_file_is_binary(const char* path);
