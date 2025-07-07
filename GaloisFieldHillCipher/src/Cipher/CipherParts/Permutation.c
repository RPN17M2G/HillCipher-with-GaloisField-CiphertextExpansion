#include "Cipher/CipherParts/Permutation.h"

STATUS_CODE permutate_uint8_vector(uint8_t** out_vector, uint8_t* vector, uint32_t vector_size, uint8_t* permutation_vector, uint32_t number_of_letters_per_element)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t group_index = 0, letter_index = 0;
    uint8_t permuted_index = 0;
    uint8_t* buffer = NULL;

    if ((NULL == out_vector) || (NULL == vector) || (NULL == permutation_vector) || (number_of_letters_per_element == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if ((vector_size % number_of_letters_per_element) != 0)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    buffer = (uint8_t*)malloc(vector_size + 1);
    if (NULL == buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (group_index = 0; group_index < vector_size; group_index += number_of_letters_per_element)
    {
        for (letter_index = 0; letter_index < number_of_letters_per_element; ++letter_index)
        {
            permuted_index = permutation_vector[letter_index];
            if (permuted_index >= number_of_letters_per_element)
            {
                return_code = STATUS_CODE_INVALID_ARGUMENT;
                goto cleanup;
            }

            buffer[group_index + letter_index] = vector[group_index + permuted_index];
        }
    }
    buffer[vector_size] = '\0';

    *out_vector = buffer;
    buffer = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(buffer);
    return return_code;
}
