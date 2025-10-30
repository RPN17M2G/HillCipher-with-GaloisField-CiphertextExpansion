#ifndef MODEPARSERS_H
#define MODEPARSERS_H

#include "Parsing/ArgumentParser.h"
#include "log.h"


#define USAGE_KEY_GENERATION_MODE \
    "Usage for key generation mode:\n" \
    "  --" FLAG_OUTPUT_FILE ", -" FLAG_OUTPUT_FILE_SHORT " " FLAG_OUTPUT_FILE_TYPE "          " FLAG_OUTPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_DIMENSION ", -" FLAG_DIMENSION_SHORT " " FLAG_DIMENSION_TYPE "        " FLAG_DIMENSION_DESCRIPTION "\n" \
    "  --" FLAG_ERROR_VECTORS ", -" FLAG_ERROR_VECTORS_SHORT " " FLAG_ERROR_VECTORS_TYPE "    " FLAG_ERROR_VECTORS_DESCRIPTION "\n" \
    "  --" FLAG_PRIME_FIELD ", -" FLAG_PRIME_FIELD_SHORT " " FLAG_PRIME_FIELD_TYPE "      " FLAG_PRIME_FIELD_DESCRIPTION "\n" \
    "  --" FLAG_RANDOM_BITS ", -" FLAG_RANDOM_BITS_SHORT " " FLAG_RANDOM_BITS_TYPE "      " FLAG_RANDOM_BITS_DESCRIPTION "\n" \
    "  --" FLAG_ASCII_MAPPING_LETTERS ", -" FLAG_ASCII_MAPPING_LETTERS_SHORT " " FLAG_ASCII_MAPPING_LETTERS_TYPE " " FLAG_ASCII_MAPPING_LETTERS_DESCRIPTION "\n"

#define USAGE_DECRYPTION_KEY_GENERATION_MODE \
    "Usage for decryption key generation mode:\n" \
    "  --" FLAG_KEY_FILE ", -" FLAG_KEY_FILE_SHORT " " FLAG_KEY_FILE_TYPE "            " FLAG_KEY_FILE_DESCRIPTION "\n" \
    "  --" FLAG_OUTPUT_FILE ", -" FLAG_OUTPUT_FILE_SHORT " " FLAG_OUTPUT_FILE_TYPE "         " FLAG_OUTPUT_FILE_DESCRIPTION "\n"

#define USAGE_DECRYPT_MODE \
    "Usage for decrypt mode:\n" \
    "  --" FLAG_INPUT_FILE ", -" FLAG_INPUT_FILE_SHORT " " FLAG_INPUT_FILE_TYPE "          " FLAG_INPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_OUTPUT_FILE ", -" FLAG_OUTPUT_FILE_SHORT " " FLAG_OUTPUT_FILE_TYPE "         " FLAG_OUTPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_KEY_FILE ", -" FLAG_KEY_FILE_SHORT " " FLAG_KEY_FILE_TYPE "            " FLAG_KEY_FILE_DESCRIPTION "\n"

#define USAGE_ENCRYPT_MODE \
    "Usage for encrypt mode:\n" \
    "  --" FLAG_INPUT_FILE ", -" FLAG_INPUT_FILE_SHORT " " FLAG_INPUT_FILE_TYPE "          " FLAG_INPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_OUTPUT_FILE ", -" FLAG_OUTPUT_FILE_SHORT " " FLAG_OUTPUT_FILE_TYPE "         " FLAG_OUTPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_KEY_FILE ", -" FLAG_KEY_FILE_SHORT " " FLAG_KEY_FILE_TYPE "            " FLAG_KEY_FILE_DESCRIPTION "\n"

