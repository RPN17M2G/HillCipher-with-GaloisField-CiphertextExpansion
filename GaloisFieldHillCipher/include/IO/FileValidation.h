#ifndef FILE_VALIDATION_H
#define FILE_VALIDATION_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "StatusCodes.h"
#include "OutputFormat.h"

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

/**
 * @brief Validates if the given output file is writable and of the correct extension.
 *
 * @param path - The path to the output file to be validated.
 * @param format - The output format to be used for validation.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE validate_output_file(const char* path, FILE_FORMAT format);

#endif
