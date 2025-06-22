#ifndef MODEPARSERS_H
#define MODEPARSERS_H

#include "Parsing/ArgumentParser.h"
#include "log.h"

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