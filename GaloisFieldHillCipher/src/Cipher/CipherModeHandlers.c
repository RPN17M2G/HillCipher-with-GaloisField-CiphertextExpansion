#include "../../include/Cipher/CipherModeHandlers.h"

STATUS_CODE handle_generate_and_encrypt_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** encryption_matrix = NULL;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;
    uint8_t* plaintext = NULL;
    uint8_t* key_data = NULL;
    int64_t* ciphertext = NULL;
    uint32_t plaintext_size = 0, ciphertext_size = 0;

    if (!args || !args->output_file || (args->dimension == 0) || !args->input_file || !args->key)
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    printf("[*] Generating encryption matrix...\n");

    return_code = generate_encryption_matrix(&encryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, 3);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Encryption matrix generated.\n");
    if (args->verbose)
    {
        print_matrix(encryption_matrix, args->dimension);
    }

    return_code = serialize_matrix(&serialized_data, &serialized_size, encryption_matrix, args->dimension);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:");
        printf("[*] Writing to key file: %s\n", args->key);
    }

    return_code = write_uint8_to_file(args->key, serialized_data, serialized_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading plaintext from: %s\n", args->input_file);
    }

    return_code = read_uint8_from_file(&plaintext, &plaintext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(plaintext, plaintext_size / BYTE_SIZE, "[*] Plaintext data:");
    }

    printf("[*] Encrypting data...\n");

    return_code = encrypt(&ciphertext, &ciphertext_size, encryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    ciphertext_size = (ciphertext_size / (BYTE_SIZE * sizeof(int64_t))); // Size is returned as bits

    printf("[*] Encryption completed, ciphertext size: %ld\n", ciphertext_size);

    if (args->verbose)
    {
        print_int64_vector(ciphertext, ciphertext_size, "[*] Ciphertext data:");
        printf("[*] Writing ciphertext to: %s\n", args->output_file);
    }

    return_code = write_int64_to_file(args->output_file, ciphertext, ciphertext_size);

    if (args->verbose)
    {
        printf("[*] Ciphertext written successfully.\n");
    }

cleanup:
    if (serialized_data)
    {
        free(serialized_data);
    }
    if (plaintext)
    {
        free(plaintext);
    }
    if (key_data)
    {
        free(key_data);
    }
    if (ciphertext)
    {
        free(ciphertext);
    }
    free_matrix(encryption_matrix, args->dimension);
    return return_code;
}

STATUS_CODE handle_key_generation_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** encryption_matrix = NULL;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;

    if (!args || !args->output_file || (args->dimension == 0))
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    printf("[*] Generating encryption matrix...\n");

    return_code = generate_encryption_matrix(&encryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, 3);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Encryption matrix generated.\n");
    if (args->verbose)
    {
        print_matrix(encryption_matrix, args->dimension);
    }

    return_code = serialize_matrix(&serialized_data, &serialized_size, encryption_matrix, args->dimension);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:");
        printf("[*] Writing to key file: %s\n", args->output_file);
    }

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

cleanup:
    if (serialized_data)
    {
        free(serialized_data);
    }
    free_matrix(encryption_matrix, args->dimension);
    return return_code;
}

STATUS_CODE handle_decryption_key_generation_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* key_data = NULL;
    int64_t** encryption_matrix = NULL;
    int64_t** decryption_matrix = NULL;
    uint32_t key_size = 0;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;

    if (!args || !args->key || !args->output_file || (args->dimension == 0))
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    if (args->verbose)
    {
        printf("[*] Reading encryption key from: %s\n", args->key);
    }

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");
    }

    return_code = deserialize_matrix(&encryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Generating decryption matrix...\n");

    return_code = generate_decryption_matrix(&decryption_matrix, args->dimension,
                                           encryption_matrix, DEFAULT_PRIME_GALOIS_FIELD);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Decryption matrix generated.\n");

    if (args->verbose)
    {
        print_matrix(decryption_matrix, args->dimension);
    }

    return_code = serialize_matrix(&serialized_data, &serialized_size, decryption_matrix, args->dimension);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:");
        printf("[*] Writing to key file: %s\n", args->output_file);
    }

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

cleanup:
    free_matrix(encryption_matrix, args->dimension);
    free_matrix(decryption_matrix, args->dimension);
    if (key_data)
    {
        free(key_data);
    }
    return return_code;
}

