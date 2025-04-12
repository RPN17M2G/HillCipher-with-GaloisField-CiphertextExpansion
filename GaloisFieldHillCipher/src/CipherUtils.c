#include "CipherUtils.h"

STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    uint32_t output_byte = 0;
    uint8_t current_working_byte = 0;

    if ((NULL == out) || (NULL == out_bit_size))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    // Calculate the size of the output vector
    uint32_t number_of_random_bits = NUMBER_OF_RANDOM_BITS_TO_ADD * (value_bit_length / BYTE_SIZE);
    uint32_t total_bits = value_bit_length + number_of_random_bits;
    total_bits = total_bits + (BYTE_SIZE - (total_bits % BYTE_SIZE)); // Round up to the nearest up byte
    uint32_t total_bytes = (total_bits / BYTE_SIZE);

    *out = (uint8_t*)malloc(total_bytes);
    *out_bit_size = total_bits;
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(*out, 0, total_bytes);

    uint32_t output_bit = 0;
    uint32_t random_bits = 0;
    for (uint32_t bit_number = 0; bit_number < value_bit_length; ++bit_number)
    {
        return_code = generate_secure_random_number(&random_bits, (uint32_t)0, (uint32_t)TWO_BITS_MAX_VALUE);
		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

        uint8_t masked_random_bits = random_bits & TWO_BITS_MASK; // TODO: Add random bits

        if ((output_bit % BYTE_SIZE == 0) && (output_bit != 0))
        {
            ++output_byte;
        }

        if (bit_number % (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD) < BYTE_SIZE)
        {
            current_working_byte = value[bit_number / BYTE_SIZE];
        }
        else
        {
            current_working_byte = random_bits;
        }

        if (IS_BIT_SET(current_working_byte, bit_number % BYTE_SIZE))
        {
            (*out)[output_byte] = SET_BIT((*out)[output_byte], output_bit);
        }
        else
        {
            (*out)[output_byte] = CLEAR_BIT((*out)[output_byte], output_bit);
        }

        ++output_bit;
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out) && (NULL != *out))
    {
        free(*out);
        *out = NULL;
        if (NULL != out_bit_size)
            *out_bit_size = 0;
    }
    return return_code;
}

STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if ((NULL == out) || (NULL == out_bit_size) || (NULL == value))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *out = (uint8_t*)malloc((value_bit_length / (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD)) * BYTE_SIZE + 1);
    *out_bit_size = (value_bit_length / (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD)) * BYTE_SIZE;
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(*out, 0, value_bit_length / (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD) * BYTE_SIZE);

    uint32_t output_byte = 0;
    uint32_t output_bit = 0;
    uint8_t random_bits_counter = 0;

    for (uint32_t bit_number = 0; bit_number < value_bit_length; ++bit_number)
    {
        // Skip the random bits
        if ((output_bit % BYTE_SIZE == 0) && (output_bit != 0))
        {
            if (random_bits_counter != NUMBER_OF_RANDOM_BITS_TO_ADD)
            {
                ++random_bits_counter;
                continue;
            }
            random_bits_counter = 0;
        }

        if (IS_BIT_SET(value[bit_number / BYTE_SIZE], bit_number % BYTE_SIZE))
        {
            (*out)[output_byte] = SET_BIT((*out)[output_byte], output_bit);
        }
        else
        {
            (*out)[output_byte] = CLEAR_BIT((*out)[output_byte], output_bit);
        }

        ++output_bit;
        if (output_bit % BYTE_SIZE == 0)
        {
            ++output_byte;
        }
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out) && (NULL != *out))
    {
        free(*out);
        *out = NULL;
        if (NULL != out_bit_size)
            *out_bit_size = 0;
    }
    return return_code;
}

STATUS_CODE pad_to_length(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length, uint32_t target_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if ((NULL == out) || (NULL == value))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (value_bit_length > target_bit_length)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (target_bit_length == value_bit_length)
    {
        target_bit_length += block_bit_size;
    }

    *out = (uint8_t*)malloc(target_bit_length / BYTE_SIZE);
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    // Copy the original value to the output array
    memcpy_s(*out, target_bit_length / BYTE_SIZE, value, value_bit_length / BYTE_SIZE);

    // Pad the remaining bytes with 0
    memset(*out + (value_bit_length / BYTE_SIZE), 0, (target_bit_length - value_bit_length) / BYTE_SIZE);
    *(*out + (value_bit_length / BYTE_SIZE)) = PADDING_MAGIC;

    *out_bit_length = target_bit_length;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out) && (NULL != *out) && (NULL != out_bit_length))
    {
        free(*out);
        *out = NULL;
        *out_bit_length = 0;
    }
    return return_code;
}

