#ifndef CIPHER_H
#define CIPHER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "Math/MathUtils.h"
#include "CipherParts/BlockDividing.h"
#include "Math/MatrixInverse.h"
#include "Math/MatrixMultiplication.h"
#include "CipherParts/CiphertextExpansion.h"
#include "CipherParts/Padding.h"
#include "Cipher/CipherParts/AffineTransformation.h"
#include "IO/SerDes.h"
#include "../Secrets/Secrets.h"
#include "IO/PrintUtils.h"
#include "log.h"


/**
 * @brief Encrypts a plaintext vector using the Extended Hill Cipher algorithm with affine transformation (error vectors).
 *
 * @param out_ciphertext - Pointer to the output ciphertext array - allocated inside the function and memory released if fails.
 * @param out_ciphertext_size - Pointer to the size of the ciphertext in bits.
 * @param plaintext_vector - The plaintext vector to be encrypted.
 * @param vector_size - The size of the plaintext vector in bits.
 * @param secrets - The secrets containing the encryption matrix, error vectors, and other parameters.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE encrypt(int64_t** out_ciphertext, uint32_t* out_ciphertext_size, uint8_t* plaintext_vector, uint32_t vector_size, Secrets secrets);

/**
 * @brief Decrypts a ciphertext vector using the Extended Hill Cipher algorithm with affine transformation (error vectors).
 *
 * @param out_plaintext - Pointer to the output plaintext array - allocated inside the function and memory released if fails.
 * @param out_plaintext_size - Pointer to the size of the plaintext in bits.
 * @param ciphertext_vector - The ciphertext vector to be decrypted.
 * @param vector_size - The size of the ciphertext vector in bits.
 * @param secrets - The secrets containing the decryption matrix, error vectors, and other parameters.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE decrypt(uint8_t** out_plaintext, uint32_t* out_plaintext_size, int64_t* ciphertext_vector, uint32_t vector_size, Secrets secrets);

#endif

