#include "Cipher/CipherModeHandlers.h"

STATUS_CODE handle_generate_and_encrypt_mode(const GenerateAndEncryptArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!args || !args->encrypt_arguments || !args->key_generation_arguments)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = handle_key_generation_mode(args->key_generation_arguments);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = handle_encrypt_mode(args->encrypt_arguments);

cleanup:
    free(args->encrypt_arguments);
    free(args->key_generation_arguments);
    free((void*)args);
    return return_code;
}

STATUS_CODE handle_key_generation_mode(const KeyGenerationArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** encryption_matrix = NULL;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;
    Secrets secrets = {0};
    int64_t number_of_digits_per_field_element = 0;
    size_t digit = 0, letter = 0, index = 0;
    uint32_t random_ascii_character = 0, random_permutation_index = 0;

    if (!args || !args->output_file || (0 == args->dimension))
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Generating encryption matrix...\n");

    return_code = generate_encryption_matrix(&encryption_matrix, args->dimension, args->prime_field, 3);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    log_info("[*] Encryption matrix generated.\n");
    print_matrix(encryption_matrix, args->dimension);

    secrets.key_matrix = encryption_matrix;
    secrets.dimension = args->dimension;
    secrets.prime_field = args->prime_field;
    secrets.number_of_error_vectors = args->number_of_error_vectors;

    // Generate error vectors
    if (args->number_of_error_vectors > 0)
    {
        return_code = generate_matrix_over_field(&secrets.error_vectors, args->number_of_error_vectors, args->dimension, args->prime_field);
        if (STATUS_FAILED(return_code))
        {
            goto cleanup;
        }
    }

    log_info("[*] Generating ASCII mapping...\n");
    secrets.ascii_mapping = (uint8_t**)malloc(NUMBER_OF_DIGITS * sizeof(uint8_t*));
    if (!secrets.ascii_mapping)
    {
        log_error("[!] Memory allocation failed for ASCII mapping.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (digit = 0; digit < NUMBER_OF_DIGITS; ++digit)
    {
        secrets.ascii_mapping[digit] = (uint8_t*)malloc(args->number_of_letters_for_each_digit_ascii_mapping * sizeof(uint8_t));
        if (!secrets.ascii_mapping[digit])
        {
            log_error("[!] Memory allocation failed for ASCII mapping.");
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (letter = 0; letter < args->number_of_letters_for_each_digit_ascii_mapping; ++letter)
        {
            return_code = generate_secure_random_number(&random_ascii_character, MINIMUM_ASCII_PRINTABLE_CHARACTER, MAXIMUM_ASCII_PRINTABLE_CHARACTER);
            if (STATUS_FAILED(return_code))
            {
                log_error("[!] Failed to generate random ASCII character.");
                goto cleanup;
            }
            secrets.ascii_mapping[digit][letter] = (uint8_t)random_ascii_character;
        }
    }

    secrets.number_of_letters_for_each_digit_ascii_mapping = args->number_of_letters_for_each_digit_ascii_mapping;

    log_info("[*] Generating permutation vector...\n");
    number_of_digits_per_field_element = calculate_digits_per_element(args->prime_field);
    secrets.permutation_vector = (uint8_t*)malloc(number_of_digits_per_field_element * sizeof(uint8_t));
    if (!secrets.permutation_vector)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (index = 0; index < number_of_digits_per_field_element; ++index)
    {
        return_code = generate_secure_random_number(&random_permutation_index, 0, number_of_digits_per_field_element);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to generate random permutation index.");
            goto cleanup;
        }
        secrets.permutation_vector[index] = random_permutation_index;
    }

    secrets.number_of_random_bits_to_add = args->number_of_random_bits_to_add;

    return_code = serialize_secrets(&serialized_data, &serialized_size, secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(serialized_data, serialized_size, "[*] Serialized secrets data:");
    log_info("[*] Writing to key file: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

cleanup:
    free(serialized_data);
    free_int64_matrix(encryption_matrix, args->dimension);
    free_secrets(&secrets);
    return return_code;
}

STATUS_CODE handle_encrypt_mode(const EncryptArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* plaintext = NULL;
    uint8_t* key_data = NULL;
    int64_t* ciphertext = NULL;
    uint32_t plaintext_size = 0, ciphertext_size = 0, key_size = 0;
    uint8_t* serialized_ciphertext = NULL;
    uint32_t serialized_ciphertext_size = 0;
    Secrets secrets = {0};

    if (!args || !args->input_file || !args->key || !args->output_file)
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Reading plaintext from: %s\n", args->input_file);

    return_code = read_uint8_from_file(&plaintext, &plaintext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(plaintext, plaintext_size / BYTE_SIZE, "[*] Plaintext data:");

    log_info("[*] Reading key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");

    return_code = deserialize_secrets(&secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_matrix(secrets.key_matrix, secrets.dimension);

    return_code = encrypt(&ciphertext, &ciphertext_size, plaintext, plaintext_size, secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    ciphertext_size = (ciphertext_size / (BYTE_SIZE * sizeof(int64_t))); // Size is returned as bits

    log_info("[*] Encryption completed, ciphertext size: %ld\n", ciphertext_size);

    print_int64_vector(ciphertext, ciphertext_size, "[*] Ciphertext data:");
    log_info("[*] Writing ciphertext to: %s\n", args->output_file);

    if (STATUS_SUCCESS(validate_file_is_binary(args->output_file))) // Binary format
    {
        log_info("[*] Serializing ciphertext to binary...\n");
        return_code = serialize_vector(&serialized_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size, secrets.prime_field);
    }
    else // Text format
    {
        log_info("[*] Mapping int64 ciphertext to text...\n");
        return_code = map_from_int64_to_ascii(&serialized_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size);
        // TODO: Add permutation
    }
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(serialized_ciphertext, serialized_ciphertext_size, "[*] Serialized ciphertext data:");

    return_code = write_uint8_to_file(args->output_file, serialized_ciphertext, serialized_ciphertext_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("[*] Ciphertext written successfully.\n");
    }

cleanup:
    free(plaintext);
    free(serialized_ciphertext);
    free(key_data);
    free(ciphertext);
    free_secrets(&secrets);
    free((void*)args);
    return return_code;
}

STATUS_CODE handle_decryption_key_generation_mode(const DecryptionKeyGenerationArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* key_data = NULL;
    int64_t** decryption_matrix = NULL;
    uint32_t key_size = 0;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;
    Secrets encryption_secrets = {0};
    Secrets decryption_secrets = {0};
    uint8_t* reversed_permutation_vector = NULL;
    size_t permutation_size = 0, index = 0;

    if (!args || !args->key || !args->output_file)
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Reading encryption key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");

    return_code = deserialize_secrets(&encryption_secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    log_info("[*] Generating decryption matrix...\n");

    return_code = generate_decryption_matrix(&decryption_matrix, encryption_secrets.dimension,
                                           encryption_secrets.key_matrix, encryption_secrets.prime_field);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    log_info("[*] Decryption matrix generated.\n");

    print_matrix(decryption_matrix, encryption_secrets.dimension);

    // Reverse the permutation vector
    if (encryption_secrets.permutation_vector)
    {
        permutation_size = calculate_digits_per_element(encryption_secrets.prime_field);
        reversed_permutation_vector = (uint8_t*)malloc(permutation_size * sizeof(uint8_t));
        if (!reversed_permutation_vector)
        {
            log_error("[!] Memory allocation failed for reversed permutation vector.");
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (index = 0; index < permutation_size; ++index)
        {
            reversed_permutation_vector[encryption_secrets.permutation_vector[index]] = (uint8_t)index;
        }
    }

    decryption_secrets.dimension = encryption_secrets.dimension;
    decryption_secrets.number_of_error_vectors = encryption_secrets.number_of_error_vectors;
    decryption_secrets.prime_field = encryption_secrets.prime_field;
    decryption_secrets.key_matrix = decryption_matrix;
    decryption_secrets.error_vectors = encryption_secrets.error_vectors;
    decryption_secrets.ascii_mapping = encryption_secrets.ascii_mapping;
    decryption_secrets.permutation_vector = reversed_permutation_vector;
    decryption_secrets.number_of_letters_for_each_digit_ascii_mapping = encryption_secrets.number_of_letters_for_each_digit_ascii_mapping;
    decryption_secrets.number_of_random_bits_to_add = encryption_secrets.number_of_random_bits_to_add;

    return_code = serialize_secrets(&serialized_data, &serialized_size, decryption_secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:");
    log_info("[*] Writing to key file: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

cleanup:
    free(serialized_data);
    free(key_data);
    free_int64_matrix(decryption_matrix, decryption_secrets.dimension);
    free(reversed_permutation_vector);
    free_secrets(&encryption_secrets);
    return return_code;
}

STATUS_CODE handle_decrypt_mode(const DecryptArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* key_data = NULL;
    uint8_t* decrypted_text = NULL;
    int64_t* ciphertext = NULL;
    uint32_t serialized_ciphertext_size = 0;
    uint8_t* serialized_ciphertext = NULL;
    uint32_t ciphertext_size = 0, decrypted_size = 0, key_size = 0;
    Secrets secrets = {0};

    if (!args || !args->input_file || !args->key || !args->output_file)
    {
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Reading key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(key_data, key_size / BYTE_SIZE, "[*] Key data:");

    return_code = deserialize_secrets(&secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to deserialize secrets.");
        goto cleanup;
    }

    log_info("Reading ciphertext from: %s\n", args->input_file);

    return_code = read_uint8_from_file(&serialized_ciphertext, &serialized_ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (STATUS_SUCCESS(validate_file_is_binary((const char*)serialized_ciphertext))) // Binary format
    {
        log_info("[*] Deserializing ciphertext from binary...\n");
        return_code = deserialize_vector(&ciphertext, &ciphertext_size, serialized_ciphertext, serialized_ciphertext_size, secrets.prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to deserialize ciphertext from binary.");
            goto cleanup;
        }
    }
    else // Text format
    {
        // TODO: Add permutation
        log_info("[*] Mapping ASCII ciphertext to int64...\n");
        return_code = map_from_ascii_to_int64(&ciphertext, &ciphertext_size, serialized_ciphertext, serialized_ciphertext_size);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to map ASCII ciphertext to int64.");
            goto cleanup;
        }
    }

    print_int64_vector(ciphertext, ciphertext_size / (sizeof(int64_t) * BYTE_SIZE), "[*] Ciphertext data:");

    return_code = decrypt(&decrypted_text, &decrypted_size, ciphertext, ciphertext_size, secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    decrypted_size = (decrypted_size / BYTE_SIZE); // Size is returned as bits

    log_info("Decryption completed, plaintext size: %ld\n", decrypted_size);

    print_uint8_vector(decrypted_text, decrypted_size, "[*] Decrypted data:");
    log_info("[*] Writing plaintext to: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, decrypted_text, decrypted_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("[*] Plaintext written successfully.\n");
    }

cleanup:
    free(key_data);
    free(serialized_ciphertext);
    free(ciphertext);
    free(decrypted_text);
    free_secrets(&secrets);
    free((void*)args);
    return return_code;
}

STATUS_CODE handle_generate_and_decrypt_mode(const GenerateAndDecryptArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    DecryptionKeyGenerationArguments* key_generation_arguments = args->key_generation_arguments;
    DecryptArguments* decrypt_arguments = args->decrypt_arguments;

    if (!args || !args->decrypt_arguments || !args->key_generation_arguments)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = handle_decryption_key_generation_mode(key_generation_arguments);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = handle_decrypt_mode(decrypt_arguments);

cleanup:
    free(args->decrypt_arguments);
    free(args->key_generation_arguments);
    free((void*)args);
    return return_code;
}
