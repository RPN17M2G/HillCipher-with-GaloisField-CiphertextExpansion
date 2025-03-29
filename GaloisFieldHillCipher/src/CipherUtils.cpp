#include "CipherUtils.h"

STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    uint32_t output_byte = 0;
    uint8_t current_working_byte = 0;

    if ((NULL == out) || (NULL == out_size))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    // Calculate the size of the output vector
    uint32_t number_of_bytes_in_value = value_length / BYTE_SIZE + 1;
    uint32_t number_of_random_bits = NUMBER_OF_RANDOM_BITS_TO_ADD * number_of_bytes_in_value;
    uint32_t total_bits = value_length + number_of_random_bits;
	total_bits = total_bits + (BYTE_SIZE - (total_bits % BYTE_SIZE)); // Round up to the nearest byte
    uint32_t total_bytes = (total_bits / BYTE_SIZE) + 1; 

    *out = (uint8_t*)malloc(total_bytes);
    *out_size = total_bits;
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(*out, 0, total_bytes);

    uint32_t value_index = 0;
    uint32_t output_bit = 0;

    for (uint32_t byte_number = 0; byte_number < number_of_bytes_in_value; ++byte_number)
    {
        uint8_t random_bits = 0 & TWO_BITS_MASK; // TODO: Add random bits

        // Insert the original byte bits and random bits into the array
        for (uint32_t bit_number = 0; bit_number < (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD); ++bit_number)
        {
            if ((output_bit % BYTE_SIZE == 0) && (output_bit != 0))
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
            if ((current_working_byte & (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)))) != 0)
            {
                // If set, using OR operator to set the bit in the byte array
                (*out)[output_byte] |= (1 << (BYTE_SIZE - 1 - (output_bit % BYTE_SIZE)));
            }
            else
            {
                // If not set, using AND operator to clear the bit in the byte array
                (*out)[output_byte] &= ~(1 << (BYTE_SIZE - 1 - (output_bit % BYTE_SIZE)));
            }

            ++output_bit;
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


STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if ((NULL == out) || (NULL == out_size) || (NULL == value))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *out = (uint8_t*)malloc((value_length / (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD)) * BYTE_SIZE + 1);
	*out_size = (value_length / (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD)) * BYTE_SIZE;
    if (NULL == *out)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memset(*out, 0, value_length / (BYTE_SIZE + NUMBER_OF_RANDOM_BITS_TO_ADD) * BYTE_SIZE);

    uint32_t output_byte = 0;
    uint32_t output_bit = 0;
    uint8_t random_bits_counter = 0;

    for (uint32_t byte_number = 0; byte_number < value_length / BYTE_SIZE; ++byte_number)
    {
        for (uint32_t bit_number = 0; bit_number < BYTE_SIZE; ++bit_number)
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

            // Check if current bit is set
            if ((value[byte_number] & (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)))) != 0)
            {
                // If set, using OR operator to set the bit in the byte array
                (*out)[output_byte] |= (1 << (BYTE_SIZE - 1 - (output_bit % BYTE_SIZE)));
            }   
            else
            {
                // If not set, using AND operator to clear the bit in the byte array
                (*out)[output_byte] &= ~(1 << (BYTE_SIZE - 1 - (output_bit % BYTE_SIZE)));
            }

            ++output_bit;
            if (output_bit % BYTE_SIZE == 0)
            {
                ++output_byte;
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

