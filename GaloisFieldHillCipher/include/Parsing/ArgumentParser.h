#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <argparse.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "log.h"
#include "OutputFormat.h"
#include "Modes.h"
#include "StatusCodes.h"
#include "IO/FileValidation.h"

#define MAX_ERROR_MSG_LEN (256)
#define DECIMAL_BASE (10)
#define DEFAULT_VALUE_OF_NUMBER_OF_RANDOM_BITS_TO_ADD (2)

#define MODE_KEY_KEY_GENERATION_SHORT "ek"
#define MODE_KEY_DECRYPTION_KEY_GENERATION_SHORT "dk"
#define MODE_KEY_DECRYPT_SHORT "d"
#define MODE_KEY_ENCRYPT_SHORT "e"
#define MODE_KEY_GENERATE_AND_ENCRYPT_SHORT "ge"
#define MODE_KEY_GENERATE_AND_DECRYPT_SHORT "gd"

#define ARGUMENT_RANDOM_BITS_SHORT "r"
#define ARGUMENT_INPUT_SHORT "i"
#define ARGUMENT_OUTPUT_SHORT "o"
#define ARGUMENT_DIMENSION_SHORT "d"
#define ARGUMENT_VERBOSE_SHORT "v"
#define ARGUMENT_MODE_SHORT "m"
#define ARGUMENT_KEY_SHORT "k"
#define ARGUMENT_LOG_SHORT "l"


#define ARGUMENT_RANDOM_BITS_LONG "number_of_random_bits"
#define ARGUMENT_INPUT_LONG "input_file"
#define ARGUMENT_OUTPUT_LONG "output_file"
#define ARGUMENT_DIMENSION_LONG "dimension"
#define ARGUMENT_VERBOSE_LONG "verbose"
#define ARGUMENT_MODE_LONG "mode"
#define ARGUMENT_KEY_LONG "key"
#define ARGUMENT_LOG_LONG "log_file"

#define ARGUMENT_RANDOM_BITS_DOCUMENTATION "Number of random bits to add between bytes"
#define ARGUMENT_INPUT_DOCUMENTATION "Path to input file"
#define ARGUMENT_OUTPUT_DOCUMENTATION "Path to output file(Binary file for compact storage, text file for entropy hardened ciphertext)"
#define ARGUMENT_DIMENSION_DOCUMENTATION "Dimension (uint32_t)"
#define ARGUMENT_VERBOSE_DOCUMENTATION "Enable verbose output"
#define ARGUMENT_MODE_DOCUMENTATION "Cipher mode of operation(encrypt/decrypt/key generation)"
#define ARGUMENT_KEY_DOCUMENTATION "Path to key file"
#define ARGUMENT_LOG_DOCUMENTATION "Path to log file (must be a text file)"

