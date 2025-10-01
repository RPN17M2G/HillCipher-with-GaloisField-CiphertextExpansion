#ifndef SECRETSGENERATION_H
#define SECRETSGENERATION_H

#include <stdint.h>

#include "Secrets.h"
#include "Parsing/ArgumentParser.h"
#include "Cipher/CipherParts/CSPRNG.h"
#include "IO/SerDes.h"
#include "Math/MatrixUtils.h"

#define MINIMUM_ASCII_PRINTABLE_CHARACTER (33)
#define MAXIMUM_ASCII_PRINTABLE_CHARACTER (94)

/**
 * Generates a secure random mapping from digits to ASCII characters.
 *
 * @param out_mapping Pointer to the generated mapping
 * @param letters_per_digit Number of letters to map for each digit
 * @param base Numerical base (default is 10 for decimal)
 * @return
 */
STATUS_CODE generate_ascii_mapping(uint8_t*** out_mapping, uint32_t letters_per_digit, uint32_t base);

/**
 * Generates a secure random permutation vector.
 *
 * @param out_permutation_vector Pointer to the generated permutation vector
 * @param length Length of the permutation vector
 * @return
 */
STATUS_CODE generate_permutation_vector(uint8_t** out_permutation_vector, uint32_t length);

/**
 * @brief Generates an encryption matrix with cryptography secure random values.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param dimension - Dimension of the square matrix.
 * @param prime_field - Prime field to use for generating random values.
 * @param max_attempts - Maximum number of attempts to generate an invertible matrix.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_encryption_matrix(int64_t*** out_matrix, uint32_t dimension, uint32_t prime_field, uint32_t max_attempts);

/**
 * @brief Generates a decryption matrix from encryption matrix.
 *
 * @param out_matrix - Pointer to the output matrix - allocated inside the function and memory released if fails.
 * @param dimension - Dimension of the square matrix.
 * @param encryption_matrix - The encryption matrix
 * @param prime_field - Prime field to use for generating random values.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE generate_decryption_matrix(int64_t*** out_matrix, uint32_t dimension, int64_t** encryption_matrix, uint32_t prime_field);

/**
 * @brief Free secrets memory
 *
 * @param secrets - Pointer to the Secrets structure to free.
 */
void free_secrets(Secrets* secrets);

/**
 * @brief Generates all necessary secrets for encryption.
 *
 * @param out_secrets - Pointer to the output Secrets structure - allocated inside the function and memory released if fails.
 * @param args - Key generation arguments containing parameters for secret generation.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE build_encryption_secrets(Secrets** out_secrets, const KeyGenerationArguments *args);

/**
 * @brief Generates all necessary secrets for decryption based on encryption secrets.
 *
 * @param out_secrets - Pointer to the output Secrets structure - allocated inside the function and memory released if fails.
 * @param encryption_secrets - Pointer to the encryption Secrets structure **which is modified in this function**.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE build_decryption_secrets(Secrets** out_secrets, Secrets* encryption_secrets);

#endif //SECRETSGENERATION_H
