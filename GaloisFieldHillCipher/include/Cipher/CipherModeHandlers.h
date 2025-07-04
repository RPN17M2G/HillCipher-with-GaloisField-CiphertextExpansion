#ifndef CIPHER_MODE_OPERATIONS_H
#define CIPHER_MODE_OPERATIONS_H

#include "StatusCodes.h"
#include "Parsing/ArgumentParser.h"
#include "IO/PrintUtils.h"
#include "IO/FileOperations.h"
#include "Math/MatrixUtils.h"
#include "IO/SerDes.h"
#include "BlockDividing.h"
#include "Cipher.h"
#include "Cipher/AsciiMapping.h"
#include "log.h"

/**
 * @brief Handle key generation mode - Generate a key and write it to the specified file.
 *
 * @param args - The parsed main arguments
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE handle_key_generation_mode(const ParsedArguments* args);

/**
 * @brief Handle decryption key generation mode - Generate a decryption key from encryption key and write it to the specified file.
 *
 * @param args - The parsed main arguments
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE handle_decryption_key_generation_mode(const ParsedArguments* args);

/**
 * @brief Handle encrypt mode - Encrypt using a key.
 *
 * @param args - The parsed main arguments
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE handle_encrypt_mode(const ParsedArguments* args);

/**
 * @brief Handle decrypt mode - Decrypt using a key.
 *
 * @param args - The parsed main arguments
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE handle_decrypt_mode(const ParsedArguments* args);

/**
 * @brief Handle generate and decrypt mode - Generate a decryption key and then decrypt.
 *
 * @param args - The parsed main arguments
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE handle_generate_and_decrypt_mode(const ParsedArguments* args);

/**
 * @brief Handle generate and encrypt mode - Generate an encryption key and then encrypt.
 *
 * @param args - The parsed main arguments
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE handle_generate_and_encrypt_mode(const ParsedArguments* args);

#endif
