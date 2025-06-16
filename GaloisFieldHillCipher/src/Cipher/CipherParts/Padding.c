#include "../../../include/Cipher/CipherParts/Padding.h"

STATUS_CODE pad_to_length(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length, uint32_t target_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t index = 0;
    uint8_t* out_buffer = NULL;

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

    out_buffer = (uint8_t*)malloc(target_bit_length / BYTE_SIZE);
    if (NULL == out_buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }


    // Copy the original value to the output array
    for (index = 0; index < value_bit_length / BYTE_SIZE; ++index)
    {
        (out_buffer)[index] = value[index];
    }

    // Pad the remaining bytes with 0
    for (index = value_bit_length / BYTE_SIZE; index < target_bit_length / BYTE_SIZE; ++index)
    {
        (out_buffer)[index] = 0;
    }

    // Set the padding magic byte
    (out_buffer)[value_bit_length / BYTE_SIZE] = PADDING_MAGIC;

    *out = out_buffer;
    out_buffer = NULL;
    *out_bit_length = target_bit_length;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_buffer);
    return return_code;
}

STATUS_CODE remove_padding(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	bool out_flag_check = false;
    size_t byte_number = 0, copy_index = 0;
    uint32_t out_bit_length_buffer = 0;
    uint8_t* out_buffer = NULL;

    if ((NULL == out) || (NULL == value) || (NULL == out_bit_length))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

	for (byte_number = 0; byte_number < value_bit_length / BYTE_SIZE; ++byte_number)
	{
		// Check if the magic byte is part of the padding
        if (out_flag_check && (value[byte_number] != 0))
        {
            out_flag_check = false;
        }

		// Check if the byte is the padding magic number
		if (value[byte_number] == PADDING_MAGIC)
		{
			out_bit_length_buffer = (byte_number * BYTE_SIZE);
            out_flag_check = true;
		}
	}

    if (!out_flag_check)
    {
        return_code = STATUS_CODE_NO_PADDING;
        goto cleanup;
    }

    out_buffer = (uint8_t*)malloc(out_bit_length_buffer / BYTE_SIZE);
    if (NULL == out_buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    // Copy the original value to the output array
    for (copy_index = 0; copy_index < (out_bit_length_buffer / BYTE_SIZE); ++copy_index)
    {
        (out_buffer)[copy_index] = value[copy_index];
    }

    *out = out_buffer;
    out_buffer = NULL;
    *out_bit_length = out_bit_length_buffer;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_buffer);
    return return_code;
}
