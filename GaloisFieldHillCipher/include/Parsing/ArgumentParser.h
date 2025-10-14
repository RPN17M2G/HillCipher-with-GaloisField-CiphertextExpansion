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
#include "Modes.h"
#include "StatusCodes.h"
#include "IO/FileValidation.h"
#include "Parsing/ModeParsers.h"

#define MAX_ERROR_MSG_LEN (256)
#define DECIMAL_BASE (10)
#define DEFAULT_VALUE_OF_NUMBER_OF_RANDOM_BITS_TO_ADD (2)
#define DEFAULT_VALUE_OF_NUMBER_OF_ERROR_VECTORS_TO_ADD (5)
#define DEFAULT_VALUE_OF_NUMBER_OF_ASCII_CHARACTERS_MAPPED_TO_EACH_DIGIT (5)
#define DEFAULT_VALUE_OF_GALOIS_FIELD (16777619)
#define NUMBER_OF_FLAGS_FOR_EACH_OPTION (2)
#define MEMORY_FOR_FLAG_PREFIX (3)

#define MODE_KEY_GENERATION "kg"
#define MODE_DECRYPTION_KEY_GENERATION "dkg"
#define MODE_ENCRYPT "e"
#define MODE_DECRYPT "d"
#define MODE_GENERATE_AND_ENCRYPT "kge"
#define MODE_GENERATE_AND_DECRYPT "kgd"

#define FLAG_INPUT_FILE "input"
#define FLAG_INPUT_FILE_SHORT "i"
#define FLAG_INPUT_FILE_TYPE "<FILE>"
#define FLAG_INPUT_FILE_DESCRIPTION "Specify the input file (required for encrypt, decrypt, generate_and_encrypt, and generate_and_decrypt modes)."

#define FLAG_OUTPUT_FILE "output"
#define FLAG_OUTPUT_FILE_SHORT "o"
#define FLAG_OUTPUT_FILE_TYPE "<FILE>"
#define FLAG_OUTPUT_FILE_DESCRIPTION "Specify the output file (required for all modes)."

#define FLAG_KEY_FILE "key"
#define FLAG_KEY_FILE_SHORT "k"
#define FLAG_KEY_FILE_TYPE "<FILE>"
#define FLAG_KEY_FILE_DESCRIPTION "Specify the key file (required for encrypt, decrypt, decryption_key_generation, and generate_and_encrypt modes)."

#define FLAG_LOG_FILE "log"
#define FLAG_LOG_FILE_SHORT "l"
#define FLAG_LOG_FILE_TYPE "<FILE>"
#define FLAG_LOG_FILE_DESCRIPTION "Specify the log file (optional)."

#define FLAG_DIMENSION "dimension"
#define FLAG_DIMENSION_SHORT "d"
#define FLAG_DIMENSION_TYPE "<NUMBER>"
#define FLAG_DIMENSION_DESCRIPTION "Specify the matrix dimension (required for key_generation and generate_and_encrypt modes)."

#define FLAG_RANDOM_BITS "random-bits"
#define FLAG_RANDOM_BITS_SHORT "r"
#define FLAG_RANDOM_BITS_TYPE "<NUMBER>"
#define FLAG_RANDOM_BITS_DESCRIPTION "Specify the number of random bits to add between bytes (optional, default: 2)."

#define FLAG_ERROR_VECTORS "error-vectors"
#define FLAG_ERROR_VECTORS_SHORT "e"
#define FLAG_ERROR_VECTORS_TYPE "<NUMBER>"
#define FLAG_ERROR_VECTORS_DESCRIPTION "Specify the number of error vectors (optional)."

#define FLAG_VERBOSE "verbose"
#define FLAG_VERBOSE_SHORT "v"
#define FLAG_VERBOSE_TYPE ""
#define FLAG_VERBOSE_DESCRIPTION "Enable verbose output (optional)."

#define FLAG_MODE "mode"
#define FLAG_MODE_SHORT "m"
#define FLAG_MODE_TYPE "<MODE>"
#define FLAG_MODE_DESCRIPTION "Specify the operation mode. Available modes: " \
    MODE_KEY_GENERATION " (key generation), " \
    MODE_DECRYPTION_KEY_GENERATION " (decryption key generation), " \
    MODE_ENCRYPT " (encrypt), " \
    MODE_DECRYPT " (decrypt), " \
    MODE_GENERATE_AND_ENCRYPT " (generate and encrypt), " \
    MODE_GENERATE_AND_DECRYPT " (generate and decrypt)."

