#include "../../../include/Cipher/CipherParts/CiphertextExpansion.h"

STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length, uint32_t number_of_random_bits_to_add)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t bit_number = 0;
    uint32_t output_byte = 0;
    uint8_t current_working_byte = 0;
    uint32_t random_bit = 0;
    uint32_t value_bit = 0;
    uint32_t current_working_byte_bit_index = 0;
    uint32_t number_of_random_bits = 0;
    uint32_t total_bits = 0;
    uint32_t total_bytes = 0;

    uint8_t* out_buffer = NULL;

    if ((NULL == out) || (NULL == out_bit_size) || (number_of_random_bits_to_add > ((UINT32_MAX / (value_bit_length / BYTE_SIZE)) + value_bit_length)))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    // Calculate the size of the output vector
    number_of_random_bits = number_of_random_bits_to_add * (value_bit_length / BYTE_SIZE);
    total_bits = value_bit_length + number_of_random_bits;
    total_bytes = total_bits / BYTE_SIZE;

    out_buffer = (uint8_t*)malloc(total_bytes);
    if (NULL == out_buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(out_buffer, 0, total_bytes);

    for (bit_number = 0; bit_number < total_bits; ++bit_number)
    {
        if ((bit_number % BYTE_SIZE == 0) && (bit_number != 0))
        {
            ++output_byte;
        }

        if (bit_number % (BYTE_SIZE + number_of_random_bits_to_add) < BYTE_SIZE)
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
            (out_buffer)[output_byte] = SET_BIT((out_buffer)[output_byte], bit_number);
        }
        else
        {
            (out_buffer)[output_byte] = CLEAR_BIT((out_buffer)[output_byte], bit_number);
        }
    }

    *out = out_buffer;
    out_buffer = NULL;
    *out_bit_size = total_bits;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_buffer);
    return return_code;
}

STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_bit_size, uint8_t* value, uint32_t value_bit_length, uint32_t number_of_random_bits_to_remove)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint32_t block_size = 0;
    size_t bit_number = 0;
    double number_of_random_plus_byte_blocks = 0;
    uint32_t output_byte = 0;
    uint32_t output_bit = 0;
    uint8_t random_bits_counter = 0;
    uint32_t byte_index = 0;

    uint32_t out_bit_size_buffer = 0;
    uint8_t* out_buffer = NULL;

    if ((NULL == out) || (NULL == out_bit_size) || (NULL == value) || ((value_bit_length / (BYTE_SIZE + number_of_random_bits_to_remove)) > (UINT32_MAX / BYTE_SIZE)))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    block_size = BYTE_SIZE + number_of_random_bits_to_remove;
    number_of_random_plus_byte_blocks = value_bit_length / block_size;
    out_bit_size_buffer = (number_of_random_plus_byte_blocks + (uint32_t)(value_bit_length % block_size != 0)) * BYTE_SIZE;
    out_buffer = (uint8_t*)malloc(out_bit_size_buffer + 1);
    if (NULL == out_buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(out_buffer, 0, out_bit_size_buffer);

    for (bit_number = 0; bit_number < value_bit_length; ++bit_number)
    {
        // Skip the random bits
        if ((output_bit % BYTE_SIZE == 0) && (output_bit != 0))
        {
            if (random_bits_counter != number_of_random_bits_to_remove)
            {
                ++random_bits_counter;
                continue;
            }
            random_bits_counter = 0;
        }

        byte_index = (uint32_t)(bit_number / BYTE_SIZE);
        if (IS_BIT_SET(value[byte_index], bit_number % BYTE_SIZE))
        {
            (out_buffer)[output_byte] = SET_BIT((out_buffer)[output_byte], output_bit);
        }
        else
        {
            (out_buffer)[output_byte] = CLEAR_BIT((out_buffer)[output_byte], output_bit);
        }

        ++output_bit;
        if (output_bit % BYTE_SIZE == 0)
        {
            ++output_byte;
        }
    }

    *out = out_buffer;
    out_buffer = NULL;
    *out_bit_size = out_bit_size_buffer;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_buffer);
    return return_code;
}
