#include "GaloisFieldHillCipher.h"



STATUS_CODE handle_key_generation_mode(const ParsedArguments* args)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** encryption_matrix = NULL;
    uint8_t* serialized_data = NULL;
    uint32_t serialized_size = 0;

    if (!args || !args->key || (args->dimension == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
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
    
    return_code = serialize_matrix(encryption_matrix, args->dimension, &serialized_data, &serialized_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Serialized encryption matrix. Writing to key file: %s\n", args->key);
    }

    return_code = write_to_file(args->key, serialized_data, serialized_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(serialized_data);
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

    if (!args || !args->key || (args->dimension == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading encryption key file: %s\n", args->key);
    }

    return_code = read_from_file(args->key, &key_data, &key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = deserialize_matrix(&encryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
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

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free_matrix(encryption_matrix, args->dimension);
    free_matrix(decryption_matrix, args->dimension);
    free(key_data);
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
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading plaintext from: %s\n", args->input_file);
    }

    return_code = read_from_file(args->input_file, &plaintext, &plaintext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading key from: %s\n", args->key);
    }

    return_code = read_from_file(&key_data, &key_size, args->key);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = deserialize_matrix(&encryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Encrypting...\n");
    
    return_code = encrypt(&ciphertext, &ciphertext_size, encryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

	printf("[*] Encryption completed. Ciphertext size: %u bytes\n", ciphertext_size);
    if (args->verbose)
    {
        printf("[*] Writing ciphertext to: %s\n", args->output_file);
    }

    return_code = write_to_file(args->output_file, (uint8_t*)ciphertext, ciphertext_size);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(plaintext);
    free(key_data);
    free(ciphertext);
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
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading ciphertext from: %s\n", args->input_file);
    }

    return_code = read_from_file((uint8_t**)&ciphertext, &ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading key from: %s\n", args->key);
    }
    return_code = read_from_file(args->key, &key_data, &key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = deserialize_matrix(&decryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Decrypting...\n");

    return_code = decrypt(&decrypted_text, &decrypted_size, decryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, ciphertext, ciphertext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Decryption completed. Plaintext size: %u bytes\n", decrypted_size);
    if (args->verbose)
    {
        printf("[*] Writing plaintext to: %s\n", args->output_file);
    }
    return_code = write_to_file(args->output_file, decrypted_text, decrypted_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(key_data);
    free(ciphertext);
    free(decrypted_text);
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
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading ciphertext from: %s\n", args->input_file);
    }

    return_code = read_from_file((uint8_t**)&ciphertext, &ciphertext_size, args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    if (args->verbose)
    {
        printf("[*] Reading key from: %s\n", args->key);
    }
    return_code = read_from_file(args->key, &key_data, &key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = deserialize_matrix(&encryption_matrix, args->dimension, key_data, key_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Generating decryption matrix...\n");

    return_code = generate_decryption_matrix(&decryption_matrix, args->dimension, encryption_matrix, DEFAULT_PRIME_GALOIS_FIELD);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    printf("[*] Decrypting...\n");
    
    return_code = decrypt(&decrypted_text, &decrypted_size, decryption_matrix, args->dimension, DEFAULT_PRIME_GALOIS_FIELD, ciphertext, ciphertext_size);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

	printf("[*] Decryption completed. Plaintext size: %u bytes\n", decrypted_size);
    if (args->verbose)
    {
        printf("[*] Writing plaintext to: %s\n", args->output_file);
    }
    return_code = write_to_file(args->output_file, decrypted_text, decrypted_size);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(key_data);
    free(ciphertext);
    free(decrypted_text);
    free_matrix(encryption_matrix, args->dimension);
    free_matrix(decryption_matrix, args->dimension);
    return return_code;
}


int main(int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    ParsedArguments parsed_args = { 0 };
    return_code = parse_arguments(&parsed_args, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        printf("[!] Failed to parse arguments.\n");
        return return_code;
    }

    if (parsed_args.verbose)
    {
        printf("[*] Verbose mode is enabled.\n");
    }

    switch (parsed_args.mode)
    {
    case KEY_GENERATION_MODE:
        return_code = handle_key_generation_mode(&parsed_args);
        break;
    case DECRYPTION_KEY_GENERATION_MODE:
        return_code = handle_decryption_key_generation_mode(&parsed_args);
        break;
    case ENCRYPT_MODE:
        return_code = handle_encrypt_mode(&parsed_args);
        break;
    case DECRYPT_MODE:
        return_code = handle_decrypt_mode(&parsed_args);
        break;
    case GENERATE_AND_ENCRYPT_MODE:
        return_code = handle_key_generation_mode(&parsed_args);
		if (STATUS_FAILED(return_code))
		{
			printf("[!] Failed to generate key for encryption.\n");
            goto cleanup;
		}
        return_code = handle_encrypt_mode(&parsed_args);
        break;
    case GENERATE_AND_DECRYPT_MODE:
        return_code = handle_generate_and_decrypt_mode(&parsed_args);
        break;
    default:
        printf("[!] Invalid mode specified.\n");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
    }

cleanup:
    return return_code;
}
