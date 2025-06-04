#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../StatusCodes.h"
#include "../Math/MathUtils.h"
#include "CipherUtils.h"


/**
 * @brief Encrypts a plaintext vector using the Extended Hill Cipher algorithm.
 *
 * @param out_ciphertext - Pointer to the output ciphertext array - allocated inside the function and memory released if fails.
 * @param out_ciphertext_size - Pointer to the size of the ciphertext in bits.
 * @param encryption_matrix - The encryption matrix used for the Hill Cipher. Must be invertible over the prime field.
 * @param dimension - The dimension of the encryption matrix.
 * @param prime_field - The prime field to use for modular arithmetic.
 * @param plaintext_vector - The plaintext vector to be encrypted.
 * @param vector_size - The size of the plaintext vector in bits.
 * @param number_of_random_bits - The number of random bits to insert between each byte of the plaintext vector.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE encrypt(int64_t** out_ciphertext, uint32_t* out_ciphertext_size, int64_t** encryption_matrix, uint32_t dimension, uint32_t prime_field, uint8_t* plaintext_vector, uint32_t vector_size, uint32_t number_of_random_bits);

/**
 * @brief Decrypts a ciphertext vector using the Extended Hill Cipher algorithm.
 *
 * @param out_plaintext - Pointer to the output plaintext array - allocated inside the function and memory released if fails.
 * @param out_plaintext_size - Pointer to the size of the plaintext in bits.
 * @param decryption_matrix - The decryption matrix used for the Hill Cipher.
 * @param dimension - The dimension of the decryption matrix.
 * @param prime_field - The prime field to use for modular arithmetic.
 * @param ciphertext_vector - The ciphertext vector to be decrypted.
 * @param vector_size - The size of the ciphertext vector in bits.
 * @param number_of_random_bits - The number of random bits that were inserted between each byte of the plaintext vector during encryption.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE decrypt(uint8_t** out_plaintext, uint32_t* out_plaintext_size, int64_t** decryption_matrix, uint32_t dimension, uint32_t prime_field, int64_t* ciphertext_vector, uint32_t vector_size, uint32_t number_of_random_bits);

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
