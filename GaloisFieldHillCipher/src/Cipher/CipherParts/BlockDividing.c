#include "../../../include/Cipher/CipherParts/BlockDividing.h"

STATUS_CODE divide_uint8_t_into_blocks(uint8_t*** out_blocks, uint32_t* num_blocks, uint8_t* value, uint32_t value_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t block_number = 0;
    uint8_t** out_blocks_buffer = NULL;
    uint32_t num_blocks_buffer = 0;

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

    num_blocks_buffer = (value_bit_length / block_bit_size);
    out_blocks_buffer = (uint8_t**)malloc(num_blocks_buffer * sizeof(out_blocks_buffer));
    if (NULL == out_blocks_buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (block_number = 0; block_number < num_blocks_buffer; ++block_number)
    {
        out_blocks_buffer[block_number] = (uint8_t*)malloc(block_bit_size / BYTE_SIZE);
        if (NULL == out_blocks_buffer[block_number])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        // Copy the block data
        memcpy(out_blocks_buffer[block_number], value + (block_number * block_bit_size / BYTE_SIZE), block_bit_size / BYTE_SIZE);
    }

    *out_blocks = out_blocks_buffer;
    out_blocks_buffer = NULL;
    *num_blocks = num_blocks_buffer;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    (void)free_uint8_matrix(out_blocks_buffer, num_blocks_buffer);
    return return_code;
}

STATUS_CODE divide_int64_t_into_blocks(int64_t*** out_blocks, uint32_t* num_blocks, int64_t* value, uint32_t value_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t block_number = 0, element_index_in_block = 0, index = 0;
    int64_t** out_blocks_buffer = NULL;
    uint32_t num_blocks_buffer = 0;

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

    num_blocks_buffer = value_bit_length / block_bit_size;
    uint32_t ints_per_block = block_bit_size / (sizeof(int64_t) * BYTE_SIZE);
    uint32_t total_ints = value_bit_length / (sizeof(int64_t) * BYTE_SIZE);

    out_blocks_buffer = (int64_t**)malloc(num_blocks_buffer * sizeof(int64_t*));
    if (NULL == out_blocks_buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (block_number = 0; block_number < num_blocks_buffer; ++block_number)
    {
        out_blocks_buffer[block_number] = (int64_t*)malloc(ints_per_block * sizeof(int64_t));
        if (NULL == out_blocks_buffer[block_number])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        memset(out_blocks_buffer[block_number], 0, ints_per_block * sizeof(int64_t));

        for (element_index_in_block = 0; element_index_in_block < ints_per_block; ++element_index_in_block)
        {
            index = block_number * ints_per_block + element_index_in_block;
            if (index < total_ints)
            {
                out_blocks_buffer[block_number][element_index_in_block] = value[index];
            }
        }
    }

    *out_blocks = out_blocks_buffer;
    out_blocks_buffer = NULL;
    *num_blocks = num_blocks_buffer;

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    (void)free_int64_matrix(out_blocks_buffer, num_blocks_buffer);
    return return_code;
}
