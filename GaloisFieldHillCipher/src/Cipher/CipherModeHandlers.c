#include "Cipher/CipherModeHandlers.h"

STATUS_CODE handle_generate_and_encrypt_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    ParsedArguments key_gen_args = *args;
    ParsedArguments encrypt_args = *args;

    if (!args || !args->output_file || (0 == args->dimension) || !args->input_file || !args->key)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    key_gen_args.output_file = args->key;
    return_code = handle_key_generation_mode(&key_gen_args);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = handle_encrypt_mode(&encrypt_args);

cleanup:
    return return_code;
}

STATUS_CODE handle_key_generation_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** encryption_matrix = NULL;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;

    if (!args || !args->output_file || (0 == args->dimension))
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Generating encryption matrix...\n");

    return_code = generate_encryption_matrix(&encryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, 3);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    log_info("[*] Encryption matrix generated.\n");
    if (args->verbose)
    {
        print_matrix(encryption_matrix, args->dimension);
    }

    return_code = serialize_square_matrix(&serialized_data, &serialized_size, encryption_matrix, args->dimension);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:");
        log_info("[*] Writing to key file: %s\n", args->output_file);
    }

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

cleanup:
    free(serialized_data);
    (void)free_int64_matrix(encryption_matrix, args->dimension);
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
    uint8_t* serialized_ciphertext = NULL;
    uint32_t serialized_ciphertext_size = 0;
    uint32_t dimension = 0;
    int64_t** error_vector_matrix = NULL;

    if (!args || !args->input_file || !args->key || !args->output_file)
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    if (args->verbose)
    {
        log_info("[*] Reading plaintext from: %s\n", args->input_file);
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
        log_info("[*] Reading key from: %s\n", args->key);
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

    dimension = sqrt(key_size / (BYTE_SIZE * NUMBER_OF_BYTES_PER_ELEMENT));
    return_code = deserialize_square_matrix(&encryption_matrix, dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_matrix(encryption_matrix, dimension);
    }

    return_code = encrypt(&ciphertext, &ciphertext_size, encryption_matrix, dimension, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_size, args->number_of_random_bits_between_bytes, error_vector_matrix, args->number_of_error_vectors);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    ciphertext_size = (ciphertext_size / (BYTE_SIZE * sizeof(int64_t))); // Size is returned as bits

    log_info("[*] Encryption completed, ciphertext size: %ld\n", ciphertext_size);

    if (args->verbose)
    {
        print_int64_vector(ciphertext, ciphertext_size, "[*] Ciphertext data:");
        log_info("[*] Writing ciphertext to: %s\n", args->output_file);
    }

    if (args->output_format == OUTPUT_FORMAT_BINARY)
    {
        if (args->verbose)
        {
            log_info("[*] Serializing ciphertext to binary...\n");
        }
        return_code = serialize_vector(&serialized_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size);
    }
    else // Text format
    {
        if (args->verbose)
        {
            log_info("[*] Mapping int64 ciphertext to text...\n");
        }
        return_code = map_from_int64_to_ascii(&serialized_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size);
    }
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    if (args->verbose)
    {
        print_uint8_vector(serialized_ciphertext, serialized_ciphertext_size, "[*] Serialized ciphertext data:");
    }

    return_code = write_uint8_to_file(args->output_file, serialized_ciphertext, serialized_ciphertext_size);

    if (args->verbose && STATUS_SUCCESS(return_code))
    {
        log_info("[*] Ciphertext written successfully.\n");
    }

cleanup:
    free(plaintext);
    free(serialized_ciphertext);
    free(key_data);
    free(ciphertext);
    (void)free_int64_matrix(encryption_matrix, dimension);
    (void)free_int64_matrix(error_vector_matrix, args->number_of_error_vectors);
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
    uint32_t dimension = 0;

    if (!args || !args->key || !args->output_file)
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    if (args->verbose)
    {
        log_info("[*] Reading encryption key from: %s\n", args->key);
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

    dimension = sqrt(key_size / (BYTE_SIZE * NUMBER_OF_BYTES_PER_ELEMENT));
    return_code = deserialize_square_matrix(&encryption_matrix, dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    log_info("[*] Generating decryption matrix...\n");

    return_code = generate_decryption_matrix(&decryption_matrix, dimension,
                                           encryption_matrix, DEFAULT_PRIME_GALOIS_FIELD);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    log_info("[*] Decryption matrix generated.\n");

    if (args->verbose)
    {
        print_matrix(decryption_matrix, dimension);
    }

    return_code = serialize_square_matrix(&serialized_data, &serialized_size, decryption_matrix, dimension);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        print_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:");
        log_info("[*] Writing to key file: %s\n", args->output_file);
    }

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

cleanup:
    (void)free_int64_matrix(encryption_matrix, dimension);
    (void)free_int64_matrix(decryption_matrix, dimension);
    free(key_data);
    return return_code;
}

STATUS_CODE handle_decrypt_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* key_data = NULL;
    uint8_t* decrypted_text = NULL;
    int64_t* ciphertext = NULL;
    uint32_t serialized_ciphertext_size = 0;
    uint8_t* serialized_ciphertext = NULL;
    int64_t** decryption_matrix = NULL;
    uint32_t ciphertext_size = 0, decrypted_size = 0, key_size = 0;
    uint32_t dimension = 0;
    int64_t** error_vector_matrix = NULL;
    uint8_t* error_vector_data = NULL;

    if (!args || !args->input_file || !args->key || !args->output_file)
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    if (args->verbose)
    {
        log_info("Reading ciphertext from: %s\n", args->input_file);
    }

    return_code = read_uint8_from_file(&serialized_ciphertext, &serialized_ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    return_code = deserialize_vector(&ciphertext, &ciphertext_size, serialized_ciphertext, serialized_ciphertext_size);
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
        log_info("[*] Reading key from: %s\n", args->key);
    }

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    dimension = sqrt(key_size / (BYTE_SIZE * NUMBER_OF_BYTES_PER_ELEMENT));

    if (args->verbose)
    {
        print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");
    }

    // TODO: Read error vectors from a file if/when persistent storage is required

    return_code = decrypt(&decrypted_text, &decrypted_size, decryption_matrix, dimension, DEFAULT_PRIME_GALOIS_FIELD, ciphertext, ciphertext_size, args->number_of_random_bits_between_bytes, error_vector_matrix, args->number_of_error_vectors);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    decrypted_size = (decrypted_size / BYTE_SIZE); // Size is returned as bits

    log_info("Decryption completed, plaintext size: %ld\n", decrypted_size);

    if (args->verbose)
    {
        print_uint8_vector(decrypted_text, decrypted_size, "[*] Decrypted data:");
        log_info("[*] Writing plaintext to: %s\n", args->output_file);
    }

    return_code = write_uint8_to_file(args->output_file, decrypted_text, decrypted_size);

    if (args->verbose && STATUS_SUCCESS(return_code))
    {
        log_info("[*] Plaintext written successfully.\n");
    }

cleanup:
    free(key_data);
    free(serialized_ciphertext);
    free(ciphertext);
    free(decrypted_text);
    free(error_vector_data);
    (void)free_int64_matrix(decryption_matrix, dimension);
    (void)free_int64_matrix(error_vector_matrix, args->number_of_error_vectors);
    return return_code;
}

STATUS_CODE handle_generate_and_decrypt_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    ParsedArguments key_gen_args = *args;
    ParsedArguments decrypt_args = *args;

    if (!args || !args->input_file || !args->key || !args->output_file)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    key_gen_args.output_file = args->decryption_key;
    return_code = handle_decryption_key_generation_mode(&key_gen_args);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    decrypt_args.key = args->decryption_key;
    return_code = handle_decrypt_mode(&decrypt_args);

cleanup:
    return return_code;
}