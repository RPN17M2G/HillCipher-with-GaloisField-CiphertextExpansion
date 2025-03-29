#include "CipherUtils.h"


STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    uint32_t output_byte = 0;
    uint32_t output_bit = 0;
    uint8_t current_working_byte = 0;

	if ((NULL == out) || (NULL == out_size))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	*out = (uint8_t*)malloc(((value_length) + (NUMBER_OF_RANDOM_BITS_TO_ADD * ((value_length / BYTE_SIZE) + 1))) / BYTE_SIZE + 1);
    *out_size = (value_length) + (NUMBER_OF_RANDOM_BITS_TO_ADD * ((value_length / BYTE_SIZE) + 1));
	if (NULL == *out)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	uint32_t value_index = 0;

    for (uint32_t byte_number = 0; byte_number < sizeof(value); ++byte_number)
    {
        uint8_t random_bits = 0 & TWO_BITS_MASK; // TODO: Add random bits

        // Insert the original byte bits and random bits into the array
        for (uint32_t bit_number = 0; bit_number < (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD); ++bit_number)
        {
            ++output_bit;
            if (output_bit % BYTE_SIZE == 0)
            {
                ++output_byte;
            }

            if (bit_number < BYTE_SIZE)
            {
                current_working_byte = value[byte_number];
            }
            else
            {
                current_working_byte = random_bits;
            }

            // Check if current bit is set
            if (current_working_byte & (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))
            {
                // If set, using OR operator to set the bit in the byte array
                (*out)[output_byte] |= (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)));
            }
            else
            {
                // If not set, using AND operator to clear the bit in the byte array
                (*out)[output_byte] &= ~(1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)));
            }
        }
    }

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if ((STATUS_FAILED(return_code)) && (NULL != out) && (NULL != *out))
	{
		free(*out);
		*out = NULL;
        *out_size = 0;
	}
	return return_code;
}

STATUS_CODE add_padding_to_match_size(uint8_t** out, uint32_t* out_size, uint32_t current_size, uint32_t block_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (out == NULL || out_size == NULL)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    uint32_t padding_size = block_size - (current_size % block_size);
    if (padding_size == block_size)
    {
        return_code = STATUS_CODE_SUCCESS;
        goto cleanup;
    }

    *out_size = current_size + padding_size;

    uint8_t* new_out = (uint8_t*)realloc(*out, (*out_size / BYTE_SIZE) + 1);
    if (new_out == NULL)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }
    *out = new_out;

    // Add padding (zero-padding)
    memset((*out) + (current_size / BYTE_SIZE), 0, padding_size / BYTE_SIZE);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if ((STATUS_FAILED(return_code)) && (*out != NULL))
    {
        free(*out);
        *out = NULL;
        *out_size = 0;
    }
    return return_code;
}
