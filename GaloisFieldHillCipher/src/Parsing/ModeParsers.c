#include "Parsing/ModeParsers.h"

STATUS_CODE parse_key_generation_arguments(KeyGenerationArguments** out_arguments, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* output_file = NULL;
    uint32_t dimension = 0;
    uint32_t number_of_error_vectors = 0;
    uint32_t prime_field = DEFAULT_VALUE_OF_GALOIS_FIELD;
    uint32_t number_of_random_bits_to_add = DEFAULT_VALUE_OF_NUMBER_OF_RANDOM_BITS_TO_ADD;
    uint32_t number_of_letters_for_each_digit_ascii_mapping = 0;
    KeyGenerationArguments* parsed_arguments = NULL;

    struct argparse_option options[] = {
        OPT_STRING(*FLAG_OUTPUT_FILE_SHORT, FLAG_OUTPUT_FILE, &output_file, FLAG_OUTPUT_FILE_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_DIMENSION_SHORT, FLAG_DIMENSION, &dimension, FLAG_DIMENSION_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_ERROR_VECTORS_SHORT, FLAG_ERROR_VECTORS, &number_of_error_vectors, FLAG_ERROR_VECTORS_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_PRIME_FIELD_SHORT, FLAG_PRIME_FIELD, &prime_field, FLAG_PRIME_FIELD_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_RANDOM_BITS_SHORT, FLAG_RANDOM_BITS, &number_of_random_bits_to_add, FLAG_RANDOM_BITS_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_ASCII_MAPPING_LETTERS_SHORT, FLAG_ASCII_MAPPING_LETTERS, &number_of_letters_for_each_digit_ascii_mapping, FLAG_ASCII_MAPPING_LETTERS_DESCRIPTION, 0, 0),
        OPT_END(),
    };

    if (!out_arguments || !argv) 
    {
        log_error("[!] Invalid argument: out_arguments or argv is NULL in parse_key_generation_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code)) 
    {
        log_error("[!] Failed to parse arguments for KEY_GENERATION_MODE.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (!output_file || STATUS_FAILED(validate_file_is_writeable(output_file)) || (0 == dimension) ||
        (0 == number_of_error_vectors) || (0 == number_of_letters_for_each_digit_ascii_mapping) || (0 == prime_field))
    {
        log_error("[!] Invalid arguments for KEY_GENERATION_MODE.");
        fprintf(stderr, "%s", USAGE_KEY_GENERATION_MODE);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    parsed_arguments = malloc(sizeof(KeyGenerationArguments));
    if (!parsed_arguments) 
    {
        log_error("[!] Memory allocation failed for KeyGenerationArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    parsed_arguments->output_file = output_file;
    parsed_arguments->dimension = dimension;
    parsed_arguments->number_of_error_vectors = number_of_error_vectors;
    parsed_arguments->prime_field = prime_field;
    parsed_arguments->number_of_random_bits_to_add = number_of_random_bits_to_add;
    parsed_arguments->number_of_letters_for_each_digit_ascii_mapping = number_of_letters_for_each_digit_ascii_mapping;

    *out_arguments = parsed_arguments;
    parsed_arguments = NULL;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(parsed_arguments);
    return return_code;
}

STATUS_CODE parse_decryption_key_generation_arguments(DecryptionKeyGenerationArguments** out_arguments, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* key = NULL;
    const char* output_file = NULL;
    DecryptionKeyGenerationArguments* parsed_arguments = NULL;

    struct argparse_option options[] = {
        OPT_STRING(*FLAG_KEY_FILE_SHORT, FLAG_KEY_FILE, &key, FLAG_KEY_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_OUTPUT_FILE_SHORT, FLAG_OUTPUT_FILE, &output_file, FLAG_OUTPUT_FILE_DESCRIPTION, 0, 0),
        OPT_END(),
    };

    if (!out_arguments || !argv) 
    {
        log_error("[!] Invalid argument: out_arguments or argv is NULL in parse_decryption_key_generation_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code)) 
    {
        log_error("[!] Failed to parse arguments for DECRYPTION_KEY_GENERATION_MODE.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (!key || !output_file || STATUS_FAILED(validate_file_is_writeable(output_file)) ||
        STATUS_FAILED(validate_file_is_readable(key)) || STATUS_FAILED(validate_file_is_binary(key)))
    {
        log_error("[!] Invalid arguments for DECRYPTION_KEY_GENERATION_MODE.");
        fprintf(stderr, "%s", USAGE_DECRYPTION_KEY_GENERATION_MODE);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    parsed_arguments = malloc(sizeof(DecryptionKeyGenerationArguments));
    if (!parsed_arguments) 
    {
        log_error("[!] Memory allocation failed for DecryptionKeyGenerationArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    parsed_arguments->key = key;
    parsed_arguments->output_file = output_file;

    *out_arguments = parsed_arguments;
    parsed_arguments = NULL;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(parsed_arguments);
    return return_code;
}

STATUS_CODE parse_decrypt_arguments(DecryptArguments** out_arguments, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* input_file = NULL;
    const char* output_file = NULL;
    const char* key = NULL;
    DecryptArguments* parsed_arguments = NULL;

    struct argparse_option options[] = {
        OPT_STRING(*FLAG_INPUT_FILE_SHORT, FLAG_INPUT_FILE, &input_file, FLAG_INPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_OUTPUT_FILE_SHORT, FLAG_OUTPUT_FILE, &output_file, FLAG_OUTPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_KEY_FILE_SHORT, FLAG_KEY_FILE, &key, FLAG_KEY_FILE_DESCRIPTION, 0, 0),
        OPT_END(),
    };

    if (!out_arguments || !argv || STATUS_FAILED(validate_file_is_writeable(output_file)) ||
        STATUS_FAILED(validate_file_is_readable(key)) || STATUS_FAILED(validate_file_is_binary(key)) ||
        STATUS_FAILED(validate_file_is_readable(input_file)))
    {
        log_error("[!] Invalid argument: out_arguments or argv is NULL in parse_decrypt_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code)) 
    {
        log_error("[!] Failed to parse arguments for DECRYPT_MODE.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (!input_file || !output_file || !key) 
    {
        log_error("[!] Invalid arguments for DECRYPT_MODE.");
        fprintf(stderr, "%s", USAGE_DECRYPT_MODE);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    parsed_arguments = malloc(sizeof(DecryptArguments));
    if (!parsed_arguments) 
    {
        log_error("[!] Memory allocation failed for DecryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    parsed_arguments->input_file = input_file;
    parsed_arguments->output_file = output_file;
    parsed_arguments->key = key;

    *out_arguments = parsed_arguments;
    parsed_arguments = NULL;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(parsed_arguments);
    return return_code;
}

STATUS_CODE parse_encrypt_arguments(EncryptArguments** out_arguments, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* input_file = NULL;
    const char* output_file = NULL;
    const char* key = NULL;
    EncryptArguments* parsed_arguments = NULL;

    struct argparse_option options[] = {
        OPT_STRING(*FLAG_INPUT_FILE_SHORT, FLAG_INPUT_FILE, &input_file, FLAG_INPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_OUTPUT_FILE_SHORT, FLAG_OUTPUT_FILE, &output_file, FLAG_OUTPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_KEY_FILE_SHORT, FLAG_KEY_FILE, &key, FLAG_KEY_FILE_DESCRIPTION, 0, 0),
        OPT_END(),
    };

    if (!out_arguments || !argv || STATUS_FAILED(validate_file_is_writeable(output_file)) ||
        STATUS_FAILED(validate_file_is_readable(key)) || STATUS_FAILED(validate_file_is_binary(key)) ||
        STATUS_FAILED(validate_file_is_readable(input_file)))
    {
        log_error("[!] Invalid argument: out_arguments or argv is NULL in parse_encrypt_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code)) 
    {
        log_error("[!] Failed to parse arguments for ENCRYPT_MODE.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (!input_file || !output_file || !key) 
    {
        log_error("[!] Invalid arguments for ENCRYPT_MODE.");
        fprintf(stderr, "%s", USAGE_ENCRYPT_MODE);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    parsed_arguments = malloc(sizeof(EncryptArguments));
    if (!parsed_arguments) 
    {
        log_error("[!] Memory allocation failed for EncryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    parsed_arguments->input_file = input_file;
    parsed_arguments->output_file = output_file;
    parsed_arguments->key = key;

    *out_arguments = parsed_arguments;
    parsed_arguments = NULL;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(parsed_arguments);
    return return_code;
}

STATUS_CODE parse_generate_and_encrypt_arguments(GenerateAndEncryptArguments** out_arguments, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* input_file = NULL;
    const char* output_file = NULL;
    const char* key_file = NULL;
    uint32_t dimension = 0;
    uint32_t number_of_error_vectors = 0;
    uint32_t prime_field = DEFAULT_VALUE_OF_GALOIS_FIELD;
    uint32_t number_of_random_bits_to_add = DEFAULT_VALUE_OF_NUMBER_OF_RANDOM_BITS_TO_ADD;
    uint32_t number_of_letters_for_each_digit_ascii_mapping = 0;
    GenerateAndEncryptArguments* parsed_arguments = NULL;
    EncryptArguments* encrypt_arguments = NULL;
    KeyGenerationArguments* key_arguments = NULL;

    struct argparse_option options[] = {
        OPT_STRING(*FLAG_INPUT_FILE_SHORT, FLAG_INPUT_FILE, &input_file, FLAG_INPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_OUTPUT_FILE_SHORT, FLAG_OUTPUT_FILE, &output_file, FLAG_OUTPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_KEY_FILE_SHORT, FLAG_KEY_FILE, &key_file, FLAG_KEY_FILE_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_DIMENSION_SHORT, FLAG_DIMENSION, &dimension, FLAG_DIMENSION_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_ERROR_VECTORS_SHORT, FLAG_ERROR_VECTORS, &number_of_error_vectors, FLAG_ERROR_VECTORS_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_PRIME_FIELD_SHORT, FLAG_PRIME_FIELD, &prime_field, FLAG_PRIME_FIELD_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_RANDOM_BITS_SHORT, FLAG_RANDOM_BITS, &number_of_random_bits_to_add, FLAG_RANDOM_BITS_DESCRIPTION, 0, 0),
        OPT_INTEGER(*FLAG_ASCII_MAPPING_LETTERS_SHORT, FLAG_ASCII_MAPPING_LETTERS, &number_of_letters_for_each_digit_ascii_mapping, FLAG_ASCII_MAPPING_LETTERS_DESCRIPTION, 0, 0),
        OPT_END(),
    };

    if (!out_arguments || !argv) 
    {
        log_error("[!] Invalid argument: out_arguments or argv is NULL in parse_generate_and_encrypt_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code)) 
    {
        log_error("[!] Failed to parse arguments for GENERATE_AND_ENCRYPT_MODE.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (!input_file || !output_file || STATUS_FAILED(validate_file_is_writeable(output_file)) ||
        STATUS_FAILED(validate_file_is_binary(key_file)) || STATUS_FAILED(validate_file_is_writeable(key_file)) ||
        STATUS_FAILED(validate_file_is_readable(input_file)) || (0 == dimension) ||
        (0 == number_of_error_vectors) || (0 == number_of_letters_for_each_digit_ascii_mapping) || (0 == prime_field))
    {
        log_error("[!] Invalid arguments for GENERATE_AND_ENCRYPT_MODE.");
        fprintf(stderr, "%s", USAGE_GENERATE_AND_ENCRYPT_MODE);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    parsed_arguments = malloc(sizeof(GenerateAndEncryptArguments));
    if (!parsed_arguments) 
    {
        log_error("[!] Memory allocation failed for GenerateAndEncryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }
    encrypt_arguments = malloc(sizeof(EncryptArguments));
    if (!encrypt_arguments) 
    {
        log_error("[!] Memory allocation failed for EncryptArguments in GenerateAndEncryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }
    key_arguments = malloc(sizeof(KeyGenerationArguments));
    if (!key_arguments) 
    {
        log_error("[!] Memory allocation failed for KeyGenerationArguments in GenerateAndEncryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    parsed_arguments->encrypt_arguments = encrypt_arguments;
    encrypt_arguments = NULL;
    parsed_arguments->key_generation_arguments = key_arguments;
    key_arguments = NULL;

    parsed_arguments->encrypt_arguments->input_file = input_file;
    parsed_arguments->encrypt_arguments->output_file = output_file;
    parsed_arguments->encrypt_arguments->key = key_file;
    parsed_arguments->key_generation_arguments->output_file = _strdup(key_file);
    parsed_arguments->key_generation_arguments->dimension = dimension;
    parsed_arguments->key_generation_arguments->number_of_error_vectors = number_of_error_vectors;
    parsed_arguments->key_generation_arguments->prime_field = prime_field;
    parsed_arguments->key_generation_arguments->number_of_random_bits_to_add = number_of_random_bits_to_add;
    parsed_arguments->key_generation_arguments->number_of_letters_for_each_digit_ascii_mapping = number_of_letters_for_each_digit_ascii_mapping;

    *out_arguments = parsed_arguments;
    parsed_arguments = NULL;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(encrypt_arguments);
    free(key_arguments);
    free(parsed_arguments);
    return return_code;
}

STATUS_CODE parse_generate_and_decrypt_arguments(GenerateAndDecryptArguments** out_arguments, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* input_file = NULL;
    const char* output_file = NULL;
    const char* encryption_key_file = NULL;
    const char* decryption_key_output_file = NULL;
    GenerateAndDecryptArguments* parsed_arguments = NULL;
    DecryptArguments* decrypt_arguments = NULL;
    DecryptionKeyGenerationArguments* key_generation_arguments = NULL;

    struct argparse_option options[] = {
        OPT_STRING(*FLAG_INPUT_FILE_SHORT, FLAG_INPUT_FILE, &input_file, FLAG_INPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_OUTPUT_FILE_SHORT, FLAG_OUTPUT_FILE, &output_file, FLAG_OUTPUT_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_KEY_FILE_SHORT, FLAG_KEY_FILE, &encryption_key_file, FLAG_KEY_FILE_DESCRIPTION, 0, 0),
        OPT_STRING(*FLAG_DECRYPTION_KEY_OUTPUT_FILE_SHORT, FLAG_DECRYPTION_KEY_OUTPUT_FILE, &decryption_key_output_file, FLAG_DECRYPTION_KEY_OUTPUT_FILE_DESCRIPTION, 0, 0),
        OPT_END(),
    };

    if (!out_arguments || !argv) 
    {
        log_error("[!] Invalid argument: out_arguments or argv is NULL in parse_generate_and_decrypt_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code)) 
    {
        log_error("[!] Failed to parse arguments for GENERATE_AND_DECRYPT_MODE.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (!output_file || STATUS_FAILED(validate_file_is_writeable(output_file)) ||
        STATUS_FAILED(validate_file_is_readable(encryption_key_file)) || STATUS_FAILED(validate_file_is_binary(encryption_key_file)) ||
        STATUS_FAILED(validate_file_is_readable(input_file)) || !decryption_key_output_file ||
        STATUS_FAILED(validate_file_is_writeable(decryption_key_output_file)) ||
        STATUS_FAILED(validate_file_is_binary(decryption_key_output_file)))
    {
        log_error("[!] Invalid arguments for GENERATE_AND_DECRYPT_MODE.");
        fprintf(stderr, "%s", USAGE_GENERATE_AND_DECRYPT_MODE);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    parsed_arguments = malloc(sizeof(GenerateAndDecryptArguments));
    if (!parsed_arguments) 
    {
        log_error("[!] Memory allocation failed for GenerateAndDecryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    decrypt_arguments = malloc(sizeof(DecryptArguments));
    if (!decrypt_arguments) 
    {
        log_error("[!] Memory allocation failed for DecryptArguments in GenerateAndDecryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    key_generation_arguments = malloc(sizeof(DecryptionKeyGenerationArguments));
    if (!key_generation_arguments) 
    {
        log_error("[!] Memory allocation failed for DecryptionKeyGenerationArguments in GenerateAndDecryptArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    parsed_arguments->key_generation_arguments = key_generation_arguments;
    key_generation_arguments = NULL;
    parsed_arguments->decrypt_arguments = decrypt_arguments;
    decrypt_arguments = NULL;

    parsed_arguments->decrypt_arguments->input_file = input_file;
    parsed_arguments->decrypt_arguments->output_file = output_file;
    parsed_arguments->decrypt_arguments->key = _strdup(decryption_key_output_file);
    parsed_arguments->key_generation_arguments->key = encryption_key_file;
    parsed_arguments->key_generation_arguments->output_file = decryption_key_output_file;

    *out_arguments = parsed_arguments;
    parsed_arguments = NULL;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(key_generation_arguments);
    free(decrypt_arguments);
    free(parsed_arguments);
    return return_code;
}
