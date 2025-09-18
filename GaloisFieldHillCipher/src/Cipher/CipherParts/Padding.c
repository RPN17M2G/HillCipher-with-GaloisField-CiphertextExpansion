#include "Cipher/CipherParts/Padding.h"

STATUS_CODE pad_to_length(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length, uint32_t target_bit_length, uint32_t block_bit_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t index = 0;
    uint8_t* out_buffer = NULL;

    if ((NULL == out) || (NULL == value) || (NULL == out_bit_length))
    {
        log_error("Invalid arguments in pad_to_length: %s",
            !out ? "out is NULL" : !value ? "value is NULL" : "out_bit_length is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (value_bit_length > target_bit_length)
    {
        log_error("Input length (%u bits) exceeds target length (%u bits)",
                 value_bit_length, target_bit_length);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Padding data: current=%u bits, target=%u bits, block_size=%u bits",
             value_bit_length, target_bit_length, block_bit_size);

    if (target_bit_length == value_bit_length)
    {
        target_bit_length += block_bit_size;
        log_debug("Adding extra block for padding, new target=%u bits", target_bit_length);
    }

    out_buffer = (uint8_t*)malloc(target_bit_length / BYTE_SIZE);
    if (NULL == out_buffer)
    {
        log_error("Memory allocation failed for padding buffer (size: %u bytes)",
                 target_bit_length / BYTE_SIZE);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    // Copy the original value to the output array
    for (index = 0; index < value_bit_length / BYTE_SIZE; ++index)
    {
        out_buffer[index] = value[index];
    }
    log_debug("Copied %zu bytes of original data", value_bit_length / BYTE_SIZE);

    // Set the padding magic byte
    out_buffer[value_bit_length / BYTE_SIZE] = PADDING_MAGIC;
    log_debug("Added padding magic byte at position %zu", value_bit_length / BYTE_SIZE);

    // Pad the remaining bytes with 0
    for (index = value_bit_length / BYTE_SIZE + 1; index < target_bit_length / BYTE_SIZE; ++index)
    {
        out_buffer[index] = 0;
    }
    log_debug("Padded remaining %zu bytes with zeros",
             (target_bit_length - value_bit_length) / BYTE_SIZE - 1);

    *out = out_buffer;
    out_buffer = NULL;
    *out_bit_length = target_bit_length;
    log_debug("Padding complete: final size=%u bits", target_bit_length);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_buffer);
    return return_code;
}

STATUS_CODE remove_padding(uint8_t** out, uint32_t* out_bit_length, uint8_t* value, uint32_t value_bit_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint32_t i = 0;
    uint8_t* out_buffer = NULL;
    uint32_t original_bit_length = 0;

    if ((NULL == out) || (NULL == value) || (NULL == out_bit_length))
    {
        log_error("Invalid arguments in remove_padding: %s",
            !out ? "out is NULL" : !value ? "value is NULL" : "out_bit_length is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Removing padding from data of length %u bits", value_bit_length);

    // Find the padding magic byte
    for (i = 0; i < value_bit_length / BYTE_SIZE; ++i)
    {
        if (PADDING_MAGIC == value[i])
        {
            original_bit_length = i * BYTE_SIZE;
            log_debug("Found padding magic byte at position %u, original length=%u bits",
                     i, original_bit_length);
            break;
        }
    }

    if (i >= value_bit_length / BYTE_SIZE)
    {
        log_error("Padding magic byte not found in data");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    out_buffer = (uint8_t*)malloc(original_bit_length / BYTE_SIZE);
    if (NULL == out_buffer)
    {
        log_error("Memory allocation failed for unpadded buffer (size: %u bytes)",
                 original_bit_length / BYTE_SIZE);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (i = 0; i < original_bit_length / BYTE_SIZE; ++i)
    {
        out_buffer[i] = value[i];
    }

    *out = out_buffer;
    out_buffer = NULL;
    *out_bit_length = original_bit_length;
    log_debug("Successfully removed padding: final size=%u bits", original_bit_length);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_buffer);
    return return_code;
}
