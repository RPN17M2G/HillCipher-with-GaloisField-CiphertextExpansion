#include "Cipher/CipherParts/Permutation.h"

STATUS_CODE permutate_uint8_vector(uint8_t** out_vector, uint8_t* vector, uint32_t vector_size, uint8_t* permutation_vector, uint32_t number_of_letters_per_element)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t group_index = 0, letter_index = 0;
    uint8_t permuted_index = 0;
    uint8_t* buffer = NULL;

    if ((NULL == out_vector) || (NULL == vector) || (NULL == permutation_vector) || (number_of_letters_per_element == 0))
    {
        log_error("Invalid arguments in permutate_uint8_vector: %s",
            !out_vector ? "out_vector is NULL" :
            !vector ? "vector is NULL" :
            !permutation_vector ? "permutation_vector is NULL" :
            "number_of_letters_per_element is 0");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if ((vector_size % number_of_letters_per_element) != 0)
    {
        log_error("Vector size (%u) is not divisible by number of letters per element (%u)",
                 vector_size, number_of_letters_per_element);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Starting permutation: vector_size=%u, letters_per_element=%u",
              vector_size, number_of_letters_per_element);

    buffer = (uint8_t*)malloc(vector_size + 1);
    if (NULL == buffer)
    {
        log_error("Memory allocation failed for permutation buffer (size: %u bytes)",
                 vector_size + 1);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (group_index = 0; group_index < vector_size; group_index += number_of_letters_per_element)
    {
        log_debug("Processing group at index %zu", group_index);
        for (letter_index = 0; letter_index < number_of_letters_per_element; ++letter_index)
        {
            permuted_index = permutation_vector[letter_index];
            if (permuted_index >= number_of_letters_per_element)
            {
                log_error("Invalid permutation index: %u >= %u",
                         permuted_index, number_of_letters_per_element);
                return_code = STATUS_CODE_INVALID_ARGUMENT;
                goto cleanup;
            }

            buffer[group_index + letter_index] = vector[group_index + permuted_index];
            log_debug("Permuted: letter[%zu] -> position %zu (char '%c' -> '%c')",
                     permuted_index, letter_index,
                     vector[group_index + permuted_index],
                     buffer[group_index + letter_index]);
        }
    }
    buffer[vector_size] = '\0';
    log_debug("Permutation completed successfully");

    *out_vector = buffer;
    buffer = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(buffer);
    return return_code;
}
