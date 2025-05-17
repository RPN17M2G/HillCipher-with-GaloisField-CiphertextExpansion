#pragma once

#include "StatusCodes.h"
#include "ArgumentParser.h"
#include "PrintUtils.h"
#include "FileOperations.h"
#include "MatrixUtils.h"
#include "MatrixSerDes.h"
#include "CipherUtils.h"
#include "Cipher.h"

/**
 * @brief Handle key generation mode operation
 *
 * Generates an encryption matrix and saves it to a file.
 * If verbose mode is enabled, prints the matrix and operation details.
 *
 * @param args Pointer to ParsedArguments structure containing configuration
 * @return STATUS_CODE indicating operation success or failure
 */
STATUS_CODE handle_key_generation_mode(const ParsedArguments* args);

/**
 * @brief Handle decryption key generation mode operation
 *
 * Reads an encryption key and generates corresponding decryption matrix.
 * If verbose mode is enabled, prints matrices and operation details.
 *
 * @param args Pointer to ParsedArguments structure containing configuration
 * @return STATUS_CODE indicating operation success or failure
 */
STATUS_CODE handle_decryption_key_generation_mode(const ParsedArguments* args);

/**
 * @brief Handle encryption mode operation
 *
 * Reads plaintext and encryption key, performs encryption, and saves result.
 * If verbose mode is enabled, prints data and operation details.
 *
 * @param args Pointer to ParsedArguments structure containing configuration
 * @return STATUS_CODE indicating operation success or failure
 */
STATUS_CODE handle_encrypt_mode(const ParsedArguments* args);

/**
 * @brief Handle decryption mode operation
 *
 * Reads ciphertext and decryption key, performs decryption, and saves result.
 * If verbose mode is enabled, prints data and operation details.
 *
 * @param args Pointer to ParsedArguments structure containing configuration
 * @return STATUS_CODE indicating operation success or failure
 */
STATUS_CODE handle_decrypt_mode(const ParsedArguments* args);

/**
 * @brief Handle generate and encrypt mode operation
 *
 * Generates encryption key and immediately uses it for encryption.
 * Combines key generation and encryption operations.
 *
 * @param args Pointer to ParsedArguments structure containing configuration
 * @return STATUS_CODE indicating operation success or failure
 */
STATUS_CODE handle_generate_and_encrypt_mode(const ParsedArguments* args);

/**
 * @brief Handle generate and decrypt mode operation
 *
 * Reads encryption key, generates decryption matrix, and performs decryption.
 * Combines decryption key generation and decryption operations.
 *
 * @param args Pointer to ParsedArguments structure containing configuration
 * @return STATUS_CODE indicating operation success or failure
 */
STATUS_CODE handle_generate_and_decrypt_mode(const ParsedArguments* args);