#define USAGE_GENERATE_AND_ENCRYPT_MODE \
    "Usage for generate and encrypt mode:\n" \
    "  --" FLAG_INPUT_FILE ", -" FLAG_INPUT_FILE_SHORT " " FLAG_INPUT_FILE_TYPE "          " FLAG_INPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_OUTPUT_FILE ", -" FLAG_OUTPUT_FILE_SHORT " " FLAG_OUTPUT_FILE_TYPE "         " FLAG_OUTPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_KEY_FILE ", -" FLAG_KEY_FILE_SHORT " " FLAG_KEY_FILE_TYPE "            " FLAG_KEY_FILE_DESCRIPTION "\n" \
    "  --" FLAG_DIMENSION ", -" FLAG_DIMENSION_SHORT " " FLAG_DIMENSION_TYPE "        " FLAG_DIMENSION_DESCRIPTION "\n" \
    "  --" FLAG_ERROR_VECTORS ", -" FLAG_ERROR_VECTORS_SHORT " " FLAG_ERROR_VECTORS_TYPE "    " FLAG_ERROR_VECTORS_DESCRIPTION "\n" \
    "  --" FLAG_PRIME_FIELD ", -" FLAG_PRIME_FIELD_SHORT " " FLAG_PRIME_FIELD_TYPE "      " FLAG_PRIME_FIELD_DESCRIPTION "\n" \
    "  --" FLAG_RANDOM_BITS ", -" FLAG_RANDOM_BITS_SHORT " " FLAG_RANDOM_BITS_TYPE "      " FLAG_RANDOM_BITS_DESCRIPTION "\n" \
    "  --" FLAG_ASCII_MAPPING_LETTERS ", -" FLAG_ASCII_MAPPING_LETTERS_SHORT " " FLAG_ASCII_MAPPING_LETTERS_TYPE " " FLAG_ASCII_MAPPING_LETTERS_DESCRIPTION "\n"

#define USAGE_GENERATE_AND_DECRYPT_MODE \
    "Usage for generate and decrypt mode:\n" \
    "  --" FLAG_INPUT_FILE ", -" FLAG_INPUT_FILE_SHORT " " FLAG_INPUT_FILE_TYPE "          " FLAG_INPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_OUTPUT_FILE ", -" FLAG_OUTPUT_FILE_SHORT " " FLAG_OUTPUT_FILE_TYPE "         " FLAG_OUTPUT_FILE_DESCRIPTION "\n" \
    "  --" FLAG_KEY_FILE ", -" FLAG_KEY_FILE_SHORT " " FLAG_KEY_FILE_TYPE "            " FLAG_KEY_FILE_DESCRIPTION "\n" \
    "  --" FLAG_DECRYPTION_KEY_OUTPUT_FILE ", -" FLAG_DECRYPTION_KEY_OUTPUT_FILE_SHORT " " FLAG_DECRYPTION_KEY_OUTPUT_FILE_TYPE " " FLAG_DECRYPTION_KEY_OUTPUT_FILE_DESCRIPTION "\n"

typedef struct
{
    const char* output_file;
    uint32_t dimension;
    uint32_t number_of_error_vectors;
    uint32_t prime_field;
    uint32_t number_of_random_bits_to_add;
    uint32_t number_of_letters_for_each_digit_ascii_mapping;
} KeyGenerationArguments;

typedef struct {
    const char* key;
    const char* output_file;
} DecryptionKeyGenerationArguments;

typedef struct {
    const char* input_file;
    const char* output_file;
    const char* key;
} DecryptArguments;

typedef struct {
    const char* input_file;
    const char* output_file;
    const char* key;
} EncryptArguments;

typedef struct {
    EncryptArguments* encrypt_arguments;
    KeyGenerationArguments* key_generation_arguments;
} GenerateAndEncryptArguments;

typedef struct {
    DecryptArguments* decrypt_arguments;
    DecryptionKeyGenerationArguments* key_generation_arguments;
} GenerateAndDecryptArguments;

/**
 * @brief Parses arguments for the key generation mode.
 *
 * @param out_arguments Pointer to store the parsed arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_key_generation_arguments(KeyGenerationArguments** out_arguments, int argc, char** argv);

/**
 * @brief Parses arguments for the decryption key generation mode.
 *
 * @param out_arguments Pointer to store the parsed arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_decryption_key_generation_arguments(DecryptionKeyGenerationArguments** out_arguments, int argc, char** argv);

/**
 * @brief Parses arguments for the decrypt mode.
 *
 * @param out_arguments Pointer to store the parsed arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_decrypt_arguments(DecryptArguments** out_arguments, int argc, char** argv);

/**
 * @brief Parses arguments for the encrypt mode.
 *
 * @param out_arguments Pointer to store the parsed arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_encrypt_arguments(EncryptArguments** out_arguments, int argc, char** argv);

/**
 * @brief Parses arguments for the generate and encrypt mode.
 *
 * @param out_arguments Pointer to store the parsed arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_generate_and_encrypt_arguments(GenerateAndEncryptArguments** out_arguments, int argc, char** argv);

/**
 * @brief Parses arguments for the generate and decrypt mode.
 *
 * @param out_arguments Pointer to store the parsed arguments.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return STATUS_CODE Status of the operation.
 */
STATUS_CODE parse_generate_and_decrypt_arguments(GenerateAndDecryptArguments** out_arguments, int argc, char** argv);

#endif // MODEPARSERS_H