STATUS_CODE remove_padding(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if ((NULL == out) || (NULL == value) || (NULL == out_bit_length))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

	for (uint16_t byte_number = 0; byte_number < value_bit_length / BYTE_SIZE; ++byte_number)
	{
		if (value[byte_number] == PADDING_MAGIC)
		{
			*out_bit_length = (byte_number * BYTE_SIZE);
			break;
		}
	}

    *out = (uint8_t*)malloc(*out_bit_length / BYTE_SIZE);
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    // Copy the original value to the output array
    memcpy_s(*out, *out_bit_length / BYTE_SIZE, value, value_bit_length / BYTE_SIZE);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out) && (NULL != *out) && (NULL != out_bit_length))
    {
        free(*out);
        *out = NULL;
        *out_bit_length = 0;
    }
    return return_code;
}

STATUS_CODE divide_uint8_t_into_blocks(uint8_t*** out_blocks, uint32_t* num_blocks, uint8_t* value, uint32_t value_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if ((NULL == out_blocks) || (NULL == num_blocks) || (NULL == value) || (value_bit_length % block_bit_size != 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (block_bit_size == 0)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *num_blocks = value_bit_length / block_bit_size;
    *out_blocks = (uint8_t**)malloc(*num_blocks * sizeof(*out_blocks));
    if (NULL == *out_blocks)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (uint32_t block_number = 0; block_number < *num_blocks; ++block_number)
    {
        (*out_blocks)[block_number] = (uint8_t*)malloc(block_bit_size / BYTE_SIZE);
        if (NULL == (*out_blocks)[block_number])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        // Copy the block data
        memcpy((*out_blocks)[block_number], value + (block_number * block_bit_size / BYTE_SIZE), block_bit_size / BYTE_SIZE);
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out_blocks) && (NULL != *out_blocks) && (NULL != num_blocks))
    {
        for (uint32_t i = 0; i < *num_blocks; ++i)
        {
            if ((*out_blocks)[i] != NULL)
            {
                free((*out_blocks)[i]);
            }
        }
        free(*out_blocks);
        *out_blocks = NULL;
        *num_blocks = 0;
    }
    return return_code;
}

STATUS_CODE divide_double_into_blocks(long double*** out_blocks, uint32_t* num_blocks, long double* value, uint32_t value_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if ((NULL == out_blocks) 
        || (NULL == num_blocks) 
        || (NULL == value)
        || (value_bit_length % block_bit_size != 0)
        || (sizeof(long double) % block_bit_size != 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (block_bit_size == 0)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *num_blocks = value_bit_length / block_bit_size;
    *out_blocks = (long double**)malloc(*num_blocks * sizeof(*out_blocks));
    if (NULL == *out_blocks)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (uint32_t block_number = 0; block_number < *num_blocks; ++block_number)
    {
        (*out_blocks)[block_number] = (long double*)malloc(block_bit_size / BYTE_SIZE);
        if (NULL == (*out_blocks)[block_number])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        // Copy the block data
        memcpy((*out_blocks)[block_number], value + (block_number * block_bit_size / BYTE_SIZE), block_bit_size / BYTE_SIZE);
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out_blocks) && (NULL != *out_blocks) && (NULL != num_blocks))
    {
        for (uint32_t i = 0; i < *num_blocks; ++i)
        {
            if ((*out_blocks)[i] != NULL)
            {
                free((*out_blocks)[i]);
            }
        }
        free(*out_blocks);
        *out_blocks = NULL;
        *num_blocks = 0;
    }
    return return_code;
}

STATUS_CODE generate_encryption_matrix(long double*** out_matrix, uint32_t dimentation, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (out_matrix == NULL)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *out_matrix = (long double**)malloc(dimentation * sizeof(long double*));
    if (*out_matrix == NULL)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    // Generate random numbers mod prime_field to fill the matrix
    for (uint32_t row = 0; row < dimentation; ++row)
    {
        (*out_matrix)[row] = (long double*)malloc(dimentation * sizeof(long double));
        if ((*out_matrix)[row] == NULL)
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }
        for (uint32_t column = 0; column < dimentation; ++column)
        {
			uint32_t secure_random_number = 0;
            return_code = generate_secure_random_number(&secure_random_number, (uint32_t)0, prime_field - 1);
            if (STATUS_FAILED(return_code))
            {
                goto cleanup;
            }
			(*out_matrix)[row][column] = (long double)(secure_random_number);
        }
    }

    bool matrix_invertible = false;
    return_code = is_matrix_invertible(*out_matrix, dimentation, prime_field, &matrix_invertible);
	if (STATUS_FAILED(return_code) || !matrix_invertible)
	{
		return_code = STATUS_CODE_MATRIX_NOT_INVERTIBLE;
		goto cleanup;
	}

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if (STATUS_FAILED(return_code) && (out_matrix != NULL) && (*out_matrix != NULL))
    {
        for (uint32_t row = 0; row < dimentation; ++row)
        {
            free((*out_matrix)[row]);
            (*out_matrix)[row] = NULL;
        }
        free(*out_matrix);
        *out_matrix = NULL;
    }
    return return_code;
}

STATUS_CODE generate_decryption_matrix(long double*** out_matrix, uint32_t dimentation, long double** encryption_matrix, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    return_code = inverse_square_matrix(encryption_matrix, dimentation, prime_field, out_matrix);

    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}
