#include "Secrets/SecretsGeneration.h"

STATUS_CODE generate_ascii_mapping(uint8_t*** out_mapping, uint32_t letters_per_digit, uint32_t base)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t *ascii_pool = NULL;
    uint8_t** mapping = NULL;
    uint32_t pool_index = 0;

    uint32_t printable_range = MAXIMUM_ASCII_PRINTABLE_CHARACTER - MINIMUM_ASCII_PRINTABLE_CHARACTER + 1;

    if ((NULL == out_mapping) || (letters_per_digit == 0) || (base == 0) ||
        (base > (printable_range / letters_per_digit)))
    {
        log_error("Invalid arguments in generate_ascii_mapping: %s",
                  !out_mapping ? "out_mapping is NULL" :
                  letters_per_digit == 0 ? "letters_per_digit is 0" :
                  base == 0 ? "base is 0" :
                  base > (printable_range / letters_per_digit) ? "letters_per_digit multiplied by base exceeds number of printable ASCII characters"
                  : "unknown error");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    ascii_pool = (uint8_t*)malloc(printable_range);
    if (!ascii_pool)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (uint32_t character_index = 0; character_index < printable_range; ++character_index)
    {
        ascii_pool[character_index] = (uint8_t)(MINIMUM_ASCII_PRINTABLE_CHARACTER + character_index);
    }

    return_code = secure_fisher_yates_shuffle(ascii_pool, printable_range);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    mapping = (uint8_t**)malloc(base * sizeof(uint8_t*));
    if (!mapping)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    pool_index = 0;
    for (uint32_t digit = 0; digit < base; ++digit)
    {
        mapping[digit] = (uint8_t*)malloc(letters_per_digit);
        if (!mapping[digit])
        {
            log_error("Memory allocation failed for ASCII out_mapping digit %zu", digit);
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }
        for (uint32_t variant = 0; variant < letters_per_digit; ++variant)
        {
            mapping[digit][variant] = ascii_pool[pool_index++];
            log_debug("ASCII mapping: digit %zu -> '%c'", digit, mapping[digit][variant]);
        }
    }

    *out_mapping = mapping;
    mapping = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (mapping)
    {
        for (uint32_t digit_number = 0; digit_number < base; ++digit_number)
        {
            if ((mapping)[digit_number])
            {
                free((mapping)[digit_number]);
            }
        }
        free(mapping);
        mapping = NULL;
    }
    free(ascii_pool);
    return return_code;
}

STATUS_CODE generate_permutation_vector(uint8_t** out_permutation_vector, uint32_t length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* permutation_vector = NULL;

    permutation_vector = (uint8_t*)malloc(length);
    if (!permutation_vector)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (uint32_t index = 0; index < length; ++index)
    {
        permutation_vector[index] = (uint8_t)index;
    }

    return_code = secure_fisher_yates_shuffle(permutation_vector, length);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = STATUS_CODE_SUCCESS;
    *out_permutation_vector = permutation_vector;
    permutation_vector = NULL;

cleanup:
    free(permutation_vector);
    return return_code;
}

STATUS_CODE generate_encryption_matrix(int64_t*** out_matrix, uint32_t dimension, uint32_t prime_field, uint32_t max_attempts)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    bool matrix_invertible = false;
    uint32_t attempt_number = 0;
    size_t row = 0;
    int64_t** matrix_buffer = NULL;

    if (NULL == out_matrix)
    {
        log_error("Invalid argument: out_matrix is NULL in generate_encryption_matrix");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Generating encryption matrix: dimension=%u, prime_field=%u", dimension, prime_field);

    while (++attempt_number < max_attempts)
    {
        return_code = generate_square_matrix_over_field(&matrix_buffer, dimension, prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_debug("Failed to generate square matrix on attempt %u", attempt_number);
            continue;
        }

        return_code = is_matrix_invertible(&matrix_invertible, matrix_buffer, dimension, prime_field);
        if (STATUS_FAILED(return_code) || !matrix_invertible)
        {
            log_debug("Generated matrix not invertible on attempt %u", attempt_number);
            for (row = 0; row < dimension; ++row)
            {
                free((matrix_buffer)[row]);
                (matrix_buffer)[row] = NULL;
            }
            free(matrix_buffer);
            matrix_buffer = NULL;
        }
        else if (matrix_invertible)
        {
            log_debug("Found invertible matrix on attempt %u", attempt_number);
            break;
        }
    }

    if (attempt_number >= max_attempts)
    {
        log_error("Failed to generate invertible matrix after %u attempts", max_attempts);
        return_code = STATUS_CODE_MATRIX_NOT_INVERTIBLE;
        goto cleanup;
    }

    *out_matrix = matrix_buffer;
    matrix_buffer = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    (void)free_int64_matrix(matrix_buffer, dimension);
    return return_code;
}

STATUS_CODE generate_decryption_matrix(int64_t*** out_matrix, uint32_t dimension, int64_t** encryption_matrix, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!out_matrix || !encryption_matrix)
    {
        log_error("Invalid arguments in generate_decryption_matrix");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Generating decryption matrix: dimension=%u, prime_field=%u", dimension, prime_field);

    return_code = inverse_square_matrix_gauss_jordan(out_matrix, encryption_matrix, dimension, prime_field);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to generate inverse matrix using Gauss-Jordan elimination");
        goto cleanup;
    }

    log_debug("Successfully generated decryption matrix");
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    return return_code;
}