#define FLAG_PRIME_FIELD "prime-field"
#define FLAG_PRIME_FIELD_SHORT "f"
#define FLAG_PRIME_FIELD_TYPE "<NUMBER>"
#define FLAG_PRIME_FIELD_DESCRIPTION "Specify the prime field (optional, default: 16777619)."

#define FLAG_ASCII_MAPPING_LETTERS "ascii-mapping-letters"
#define FLAG_ASCII_MAPPING_LETTERS_SHORT "a"
#define FLAG_ASCII_MAPPING_LETTERS_TYPE "<NUMBER>"
#define FLAG_ASCII_MAPPING_LETTERS_DESCRIPTION "Specify the number of letters for ASCII mapping (optional)."

#define FLAG_DECRYPTION_KEY_OUTPUT_FILE "decryption-key-output"
#define FLAG_DECRYPTION_KEY_OUTPUT_FILE_SHORT "y"
#define FLAG_DECRYPTION_KEY_OUTPUT_FILE_TYPE "<FILE>"
#define FLAG_DECRYPTION_KEY_OUTPUT_FILE_DESCRIPTION "Specify the decryption key output file (required for generate_and_decrypt mode)."

#define USAGE_STRING \
"Usage: GaloisFieldHillCipher [OPTIONS]\n" \
"\n" \
"Modes:\n" \
"  --" FLAG_MODE ", -" FLAG_MODE_SHORT " " FLAG_MODE_TYPE "               " FLAG_MODE_DESCRIPTION "\n" \
"    Available modes:\n" \
"      " MODE_KEY_GENERATION " - Key generation\n" \
"      " MODE_DECRYPTION_KEY_GENERATION " - Decryption key generation\n" \
"      " MODE_ENCRYPT " - Encrypt\n" \
"      " MODE_DECRYPT " - Decrypt\n" \
"      " MODE_GENERATE_AND_ENCRYPT " - Generate and encrypt\n" \
"      " MODE_GENERATE_AND_DECRYPT " - Generate and decrypt\n" \
"\n" \
"General Options:\n" \
"  --" FLAG_INPUT_FILE ", -" FLAG_INPUT_FILE_SHORT " " FLAG_INPUT_FILE_TYPE "          " FLAG_INPUT_FILE_DESCRIPTION "\n" \
"  --" FLAG_OUTPUT_FILE ", -" FLAG_OUTPUT_FILE_SHORT " " FLAG_OUTPUT_FILE_TYPE "         " FLAG_OUTPUT_FILE_DESCRIPTION "\n" \
"  --" FLAG_KEY_FILE ", -" FLAG_KEY_FILE_SHORT " " FLAG_KEY_FILE_TYPE "            " FLAG_KEY_FILE_DESCRIPTION "\n" \
"  --" FLAG_LOG_FILE ", -" FLAG_LOG_FILE_SHORT " " FLAG_LOG_FILE_TYPE "            " FLAG_LOG_FILE_DESCRIPTION "\n" \
"  --" FLAG_DIMENSION ", -" FLAG_DIMENSION_SHORT " " FLAG_DIMENSION_TYPE "        " FLAG_DIMENSION_DESCRIPTION "\n" \
"  --" FLAG_RANDOM_BITS ", -" FLAG_RANDOM_BITS_SHORT " " FLAG_RANDOM_BITS_TYPE "      " FLAG_RANDOM_BITS_DESCRIPTION "\n" \
"  --" FLAG_ERROR_VECTORS ", -" FLAG_ERROR_VECTORS_SHORT " " FLAG_ERROR_VECTORS_TYPE "    " FLAG_ERROR_VECTORS_DESCRIPTION "\n" \
"  --" FLAG_PRIME_FIELD ", -" FLAG_PRIME_FIELD_SHORT " " FLAG_PRIME_FIELD_TYPE "      " FLAG_PRIME_FIELD_DESCRIPTION "\n" \
"  --" FLAG_ASCII_MAPPING_LETTERS ", -" FLAG_ASCII_MAPPING_LETTERS_SHORT " " FLAG_ASCII_MAPPING_LETTERS_TYPE " " FLAG_ASCII_MAPPING_LETTERS_DESCRIPTION "\n" \
"  --" FLAG_DECRYPTION_KEY_OUTPUT_FILE ", -" FLAG_DECRYPTION_KEY_OUTPUT_FILE_SHORT " " FLAG_DECRYPTION_KEY_OUTPUT_FILE_TYPE " " FLAG_DECRYPTION_KEY_OUTPUT_FILE_DESCRIPTION "\n" \
"  --" FLAG_VERBOSE ", -" FLAG_VERBOSE_SHORT "                   " FLAG_VERBOSE_DESCRIPTION "\n" \
"\n" \
"Examples:\n" \
"  GaloisFieldHillCipher --" FLAG_MODE " " MODE_KEY_GENERATION " --" FLAG_OUTPUT_FILE " key.txt --" FLAG_DIMENSION " 4\n" \
"  GaloisFieldHillCipher --" FLAG_MODE " " MODE_ENCRYPT " --" FLAG_INPUT_FILE " plaintext.txt --" FLAG_OUTPUT_FILE " ciphertext.txt\n" \
"             --" FLAG_KEY_FILE " key.txt\n" \
"  GaloisFieldHillCipher --" FLAG_MODE " " MODE_DECRYPT " --" FLAG_INPUT_FILE " ciphertext.txt --" FLAG_OUTPUT_FILE " plaintext.txt\n" \
"             --" FLAG_KEY_FILE " key.txt\n" \
"  GaloisFieldHillCipher --" FLAG_MODE " " MODE_GENERATE_AND_ENCRYPT " --" FLAG_INPUT_FILE " plaintext.txt\n" \
"             --" FLAG_OUTPUT_FILE " ciphertext.txt --" FLAG_KEY_FILE " key.txt --" FLAG_DIMENSION " 4\n" \
"  GaloisFieldHillCipher --" FLAG_MODE " " MODE_GENERATE_AND_DECRYPT " --" FLAG_INPUT_FILE " ciphertext.txt\n" \
"             --" FLAG_OUTPUT_FILE " plaintext.txt --" FLAG_KEY_FILE " encryption_key.txt --" FLAG_DECRYPTION_KEY_OUTPUT_FILE " decryption_key.txt\n" \
"\n" \
"Notes:\n" \
"  - The input and output files must be readable and writable, respectively.\n" \
"  - The key file must be a valid binary file.\n" \
"  - The log file must be a text file.\n"

