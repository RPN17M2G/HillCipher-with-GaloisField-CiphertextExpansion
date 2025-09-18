#include "Cipher/CipherModeHandlers.h"

STATUS_CODE handle_generate_and_encrypt_mode(const GenerateAndEncryptArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!args || !args->encrypt_arguments || !args->key_generation_arguments)
    {
        log_error("Invalid arguments in generate_and_encrypt_mode");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_info("Starting generate and encrypt operation...");
    log_debug("Parameters: dimension=%u, prime_field=%u, error_vectors=%u",
        args->key_generation_arguments->dimension,
        args->key_generation_arguments->prime_field,
        args->key_generation_arguments->number_of_error_vectors);

    log_info("Generating encryption key...");
    return_code = handle_key_generation_mode(args->key_generation_arguments);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to generate encryption key");
        goto cleanup;
    }
    log_debug("Key generation completed successfully");

    log_info("Encrypting data...");
    return_code = handle_encrypt_mode(args->encrypt_arguments);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to encrypt data");
        goto cleanup;
    }
    log_info("Encryption completed successfully");

cleanup:
    if (STATUS_FAILED(return_code))
    {
        free((void*)args->encrypt_arguments);
        free((void*)args->key_generation_arguments);
    }
    free((void*)args);
    return return_code;
}

