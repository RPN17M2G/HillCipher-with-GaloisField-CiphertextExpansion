#include "GaloisFieldHillCipher.h"

void print_matrix(int64_t** matrix, uint32_t dimension)
{
    for (size_t row = 0; row < dimension; ++row)
    {
        for (size_t column = 0; column < dimension; ++column)
        {
            printf("%ld ", matrix[row][column]);
        }
        printf("\n");
    }
}

STATUS_CODE write_to_file(const char* filename, const uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!filename || !data || (size == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    FILE* file = fopen(filename, "wb");
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
        goto cleanup;
    }
    fwrite(data, 1, size, file);
    fclose(file);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE read_from_file(uint8_t** out_data, uint32_t* out_size, const char* filename)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE* file = NULL;
    uint8_t* data = NULL;
    uint32_t size = 0;

    if (!out_data || !out_size || !filename)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    file = fopen(filename, "rb");
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_FILE_OPEN;
        goto cleanup;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = (uint8_t*)malloc(size);
    if (!data)
    {
        fclose(file);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    if (fread(data, 1, size, file) != size)
    {
        fclose(file);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    fclose(file);
    file = NULL;

    *out_data = data;
    *out_size = size;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(data);
    }
    return return_code;
}

STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    uint32_t size = 0;

    if (!out_data || !out_size || !matrix || (dimension == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    size = dimension * dimension * sizeof(int64_t);
    buffer = (uint8_t*)malloc(size);
    if (!buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (uint32_t row = 0; row < dimension; ++row)
    {
        memcpy(buffer + row * dimension * sizeof(int64_t), matrix[row], dimension * sizeof(int64_t));
    }

    *out_data = buffer;
    *out_size = size;

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(buffer);
    }
    return return_code;
}

STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** result = NULL;

    if (!out_matrix || !data || dimension == 0)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    uint32_t expected_size = dimension * dimension * sizeof(int64_t);
    if (size != expected_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_FILE_SIZE;
        goto cleanup;
    }

    result = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!result)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (uint32_t i = 0; i < dimension; ++i)
    {
        result[i] = (int64_t*)malloc(dimension * sizeof(int64_t));
        if (!result[i])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }
        memcpy(result[i], data + i * dimension * sizeof(int64_t), dimension * sizeof(int64_t));
    }

    *out_matrix = result;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (STATUS_FAILED(return_code) && result != NULL)
    {
        free_matrix(result, dimension);
    }
    return return_code;
}
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

    printf("[*] Encryption matrix generated:\n");
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

    printf("[*] Decryption matrix generated:\n");

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
