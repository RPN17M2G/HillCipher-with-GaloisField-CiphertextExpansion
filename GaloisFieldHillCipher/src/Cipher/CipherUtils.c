#include "../../include/Cipher/CipherUtils.h"

STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint32_t output_byte = 0;
    uint8_t current_working_byte = 0;
    uint32_t random_bit = 0;
    uint32_t value_bit = 0;
    uint32_t current_working_byte_bit_index = 0;
    uint32_t number_of_random_bits = 0;
    uint32_t total_bits = 0;
    uint32_t total_bytes = 0;

    if ((NULL == out) || (NULL == out_bit_size))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    // Calculate the size of the output vector
    number_of_random_bits = NUMBER_OF_RANDOM_BITS_TO_ADD * (value_bit_length / BYTE_SIZE);
    total_bits = value_bit_length + number_of_random_bits;
    total_bytes = total_bits / BYTE_SIZE;

    *out = (uint8_t*)malloc(total_bytes);
    *out_bit_size = total_bits;
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(*out, 0, total_bytes);

    for (size_t bit_number = 0; bit_number < total_bits; ++bit_number)
    {
        if ((bit_number % BYTE_SIZE == 0) && (bit_number != 0))
        {
            ++output_byte;
        }

        if (bit_number % (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD) < BYTE_SIZE)
        {
            current_working_byte = value[value_bit / BYTE_SIZE];
            current_working_byte_bit_index = value_bit % BYTE_SIZE;
            ++value_bit;
        }
        else
        {
            return_code = generate_secure_random_number(&random_bit, 0, 2);
            if (STATUS_FAILED(return_code))
            {
                goto cleanup;
            }

            current_working_byte = random_bit;
            current_working_byte_bit_index = BYTE_SIZE - 1;
        }

        if (IS_BIT_SET(current_working_byte, current_working_byte_bit_index))
        {
            (*out)[output_byte] = SET_BIT((*out)[output_byte], bit_number);
        }
        else
        {
            (*out)[output_byte] = CLEAR_BIT((*out)[output_byte], bit_number);
        }
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out) && (NULL != *out))
    {
        free(*out);
        *out = NULL;
        if (NULL != out_bit_size)
        {
            *out_bit_size = 0;
        }
    }
    return return_code;
}

STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint32_t block_size = 0;
    double number_of_random_plus_byte_blocks = 0;
    uint32_t output_byte = 0;
    uint32_t output_bit = 0;
    uint8_t random_bits_counter = 0;
    uint32_t byte_index = 0;

    if ((NULL == out) || (NULL == out_bit_size) || (NULL == value))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    block_size = BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD;
    number_of_random_plus_byte_blocks = value_bit_length / block_size;
    *out_bit_size = (number_of_random_plus_byte_blocks + (uint32_t)(value_bit_length % block_size != 0)) * BYTE_SIZE;
    *out = (uint8_t*)malloc(*out_bit_size + 1);
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(*out, 0, *out_bit_size);

    for (size_t bit_number = 0; bit_number < value_bit_length; ++bit_number)
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

        byte_index = (uint32_t)(bit_number / BYTE_SIZE);
        if (IS_BIT_SET(value[byte_index], bit_number % BYTE_SIZE))
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

    if ((NULL == out) || (NULL == value) || (NULL == out_bit_length))
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
    for (size_t index = 0; index < value_bit_length / BYTE_SIZE; ++index)
    {
        (*out)[index] = value[index];
    }

    // Pad the remaining bytes with 0
    for (size_t index = value_bit_length / BYTE_SIZE; index < target_bit_length / BYTE_SIZE; ++index)
    {
        (*out)[index] = 0;
    }

    // Set the padding magic byte
    (*out)[value_bit_length / BYTE_SIZE] = PADDING_MAGIC;

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
	bool out_flag_check = false;

    if ((NULL == out) || (NULL == value) || (NULL == out_bit_length))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

	for (size_t byte_number = 0; byte_number < value_bit_length / BYTE_SIZE; ++byte_number)
	{
		// Check if the magic byte is part of the padding
        if (out_flag_check && (value[byte_number] != 0))
        {
            out_flag_check = false;
        }

		// Check if the byte is the padding magic number
		if (value[byte_number] == PADDING_MAGIC)
		{
			*out_bit_length = (byte_number * BYTE_SIZE);
            out_flag_check = true;
		}
	}

    if (!out_flag_check)
    {
        return_code = STATUS_CODE_NO_PADDING;
        goto cleanup;
    }

    *out = (uint8_t*)malloc(*out_bit_length / BYTE_SIZE);
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    // Copy the original value to the output array
    for (size_t copy_index = 0; copy_index < (*out_bit_length / BYTE_SIZE); ++copy_index)
    {
        (*out)[copy_index] = value[copy_index];
    }
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

    if (0 == block_bit_size)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *num_blocks = (value_bit_length / block_bit_size);
    *out_blocks = (uint8_t**)malloc(*num_blocks * sizeof(*out_blocks));
    if (NULL == *out_blocks)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (size_t block_number = 0; block_number < *num_blocks; ++block_number)
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
        for (size_t i = 0; i < *num_blocks; ++i)
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

STATUS_CODE divide_int64_t_into_blocks(int64_t*** out_blocks, uint32_t* num_blocks, int64_t* value, uint32_t value_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if ((NULL == out_blocks)
        || (NULL == num_blocks)
        || (NULL == value)
        || (0 == block_bit_size)
        || (block_bit_size % (sizeof(int64_t) * BYTE_SIZE) != 0)
        || (value_bit_length % block_bit_size != 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *num_blocks = value_bit_length / block_bit_size;
    uint32_t ints_per_block = block_bit_size / (sizeof(int64_t) * BYTE_SIZE);
    uint32_t total_ints = value_bit_length / (sizeof(int64_t) * BYTE_SIZE);

    *out_blocks = (int64_t**)malloc(*num_blocks * sizeof(int64_t*));
    if (NULL == *out_blocks)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (size_t block_number = 0; block_number < *num_blocks; ++block_number)
    {
        (*out_blocks)[block_number] = (int64_t*)malloc(ints_per_block * sizeof(int64_t));
        if (NULL == (*out_blocks)[block_number])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        memset((*out_blocks)[block_number], 0, ints_per_block * sizeof(int64_t));

        for (size_t element_index_in_block = 0; element_index_in_block < ints_per_block; ++element_index_in_block)
        {
            size_t index = block_number * ints_per_block + element_index_in_block;
            if (index < total_ints)
            {
                (*out_blocks)[block_number][element_index_in_block] = value[index];
            }
        }
    }

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if ((STATUS_FAILED(return_code)) && (NULL != out_blocks) && (NULL != *out_blocks) && (NULL != num_blocks))
    {
        for (size_t i = 0; i < *num_blocks; ++i)
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