STATUS_CODE handle_key_generation_mode(const KeyGenerationArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* used_indexes = NULL;
    int64_t** encryption_matrix = NULL;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;
    Secrets secrets = {0};
    int64_t number_of_digits_per_field_element = 0;
    size_t digit = 0, letter = 0, index = 0;
    uint32_t random_ascii_character = 0, random_permutation_index = 0;

    if (!args || !args->output_file || (0 == args->dimension))
    {
        log_error("Invalid arguments in key_generation_mode: %s",
            !args ? "args is NULL" :
            !args->output_file ? "output_file is NULL" :
            "dimension is 0");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("Starting key generation with parameters: dimension=%u, prime_field=%u, error_vectors=%u",
             args->dimension, args->prime_field, args->number_of_error_vectors);

    log_debug("Generating encryption matrix...");
    return_code = generate_encryption_matrix(&encryption_matrix, args->dimension, args->prime_field, 3);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to generate encryption matrix");
        goto cleanup;
    }

    log_info("Encryption matrix generated.");
    print_matrix(encryption_matrix, args->dimension, "Encryption matrix generated.", true);

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
            log_error("Failed to generate error vectors");
            goto cleanup;
        }
    }

    log_debug("Generating ASCII mapping with %u letters per digit",
              args->number_of_letters_for_each_digit_ascii_mapping);
    secrets.ascii_mapping = (uint8_t**)malloc(NUMBER_OF_DIGITS * sizeof(uint8_t*));
    if (!secrets.ascii_mapping)
    {
        log_error("Memory allocation failed for ASCII mapping");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (digit = 0; digit < NUMBER_OF_DIGITS; ++digit)
    {
        secrets.ascii_mapping[digit] = (uint8_t*)malloc(args->number_of_letters_for_each_digit_ascii_mapping * sizeof(uint8_t));
        if (!secrets.ascii_mapping[digit])
        {
            log_error("Memory allocation failed for ASCII mapping digit %zu", digit);
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (letter = 0; letter < args->number_of_letters_for_each_digit_ascii_mapping; ++letter)
        {
            return_code = generate_secure_random_number(&random_ascii_character,
                                                      MINIMUM_ASCII_PRINTABLE_CHARACTER,
                                                      MAXIMUM_ASCII_PRINTABLE_CHARACTER);
            if (STATUS_FAILED(return_code))
            {
                log_error("Failed to generate random ASCII character for digit %zu, letter %zu",
                         digit, letter);
                goto cleanup;
            }
            secrets.ascii_mapping[digit][letter] = (uint8_t)random_ascii_character;
            log_debug("Generated random ASCII char for digit %zu: '%c' (0x%02x)",
                     digit, random_ascii_character, random_ascii_character);
        }
    }
    log_info("Successfully generated ASCII mapping");

    log_debug("Generating permutation vector of size %u", number_of_digits_per_field_element);
    secrets.permutation_vector = (uint8_t*)malloc(number_of_digits_per_field_element * sizeof(uint8_t));
    if (!secrets.permutation_vector)
    {
        log_error("Memory allocation failed for permutation vector");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    used_indexes = (uint8_t*)calloc(number_of_digits_per_field_element, sizeof(uint8_t));
    if (!used_indexes)
    {
        log_error("Memory allocation failed for permutation tracking");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (index = 0; index < number_of_digits_per_field_element; ++index)
    {
        do {
            return_code = generate_secure_random_number(&random_permutation_index, 0,
                                                      number_of_digits_per_field_element);
            if (STATUS_FAILED(return_code))
            {
                log_error("Failed to generate random permutation index at position %zu", index);
                goto cleanup;
            }
        } while (used_indexes[random_permutation_index]);

        secrets.permutation_vector[index] = random_permutation_index;
        used_indexes[random_permutation_index] = 1;
        log_debug("Generated permutation: index %zu -> %u", index, random_permutation_index);
    }
    log_info("Successfully generated permutation vector");

    secrets.number_of_letters_for_each_digit_ascii_mapping = args->number_of_letters_for_each_digit_ascii_mapping;

    log_info("[*] Generating permutation vector...\n");
    number_of_digits_per_field_element = calculate_digits_per_element(args->prime_field);
    secrets.permutation_vector = (uint8_t*)malloc(number_of_digits_per_field_element * sizeof(uint8_t));
    if (!secrets.permutation_vector)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    used_indexes = (uint8_t*)calloc(number_of_digits_per_field_element, sizeof(uint8_t));
    if (!used_indexes)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (index = 0; index < number_of_digits_per_field_element; ++index)
    {
        do
        {
            return_code = generate_secure_random_number(&random_permutation_index, 0, number_of_digits_per_field_element);
            if (STATUS_FAILED(return_code))
            {
                log_error("[!] Failed to generate random permutation index.");
                goto cleanup;
            }
        } while (used_indexes[random_permutation_index]);

        secrets.permutation_vector[index] = random_permutation_index;
        used_indexes[random_permutation_index] = 1;
    }

    log_info("[*] Successfully generated permutation vector...\n");

    secrets.number_of_random_bits_to_add = args->number_of_random_bits_to_add;

    return_code = serialize_secrets(&serialized_data, &serialized_size, secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(serialized_data, serialized_size, "[*] Serialized secrets data:", true);
    log_info("[*] Writing to key file: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("[*] Key file written successfully.\n");
    }
    else
    {
        log_error("[!] Failed to write key file: %s\n", args->output_file);
    }

cleanup:
    free(serialized_data);
    free(used_indexes);
    free_secrets(&secrets);
    return return_code;
}

STATUS_CODE handle_encrypt_mode(const EncryptArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* plaintext = NULL;
    uint8_t* key_data = NULL;
    int64_t* ciphertext = NULL;
    uint8_t* mapped_ciphertext = NULL;
    uint32_t plaintext_size = 0, ciphertext_size = 0, key_size = 0;
    uint8_t* serialized_ciphertext = NULL;
    uint32_t serialized_ciphertext_size = 0;
    Secrets secrets = {0};

    if (!args || !args->input_file || !args->key || !args->output_file)
    {
        log_error("Invalid arguments in encrypt_mode");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Reading plaintext from: %s\n", args->input_file);

    return_code = read_uint8_from_file(&plaintext, &plaintext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read plaintext file");
        goto cleanup;
    }

    print_uint8_vector(plaintext, plaintext_size, "[*] Plaintext data:", false);

    log_info("[*] Reading key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read key file");
        goto cleanup;
    }

    print_uint8_vector(key_data, key_size, "[*] Key data:", true);

    return_code = deserialize_secrets(&secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to deserialize secrets");
        goto cleanup;
    }

    log_info("[*] Deserialized secrets.\n");

    return_code = encrypt(&ciphertext, &ciphertext_size, plaintext, plaintext_size * BYTE_SIZE, secrets);
    if (STATUS_FAILED(return_code))
    {
        log_error("Encryption process failed");
        goto cleanup;
    }
    ciphertext_size = (ciphertext_size / (BYTE_SIZE * sizeof(int64_t))); // Size is returned as bits

    log_info("[*] Encryption completed, ciphertext size: %ld\n", ciphertext_size);

    print_int64_vector(ciphertext, ciphertext_size, "[*] Ciphertext data:", false);
    log_info("[*] Writing ciphertext to: %s\n", args->output_file);

    if (STATUS_SUCCESS(validate_file_is_binary(args->output_file))) // Binary format
    {
        log_info("[*] Serializing ciphertext to binary...\n");
        return_code = serialize_vector(&serialized_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size, secrets.prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to serialize ciphertext to compact binary.");
            goto cleanup;
        }

    }
    else // Text format
    {
        log_info("[*] Mapping int64 ciphertext to text...\n");
        return_code = map_from_int64_to_ascii(&mapped_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size, secrets.ascii_mapping, secrets.number_of_letters_for_each_digit_ascii_mapping, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to map int64 ciphertext to ASCII.");
            goto cleanup;
        }
        log_info("[*] Successfully mapped int64 ciphertext to ASCII.");

        log_info("[*] Permutating ASCII ciphertext...\n");
        return_code = permutate_uint8_vector(&serialized_ciphertext, mapped_ciphertext, serialized_ciphertext_size, secrets.permutation_vector, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to permutate ASCII ciphertext.");
            goto cleanup;
        }
        log_info("[*] Successfully permutated ASCII ciphertext");

        serialized_ciphertext_size++; // Add one for the null terminator
    }

    print_uint8_vector(serialized_ciphertext, serialized_ciphertext_size, "[*] Serialized ciphertext data:", true);

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
    free(mapped_ciphertext);
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
        log_error("Invalid arguments in decryption_key_generation_mode");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Reading encryption key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read encryption key file");
        goto cleanup;
    }

    print_uint8_vector(key_data, key_size, "[*] Key data:", true);

    log_info("[*] Deserializing encryption secrets...\n");

    return_code = deserialize_secrets(&encryption_secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to deserialize encryption secrets");
        goto cleanup;
    }

    log_info("[*] Successfully deserialized encryption secrets.\n");
    log_info("[*] Generating decryption matrix...\n");

    return_code = generate_decryption_matrix(&decryption_matrix, encryption_secrets.dimension,
                                           encryption_secrets.key_matrix, encryption_secrets.prime_field);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to generate decryption matrix");
        goto cleanup;
    }

    log_info("[*] Decryption matrix generated:\n");

    print_matrix(decryption_matrix, encryption_secrets.dimension, "Decryption matrix generated:\n", true);

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
    encryption_secrets.error_vectors = NULL;
    decryption_secrets.ascii_mapping = encryption_secrets.ascii_mapping;
    encryption_secrets.ascii_mapping = NULL;
    decryption_secrets.permutation_vector = reversed_permutation_vector;
    decryption_secrets.number_of_letters_for_each_digit_ascii_mapping = encryption_secrets.number_of_letters_for_each_digit_ascii_mapping;
    decryption_secrets.number_of_random_bits_to_add = encryption_secrets.number_of_random_bits_to_add;

    return_code = serialize_secrets(&serialized_data, &serialized_size, decryption_secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    print_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:", true);
    log_info("[*] Writing to key file: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("[*] Key file written successfully.\n");
    }
    else
    {
        log_error("[!] Failed to write key file: %s\n", args->output_file);
    }

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
    uint8_t* ciphertext_permutated = NULL;
    uint8_t* serialized_ciphertext = NULL;
    uint32_t ciphertext_size = 0, decrypted_size = 0, key_size = 0;
    Secrets secrets = {0};

    if (!args || !args->input_file || !args->key || !args->output_file)
    {
        log_error("Invalid arguments in decrypt_mode");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("[*] Reading key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read key file");
        goto cleanup;
    }

    print_uint8_vector(key_data, key_size, "[*] Key data:", true);

    log_info("[*] Deserializing secrets...\n");

    return_code = deserialize_secrets(&secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to deserialize secrets.");
        goto cleanup;
    }

    log_info("[*] Successfully deserialized secrets.\n");

    log_info("Reading ciphertext from: %s\n", args->input_file);

    return_code = read_uint8_from_file(&serialized_ciphertext, &serialized_ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read ciphertext file");
        goto cleanup;
    }

    if (STATUS_SUCCESS(validate_file_is_binary(args->input_file))) // Binary format
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
        log_info("[*] Permutating ASCII ciphertext...\n");

        serialized_ciphertext_size -= 1; // Remove NULL terminator

        return_code = permutate_uint8_vector(&ciphertext_permutated, serialized_ciphertext, serialized_ciphertext_size, secrets.permutation_vector, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to permutate ASCII ciphertext.");
            goto cleanup;
        }

        log_info("[*] Successfully permutated ASCII ciphertext.\n");

        log_info("[*] Mapping ASCII ciphertext to int64...\n");

        return_code = map_from_ascii_to_int64(&ciphertext, &ciphertext_size, ciphertext_permutated, serialized_ciphertext_size, secrets.ascii_mapping, secrets.number_of_letters_for_each_digit_ascii_mapping, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to map ASCII ciphertext to int64.");
            goto cleanup;
        }

        log_info("[*] Successfully mapped ASCII ciphertext to int64.\n");
    }

    print_int64_vector(ciphertext, ciphertext_size / (sizeof(int64_t) * BYTE_SIZE), "[*] Ciphertext data:", false);

    return_code = decrypt(&decrypted_text, &decrypted_size, ciphertext, ciphertext_size * BYTE_SIZE, secrets);
    if (STATUS_FAILED(return_code))
    {
        log_error("Decryption process failed");
        goto cleanup;
    }
    decrypted_size = (decrypted_size / BYTE_SIZE); // Size is returned as bits

    log_info("Decryption completed, plaintext size: %ld\n", decrypted_size);

    print_uint8_vector(decrypted_text, decrypted_size, "[*] Decrypted data:", false);
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
    free(ciphertext_permutated);
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
        log_error("Invalid arguments in generate_and_decrypt_mode");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = handle_decryption_key_generation_mode(key_generation_arguments);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to generate decryption key");
        goto cleanup;
    }

    return_code = handle_decrypt_mode(decrypt_arguments);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to decrypt data");
        goto cleanup;
    }

cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(args->decrypt_arguments);
        free(args->key_generation_arguments);
    }
    free((void*)args);
    return return_code;
}
