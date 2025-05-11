#pragma once

#include <argparse.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Modes.h"
#include "StatusCodes.h"

#define MAX_ERROR_MSG_LEN (256)
#define DECIMAL_BASE (10)

#define MODE_KEY_KEY_GENERATION_SHORT "ek"
#define MODE_KEY_DECRYPTION_KEY_GENERATION_SHORT "dk"
#define MODE_KEY_DECRYPT_SHORT "d"
#define MODE_KEY_ENCRYPT_SHORT "e"
#define MODE_KEY_GENERATE_AND_ENCRYPT_SHORT "ge"
#define MODE_KEY_GENERATE_AND_DECRYPT_SHORT "gd"

#define ARGUMENT_INPUT_SHORT "i"
#define ARGUMENT_OUTPUT_SHORT "o"
#define ARGUMENT_DIMENSION_SHORT "d"
#define ARGUMENT_VERBOSE_SHORT "v"
#define ARGUMENT_MODE_SHORT "m"
#define ARGUMENT_KEY_SHORT "k"

#define ARGUMENT_INPUT_LONG "input_file"
#define ARGUMENT_OUTPUT_LONG "output_file"
#define ARGUMENT_DIMENSION_LONG "dimension"
#define ARGUMENT_VERBOSE_LONG "verbose"
#define ARGUMENT_MODE_LONG "mode"
#define ARGUMENT_KEY_LONG "key"

#define ARGUMENT_INPUT_DOCUMENTATION "Path to input file"
#define ARGUMENT_OUTPUT_DOCUMENTATION "Path to output file"
#define ARGUMENT_DIMENSION_DOCUMENTATION "Dimension (uint32_t)"
#define ARGUMENT_VERBOSE_DOCUMENTATION "Enable verbose output"
#define ARGUMENT_MODE_DOCUMENTATION "Cipher mode of operation(encrypt/decrypt/key generation)"
#define ARGUMENT_KEY_DOCUMENTATION "Path to key file"

#define USAGE_HEADER "[!] Invalid usage.\n[*] Modes:\n"
#define USAGE_KEY_GENERATION_MODE "[*]   " MODE_KEY_KEY_GENERATION_SHORT " : Key generation. Requires -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_DECRYPTION_KEY_GENERATION_MODE "[*]   " MODE_KEY_DECRYPTION_KEY_GENERATION_SHORT " : Decryption key generation. Requires -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_DECRYPT_MODE "[*]   " MODE_KEY_DECRYPT_SHORT " : Decrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_ENCRYPT_MODE "[*]   " MODE_KEY_ENCRYPT_SHORT " : Encrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_KEY_GENERATE_AND_ENCRYPT_MODE "[*]   " MODE_KEY_GENERATE_AND_ENCRYPT_SHORT " : Generate key and encrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_KEY_GENERATE_AND_DECRYPT_MODE "[*]   " MODE_KEY_GENERATE_AND_DECRYPT_SHORT " : Generate key and decrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_FLAGS_HEADER "\n[*] Flags:\n"
#define USAGE_INPUT_FLAG "[*]   -" ARGUMENT_INPUT_SHORT ", --" ARGUMENT_INPUT_LONG "       : " ARGUMENT_INPUT_DOCUMENTATION "\n"
#define USAGE_OUTPUT_FLAG "[*]   -" ARGUMENT_OUTPUT_SHORT ", --" ARGUMENT_OUTPUT_LONG "      : " ARGUMENT_OUTPUT_DOCUMENTATION "\n"
#define USAGE_DIMENSION_FLAG "[*]   -" ARGUMENT_DIMENSION_SHORT ", --" ARGUMENT_DIMENSION_LONG "        : " ARGUMENT_DIMENSION_DOCUMENTATION "\n"
#define USAGE_VERBOSE_FLAG "[*]   -" ARGUMENT_VERBOSE_SHORT ", --" ARGUMENT_VERBOSE_LONG "          : " ARGUMENT_VERBOSE_DOCUMENTATION "\n"
#define USAGE_MODE_FLAG "[*]   -" ARGUMENT_MODE_SHORT ", --" ARGUMENT_MODE_LONG "             : " ARGUMENT_MODE_DOCUMENTATION "\n"
#define USAGE_KEY_FLAG "[*]   -" ARGUMENT_KEY_SHORT ", --" ARGUMENT_KEY_LONG "              : " ARGUMENT_KEY_DOCUMENTATION "\n"

#define USAGE (USAGE_HEADER USAGE_KEY_GENERATION_MODE USAGE_DECRYPTION_KEY_GENERATION_MODE USAGE_DECRYPT_MODE USAGE_ENCRYPT_MODE USAGE_KEY_GENERATE_AND_ENCRYPT_MODE USAGE_KEY_GENERATE_AND_DECRYPT_MODE USAGE_FLAGS_HEADER USAGE_INPUT_FLAG USAGE_OUTPUT_FLAG USAGE_DIMENSION_FLAG USAGE_VERBOSE_FLAG USAGE_MODE_FLAG USAGE_KEY_FLAG)

typedef struct {
    const char* input_file;
    const char* output_file;
    const char* key;
    uint32_t dimension;
    bool verbose;
    OPERATION_MODE mode;
} ParsedArguments;


/**
 * @brief Creates an output file if it doesn't already exist.
 *
 * @param path - The path to the output file to be created.
 * @return STATUS_CODE - Status of the operation. 
 */
STATUS_CODE create_output_file_if_needed(const char* path);

/**
 * @brief Validates if the given file is readable.
 *
 * @param path - The path to the file to be validated.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE validate_readable_file(const char* path);

/**
 * @brief Extracts command-line arguments into a structured format.
 *
 * @param argc - The number of command-line arguments.
 * @param argv - The command-line arguments array.
 * @param out_args - Pointer to the structure where parsed arguments will be stored.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE extract_arguments(ParsedArguments* out_args, int argc, char** argv);

/**
 * @brief Parses command-line arguments into a structured format.
 *
 * @param argc - The number of command-line arguments.
 * @param argv - The command-line arguments array.
 * @param out_args - Pointer to the structure where parsed arguments will be stored.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE parse_arguments(ParsedArguments* out_args, int argc, char** argv);