#define USAGE_HEADER "[!] Invalid usage.\n[*] Modes:\n"
#define USAGE_RANDOM_BITS_FLAG "[*]   -" ARGUMENT_RANDOM_BITS_SHORT ", --" ARGUMENT_RANDOM_BITS_LONG "           : " ARGUMENT_RANDOM_BITS_DOCUMENTATION "\n"
#define USAGE_KEY_GENERATION_MODE "[*]   " MODE_KEY_KEY_GENERATION_SHORT " : Key generation. Requires -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_DECRYPTION_KEY_GENERATION_MODE "[*]   " MODE_KEY_DECRYPTION_KEY_GENERATION_SHORT " : Decryption key generation. Requires -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_DECRYPT_MODE "[*]   " MODE_KEY_DECRYPT_SHORT " : Decrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_ENCRYPT_MODE "[*]   " MODE_KEY_ENCRYPT_SHORT " : Encrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_KEY_GENERATE_AND_ENCRYPT_MODE "[*]   " MODE_KEY_GENERATE_AND_ENCRYPT_SHORT " : Generate key and encrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_KEY_GENERATE_AND_DECRYPT_MODE "[*]   " MODE_KEY_GENERATE_AND_DECRYPT_SHORT " : Generate key and decrypt. Requires -" ARGUMENT_INPUT_SHORT " <" ARGUMENT_INPUT_LONG "> -" ARGUMENT_OUTPUT_SHORT " <" ARGUMENT_OUTPUT_LONG "> -" ARGUMENT_KEY_SHORT " <" ARGUMENT_KEY_LONG "> -" ARGUMENT_DIMENSION_SHORT " <" ARGUMENT_DIMENSION_LONG "> [-" ARGUMENT_VERBOSE_SHORT "]\n"
#define USAGE_FLAGS_HEADER "\n[*] Flags:\n"
#define USAGE_INPUT_FLAG "[*]   -" ARGUMENT_INPUT_SHORT ", --" ARGUMENT_INPUT_LONG "       : " ARGUMENT_INPUT_DOCUMENTATION "\n"
#define USAGE_OUTPUT_FLAG "[*]   -" ARGUMENT_OUTPUT_SHORT ", --" ARGUMENT_OUTPUT_LONG "      : " ARGUMENT_OUTPUT_DOCUMENTATION "\n"
#define USAGE_DIMENSION_FLAG "[*]   -" ARGUMENT_DIMENSION_SHORT ", --" ARGUMENT_DIMENSION_LONG "        : " ARGUMENT_DIMENSION_DOCUMENTATION "\n"
#define USAGE_VERBOSE_FLAG "[*]   -" ARGUMENT_VERBOSE_SHORT ", --" ARGUMENT_VERBOSE_LONG "          : " ARGUMENT_VERBOSE_DOCUMENTATION "\n"
#define USAGE_MODE_FLAG "[*]   -" ARGUMENT_MODE_SHORT ", --" ARGUMENT_MODE_LONG "             : " ARGUMENT_MODE_DOCUMENTATION "\n"
#define USAGE_KEY_FLAG "[*]   -" ARGUMENT_KEY_SHORT ", --" ARGUMENT_KEY_LONG "              : " ARGUMENT_KEY_DOCUMENTATION "\n"
#define USAGE_LOG_FLAG "[*]   -" ARGUMENT_LOG_SHORT ", --" ARGUMENT_LOG_LONG "              : " ARGUMENT_LOG_DOCUMENTATION "\n"

#define USAGE (USAGE_HEADER \
    USAGE_KEY_GENERATION_MODE \
    USAGE_DECRYPTION_KEY_GENERATION_MODE \
    USAGE_DECRYPT_MODE \
    USAGE_ENCRYPT_MODE \
    USAGE_KEY_GENERATE_AND_ENCRYPT_MODE \
    USAGE_KEY_GENERATE_AND_DECRYPT_MODE \
    USAGE_FLAGS_HEADER \
    USAGE_INPUT_FLAG \
    USAGE_OUTPUT_FLAG \
    USAGE_DIMENSION_FLAG \
    USAGE_VERBOSE_FLAG \
    USAGE_MODE_FLAG \
    USAGE_KEY_FLAG \
    USAGE_RANDOM_BITS_FLAG \
    USAGE_LOG_FLAG)

typedef struct {
    const char* input_file;
    const char* output_file;
    const char* key;
    uint32_t dimension;
    bool verbose;
    OPERATION_MODE mode;
    FILE_FORMAT output_format;
    uint32_t number_of_random_bits_between_bytes;
    FILE_FORMAT input_format; // Not a main argument but is inferred from input file extension
    const char* log_file;     // Optional log file (must be text)
} ParsedArguments;

/**
 * @brief Parses command-line arguments into a structured format.
 *
 * @param out_args - Pointer to the structure where parsed arguments will be stored.
 * @param argc - The number of command-line arguments.
 * @param argv - The command-line arguments array.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE parse_arguments(ParsedArguments* out_args, int argc, char** argv);

/**
 * @brief Parses the mode string and outputs the corresponding OPERATION_MODE.
 *
 * @param out_mode Pointer to OPERATION_MODE to receive the result.
 * @param mode_string The mode string to parse.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_mode(OPERATION_MODE* out_mode, const char* mode_string);

/**
 * @brief Validates arguments for the selected mode.
 *
 * @param mode The operation mode.
 * @param input_file Path to input file.
 * @param output_file Path to output file.
 * @param key Path to key file.
 * @param dimension Matrix dimension.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE validate_mode_args(OPERATION_MODE mode, const char* input_file, const char* output_file, const char* key, uint32_t dimension);

/**
 * @brief Determines the file format (binary or text) for a given file.
 *
 * @param out_format Pointer to FILE_FORMAT to receive the result.
 * @param file Path to the file.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE get_file_format(FILE_FORMAT* out_format, const char* file);

#endif