STATUS_CODE handle_encrypt_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* plaintext = NULL;
    uint8_t* key_data = NULL;
    int64_t** encryption_matrix = NULL;
    int64_t* ciphertext = NULL;
    uint32_t plaintext_size = 0, ciphertext_size = 0, key_size = 0;

    if (!args || !args->input_file || !args->key || !args->output_file || (args->dimension == 0))
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    if (args->verbose)
    {
        printf("[*] Reading plaintext from: %s\n", args->input_file);
    }

    return_code = read_uint8_from_file(&plaintext, &plaintext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(plaintext, plaintext_size / BYTE_SIZE, "[*] Plaintext data:");
    }

    if (args->verbose)
    {
        printf("[*] Reading key from: %s\n", args->key);
    }

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");
    }

    return_code = deserialize_matrix(&encryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_matrix(encryption_matrix, args->dimension);
    }

    printf("[*] Encrypting data...\n");

    return_code = encrypt(&ciphertext, &ciphertext_size, encryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    ciphertext_size = (ciphertext_size / (BYTE_SIZE * sizeof(int64_t))); // Size is returned as bits

    printf("[*] Encryption completed, ciphertext size: %ld\n", ciphertext_size);

    if (args->verbose)
    {
        print_int64_vector(ciphertext, ciphertext_size, "[*] Ciphertext data:");
        printf("[*] Writing ciphertext to: %s\n", args->output_file);
    }

    return_code = write_int64_to_file(args->output_file, ciphertext, ciphertext_size);

    if (args->verbose)
    {
        printf("[*] Ciphertext written successfully.\n");
    }

cleanup:
    if (plaintext)
    {
        free(plaintext);
    }
    if (key_data)
    {
        free(key_data);
    }
    if (ciphertext)
    {
        free(ciphertext);
    }
    free_matrix(encryption_matrix, args->dimension);
    return return_code;
}

STATUS_CODE handle_decrypt_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* key_data = NULL;
    uint8_t* decrypted_text = NULL;
    int64_t* ciphertext = NULL;
    int64_t** decryption_matrix = NULL;
    uint32_t ciphertext_size = 0, decrypted_size = 0, key_size = 0;

    if (!args || !args->input_file || !args->key || !args->output_file || (args->dimension == 0))
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    if (args->verbose)
    {
        printf("Reading ciphertext from: %s\n", args->input_file);
    }

    return_code = read_int64_from_file(&ciphertext, &ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_int64_vector(ciphertext, ciphertext_size / (sizeof(int64_t) * BYTE_SIZE), "[*] Ciphertext data:");
    }

    if (args->verbose)
    {
        printf("[*] Reading key from: %s\n", args->key);
    }

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");
    }

    return_code = deserialize_matrix(&decryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_matrix(decryption_matrix, args->dimension);
    }

    printf("[*] Decrypting data...\n");

    return_code = decrypt(&decrypted_text, &decrypted_size, decryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, ciphertext, ciphertext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    decrypted_size = (decrypted_size / BYTE_SIZE); // Size is returned as bits

    printf("Decryption completed, plaintext size: %ld\n", decrypted_size);

    if (args->verbose)
    {
        print_uint8_vector(decrypted_text, decrypted_size, "[*] Decrypted data:");
        printf("[*] Writing plaintext to: %s\n", args->output_file);
    }

    return_code = write_uint8_to_file(args->output_file, decrypted_text, decrypted_size);

cleanup:
    if (key_data)
    {
        free(key_data);
    }
    if (ciphertext)
    {
        free(ciphertext);
    }
    if (decrypted_text)
    {
        free(decrypted_text);
    }
    free_matrix(decryption_matrix, args->dimension);
    return return_code;
}

STATUS_CODE handle_generate_and_decrypt_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* key_data = NULL;
    uint8_t* decrypted_text = NULL;
    int64_t* ciphertext = NULL;
    int64_t** encryption_matrix = NULL;
    int64_t** decryption_matrix = NULL;
    uint32_t ciphertext_size = 0, decrypted_size = 0, key_size = 0;

    if (!args || !args->input_file || !args->key || !args->output_file || (args->dimension == 0))
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    if (args->verbose)
    {
        printf("[*] Reading ciphertext from: %s\n", args->input_file);
    }

    return_code = read_int64_from_file(&ciphertext, &ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_int64_vector(ciphertext, ciphertext_size / (sizeof(int64_t) * BYTE_SIZE), "[*] Ciphertext data:");
    }

    if (args->verbose)
    {
        printf("[*] Reading key from: %s\n", args->key);
    }

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");
    }

    return_code = deserialize_matrix(&encryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_matrix(encryption_matrix, args->dimension);
    }

    printf("[*] Generating decryption matrix...\n");

    return_code = generate_decryption_matrix(&decryption_matrix, args->dimension, encryption_matrix, DEFAULT_PRIME_GALOIS_FIELD);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Decryption matrix generated.\n");
    if (args->verbose)
    {
        print_matrix(decryption_matrix, args->dimension);
    }

    printf("[*] Decrypting data...\n");

    return_code = decrypt(&decrypted_text, &decrypted_size, decryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, ciphertext, ciphertext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    decrypted_size = (decrypted_size / BYTE_SIZE); // Size is returned as bits

    printf("[*] Decryption completed, plaintext size: %ld\n", decrypted_size);

    if (args->verbose)
    {
        print_uint8_vector(decrypted_text, decrypted_size, "[*] Decrypted data:");
        printf("[*] Writing plaintext to: %s\n", args->output_file);
    }

    return_code = write_uint8_to_file(args->output_file, decrypted_text, decrypted_size);

cleanup:
    if (key_data)
    {
        free(key_data);
    }
    if (ciphertext)
    {
        free(ciphertext);
    }
    if (decrypted_text)
    {
        free(decrypted_text);
    }
    free_matrix(encryption_matrix, args->dimension);
    free_matrix(decryption_matrix, args->dimension);
    return return_code;
}

