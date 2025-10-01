#include "Cipher/CipherModeHandlers.h"

STATUS_CODE handle_generate_and_encrypt_mode(const GenerateAndEncryptArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    bool reached_encryption = false;

    if (!args || !args->encrypt_arguments || !args->key_generation_arguments)
    {
        log_error("[!] Invalid arguments in generate_and_encrypt_mode");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    printf("[*] Starting generate and encrypt operation...");
    printf("[*] Parameters: dimension=%u, prime_field=%u, error_vectors=%u",
        args->key_generation_arguments->dimension,
        args->key_generation_arguments->prime_field,
        args->key_generation_arguments->number_of_error_vectors);
    log_info("Starting generate and encrypt operation...");
    log_info("Parameters: dimension=%u, prime_field=%u, error_vectors=%u",
        args->key_generation_arguments->dimension,
        args->key_generation_arguments->prime_field,
        args->key_generation_arguments->number_of_error_vectors);

    return_code = handle_key_generation_mode(args->key_generation_arguments);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to generate encryption key");
        goto cleanup;
    }

    reached_encryption = true;

    return_code = handle_encrypt_mode(args->encrypt_arguments);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to encrypt data");
        printf("Failed to encrypt data");
        goto cleanup;
    }

    log_info("Generate and encrypt operation completed successfully");
    printf("[*] Generate and encrypt operation completed successfully");

cleanup:
    if (STATUS_FAILED(return_code))
    {
        if (reached_encryption)
            free((void*)args->encrypt_arguments);
        free((void*)args->key_generation_arguments);
    }
    free((void*)args);
    return return_code;
}