STATUS_CODE build_encryption_secrets(Secrets** out_secrets, const KeyGenerationArguments *args)
{
    STATUS_CODE return_code = STATUS_CODE_SUCCESS;
    Secrets* secrets = NULL;
    int64_t **encryption_matrix = NULL;
    int64_t number_of_digits_per_field_element = 0;

    if ((NULL == out_secrets) || (NULL == args))
    {
        log_error("Invalid arguments in build_encryption_secrets");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    number_of_digits_per_field_element = calculate_digits_per_element(args->prime_field);

    secrets = (Secrets*)malloc(sizeof(Secrets));
    if (NULL == secrets)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    log_info("Starting key generation with parameters: dimension=%u, prime_field=%u, error_vectors=%u",
         args->dimension, args->prime_field, args->number_of_error_vectors);

    log_debug("Generating encryption matrix...");
    return_code = generate_encryption_matrix(&encryption_matrix,
                                             args->dimension,
                                             args->prime_field,
                                             3);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to generate encryption matrix");
        goto cleanup;
    }
    log_info("Encryption matrix generated.");

    secrets->key_matrix = encryption_matrix;
    secrets->dimension = args->dimension;
    secrets->prime_field = args->prime_field;
    secrets->number_of_error_vectors = args->number_of_error_vectors;

    if (args->number_of_error_vectors > 0) {
        return_code = generate_matrix_over_field(&secrets->error_vectors,
                                                 args->number_of_error_vectors,
                                                 args->dimension,
                                                 args->prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_error("Failed to generate error vectors");
            goto cleanup;
        }
    }

    return_code = generate_ascii_mapping(&secrets->ascii_mapping, args->number_of_letters_for_each_digit_ascii_mapping, DECIMAL_BASE);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    secrets->number_of_letters_for_each_digit_ascii_mapping = args->number_of_letters_for_each_digit_ascii_mapping;
    log_info("Successfully generated ASCII mapping");

    return_code = generate_permutation_vector(&secrets->permutation_vector,
                                              number_of_digits_per_field_element);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    log_info("Successfully generated permutation vector");

    secrets->number_of_random_bits_to_add = args->number_of_random_bits_to_add;

    *out_secrets = secrets;
    secrets = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (STATUS_FAILED(return_code))
    {
        free_secrets(secrets);
        free(secrets);
    }
    return return_code;
}

STATUS_CODE build_decryption_secrets(Secrets** out_secrets, Secrets* encryption_secrets)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    Secrets* decryption_secrets = NULL;
    int64_t** decryption_matrix = NULL;
    uint8_t* reversed_permutation_vector = NULL;
    uint32_t permutation_size = 0, index = 0;

    if ((NULL == out_secrets) || (NULL == encryption_secrets) ||
        (NULL == encryption_secrets->key_matrix) || (NULL == encryption_secrets->error_vectors))
    {
        log_error("Invalid arguments in build_decryption_secrets");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    decryption_secrets = (Secrets*)malloc(sizeof(Secrets));
    if (NULL == decryption_secrets)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    log_info("Building decryption secrets from encryption secrets...");

    log_info("Generating decryption matrix...\n");
    return_code = generate_decryption_matrix(&decryption_matrix, encryption_secrets->dimension,
                                           encryption_secrets->key_matrix, encryption_secrets->prime_field);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to generate decryption matrix");
        goto cleanup;
    }
    log_matrix(decryption_matrix, encryption_secrets->dimension, "Decryption matrix generated:\n", true);

    log_info("Reversing permutation vector...");
    if (encryption_secrets->permutation_vector)
    {
        permutation_size = calculate_digits_per_element(encryption_secrets->prime_field);
        reversed_permutation_vector = (uint8_t*)malloc(permutation_size * sizeof(uint8_t));
        if (!reversed_permutation_vector)
        {
            log_error("[!] Memory allocation failed for reversed permutation vector.");
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (index = 0; index < permutation_size; ++index)
        {
            reversed_permutation_vector[encryption_secrets->permutation_vector[index]] = (uint8_t)index;
        }
    }

    decryption_secrets->dimension = encryption_secrets->dimension;
    decryption_secrets->number_of_error_vectors = encryption_secrets->number_of_error_vectors;
    decryption_secrets->prime_field = encryption_secrets->prime_field;
    decryption_secrets->key_matrix = decryption_matrix;
    decryption_secrets->error_vectors = encryption_secrets->error_vectors;
    encryption_secrets->error_vectors = NULL;
    decryption_secrets->ascii_mapping = encryption_secrets->ascii_mapping;
    encryption_secrets->ascii_mapping = NULL;
    decryption_secrets->permutation_vector = reversed_permutation_vector;
    decryption_secrets->number_of_letters_for_each_digit_ascii_mapping = encryption_secrets->number_of_letters_for_each_digit_ascii_mapping;
    decryption_secrets->number_of_random_bits_to_add = encryption_secrets->number_of_random_bits_to_add;

    return_code = STATUS_CODE_SUCCESS;
    *out_secrets = decryption_secrets;
    decryption_secrets = NULL;

cleanup:
    if (STATUS_FAILED(return_code))
    {
        free_secrets(decryption_secrets);
        free(decryption_secrets);
    }
    return return_code;
}

void free_secrets(Secrets* secrets)
{
    size_t index = 0;

    if (NULL == secrets)
    {
        return;
    }

    if (secrets->key_matrix != NULL)
    {
        for (index = 0; index < secrets->dimension; ++index)
        {
            free(secrets->key_matrix[index]);
        }
    }
    free(secrets->key_matrix);
    if (secrets->error_vectors != NULL)
    {
        for (index = 0; index < secrets->number_of_error_vectors; ++index)
        {
            free(secrets->error_vectors[index]);
        }
    }
    free(secrets->error_vectors);
    if (secrets->ascii_mapping != NULL)
    {
        for (index = 0; index < NUMBER_OF_DIGITS; ++index)
        {
            free(secrets->ascii_mapping[index]);
        }
    }
    free(secrets->ascii_mapping);
    free(secrets->permutation_vector);
}