typedef struct GlobalArguments {
    bool verbose;
    const char* log_file;
} GlobalArguments;

/**
 * @brief Parses the mode of operation from command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @param out_mode Pointer to store the parsed mode.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_mode(OPERATION_MODE* out_mode, int argc, char** argv);

/**
 * @brief Parses verbose from command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @param out_arguments Pointer to store the parsed verbose flag.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_global_arguments(GlobalArguments** out_arguments, int argc, char** argv);

/**
 * @brief Parses arguments for the specific mode.
 *
 * @param mode The operation mode.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @param out_mode_arguments Pointer to store the mode-specific arguments' struct.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_mode_arguments(void** out_mode_arguments, OPERATION_MODE mode, int argc, char** argv);

/**
 * @brief Parses command-line options using argparse.
 *
 * @param options Array of argparse options.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_generic_options(struct argparse_option* options, int argc, char** argv);

/**
 * @brief Filters argv to include only relevant flags for parsing.
 *
 * @param out_filtered_argv Pointer to store the filtered argv array.
 * @param out_filtered_argc Pointer to store the count of filtered arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @param relevant_flags Array of relevant flags to include (NULL-terminated).
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE filter_relevant_flags(char*** out_filtered_argv, int* out_filtered_argc, int argc, char** argv, const char** relevant_flags);

/**
 * @brief Extracts relevant flags from the argparse options.
 *
 * @param out_relevant_flags Pointer to store the extracted relevant flags (NULL-terminated array).
 * @param options Array of argparse options.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE extract_relevant_flags(const char*** out_relevant_flags, struct argparse_option* options);

#endif