STATUS_CODE handle_key_generation_mode(const KeyGenerationArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    bool* used_indexes = NULL;
    bool* used_ascii_characters = NULL;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;
    Secrets* secrets = NULL;

    if (!args || !args->output_file || (0 == args->dimension))
    {
        log_error("[!] Invalid arguments in key_generation_mode: %s",
            !args ? "args is NULL" :
            !args->output_file ? "output_file is NULL" :
            "dimension is 0");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    log_info("Generating encryption key...");
    printf("[*] Generating encryption key...");

    return_code = build_encryption_secrets(&secrets, args);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to build encryption secrets");
        goto cleanup;
    }

    return_code = serialize_secrets(&serialized_data, &serialized_size, *secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    log_info("Key generation completed successfully");
    printf("Key generation completed successfully.\n");

    log_uint8_vector(serialized_data, serialized_size, "Serialized secrets data:", true);
    log_info("Writing key to file: %s\n", args->output_file);
    printf("[*] Writing key to file: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("Key file written successfully.\n");
    }
    else
    {
        log_error("[!] Failed to write key file: %s\n", args->output_file);
    }

cleanup:
    free(serialized_data);
    free(used_indexes);
    free(used_ascii_characters);
    free_secrets(secrets);
    free(secrets);
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
        log_error("[!] Invalid arguments in encrypt_mode");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    printf("[*] Starting encryption operation...");
    log_info("Starting encryption operation...");

    log_info("Reading plaintext from: %s\n", args->input_file);

    return_code = read_uint8_from_file(&plaintext, &plaintext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to read plaintext file");
        goto cleanup;
    }

    log_uint8_vector(plaintext, plaintext_size, "[*] Plaintext data:", false);

    log_info("Reading key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to read key file");
        goto cleanup;
    }

    log_uint8_vector(key_data, key_size, "Key data:", true);

    return_code = deserialize_secrets(&secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to deserialize secrets");
        goto cleanup;
    }

    log_info("Deserialized secrets.\n");

    return_code = encrypt(&ciphertext, &ciphertext_size, plaintext, plaintext_size * BYTE_SIZE, secrets);
    if (STATUS_FAILED(return_code))
    {
        log_error("Encryption process failed");
        goto cleanup;
    }
    ciphertext_size = (ciphertext_size / (BYTE_SIZE * sizeof(int64_t))); // Size is returned as bits

    log_info("Encryption completed, ciphertext size: %ld\n", ciphertext_size);

    log_int64_vector(ciphertext, ciphertext_size, "Ciphertext data:", false);

    if (STATUS_SUCCESS(validate_file_is_binary(args->output_file))) // Binary format
    {
        log_info("Serializing ciphertext to binary...\n");
        return_code = serialize_vector(&serialized_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size, secrets.prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to serialize ciphertext to compact binary.");
            goto cleanup;
        }

    }
    else // Text format
    {
        log_info("Mapping int64 ciphertext to text...\n");
        return_code = map_from_int64_to_ascii(&mapped_ciphertext, &serialized_ciphertext_size, ciphertext, ciphertext_size, secrets.ascii_mapping, secrets.number_of_letters_for_each_digit_ascii_mapping, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to map int64 ciphertext to ASCII.");
            goto cleanup;
        }
        log_info("Successfully mapped int64 ciphertext to ASCII.");

        log_info("Permutating ASCII ciphertext...\n");
        return_code = permutate_uint8_vector(&serialized_ciphertext, mapped_ciphertext, serialized_ciphertext_size, secrets.permutation_vector, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to permutate ASCII ciphertext.");
            goto cleanup;
        }
        log_info("Successfully permutated ASCII ciphertext");

        serialized_ciphertext_size++; // Add one for the null terminator
    }

    log_uint8_vector(serialized_ciphertext, serialized_ciphertext_size, "Serialized ciphertext data:", true);

    log_info("Encryption completed successfully.\n");
    printf("[*] Encryption completed successfully.\n");

    log_info("Writing ciphertext to: %s\n", args->output_file);
    printf("[*] Writing ciphertext to: %s\n", args->output_file);
    return_code = write_uint8_to_file(args->output_file, serialized_ciphertext, serialized_ciphertext_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("Ciphertext written successfully.\n");
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
    uint32_t key_size = 0;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;
    Secrets encryption_secrets = {0};
    Secrets* decryption_secrets = NULL;

    if (!args || !args->key || !args->output_file)
    {
        log_error("Invalid arguments in decryption_key_generation_mode");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    printf("[*] Starting decryption key generation operation...");
    log_info("Starting decryption key generation operation...");

    log_info("Reading encryption key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read encryption key file");
        goto cleanup;
    }

    log_uint8_vector(key_data, key_size, "Key data:", true);

    log_info("Deserializing encryption secrets...\n");

    return_code = deserialize_secrets(&encryption_secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to deserialize encryption secrets");
        goto cleanup;
    }
    log_info("Successfully deserialized encryption secrets.\n");

    return_code = build_decryption_secrets(&decryption_secrets, &encryption_secrets);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to build decryption secrets");
        goto cleanup;
    }
    log_info("Successfully built decryption secrets.\n");

    return_code = serialize_secrets(&serialized_data, &serialized_size, *decryption_secrets);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Decryption key generation completed successfully.\n");
    log_info("Decryption key generation completed successfully.\n");

    log_uint8_vector(serialized_data, serialized_size, "[*] Serialized matrix data:", true);
    log_info("Writing to key file: %s\n", args->output_file);
    printf("[*] Writing to key file: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, serialized_data, serialized_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("Key file written successfully.\n");
    }
    else
    {
        log_error("[!] Failed to write key file: %s\n", args->output_file);
    }

cleanup:
    free(serialized_data);
    free(key_data);
    free_secrets(&encryption_secrets);
    free_secrets(decryption_secrets);
    free(decryption_secrets);
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
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    printf("[*] Starting decryption operation...");
    log_info("Starting decryption operation...");

    log_info("Reading key from: %s\n", args->key);

    return_code = read_uint8_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read key file");
        goto cleanup;
    }

    log_uint8_vector(key_data, key_size, "[*] Key data:", true);

    log_info("Deserializing secrets...\n");

    return_code = deserialize_secrets(&secrets, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to deserialize secrets.");
        goto cleanup;
    }

    log_info("Successfully deserialized secrets.\n");

    log_info("Reading ciphertext from: %s\n", args->input_file);

    return_code = read_uint8_from_file(&serialized_ciphertext, &serialized_ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to read ciphertext file");
        goto cleanup;
    }

    if (STATUS_SUCCESS(validate_file_is_binary(args->input_file))) // Binary format
    {
        log_info("Deserializing ciphertext from binary...\n");
        return_code = deserialize_vector(&ciphertext, &ciphertext_size, serialized_ciphertext, serialized_ciphertext_size, secrets.prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to deserialize ciphertext from binary.");
            goto cleanup;
        }
    }
    else // Text format
    {
        log_info("Permutating ASCII ciphertext...\n");

        serialized_ciphertext_size -= 1; // Remove NULL terminator

        return_code = permutate_uint8_vector(&ciphertext_permutated, serialized_ciphertext, serialized_ciphertext_size, secrets.permutation_vector, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to permutate ASCII ciphertext.");
            goto cleanup;
        }

        log_info("Successfully permutated ASCII ciphertext.\n");

        log_info("Mapping ASCII ciphertext to int64...\n");

        return_code = map_from_ascii_to_int64(&ciphertext, &ciphertext_size, ciphertext_permutated, serialized_ciphertext_size, secrets.ascii_mapping, secrets.number_of_letters_for_each_digit_ascii_mapping, calculate_digits_per_element(secrets.prime_field));
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to map ASCII ciphertext to int64.");
            goto cleanup;
        }

        log_info("Successfully mapped ASCII ciphertext to int64.\n");
    }

    log_int64_vector(ciphertext, ciphertext_size / sizeof(int64_t), "[*] Ciphertext data:", false);

    return_code = decrypt(&decrypted_text, &decrypted_size, ciphertext, ciphertext_size * BYTE_SIZE, secrets);
    if (STATUS_FAILED(return_code))
    {
        log_error("Decryption process failed");
        goto cleanup;
    }
    decrypted_size = (decrypted_size / BYTE_SIZE); // Size is returned as bits

    log_info("Decryption completed, plaintext size: %ld\n", decrypted_size);
    printf("[*] Decryption completed successfully, plaintext size: %ld\n", decrypted_size);

    log_uint8_vector(decrypted_text, decrypted_size, "[*] Decrypted data:", false);
    log_info("Writing plaintext to: %s\n", args->output_file);
    printf("[*] Writing plaintext to: %s\n", args->output_file);

    return_code = write_uint8_to_file(args->output_file, decrypted_text, decrypted_size);

    if (STATUS_SUCCESS(return_code))
    {
        log_info("Plaintext written successfully.\n");
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

    printf("[*] Starting generate and decrypt operation...");
    log_info("Starting generate and decrypt operation...");

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

    printf("[*] Generate and decrypt operation completed successfully");
    log_info("Generate and decrypt operation completed successfully");

cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(args->decrypt_arguments);
        free(args->key_generation_arguments);
    }
    free((void*)args);
    return return_code;
}
