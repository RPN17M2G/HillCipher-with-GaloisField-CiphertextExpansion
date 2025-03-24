#include "CipherUtils.h"


STATUS_CODE add_random_bits_between_bytes(double* out_value, double value)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    const size_t new_size = sizeof(value) + ((sizeof(value) * NUMBER_OF_BITS_TO_INSERT_FOR_EACH_BYTE) / BYTE_SIZE);

    uint8_t double_as_byte_array[new_size];
    memset(double_as_byte_array, 0, new_size);

    uint32_t output_byte = 0;
    uint32_t output_bit = 0;
	uint8_t current_working_byte = 0;

    // Copy value to int for doing arithmetic operations
    int64_t arithmetic_value = 0;
    // TODO: Not working well
    if (memcpy_s(&arithmetic_value, sizeof(arithmetic_value), &value, sizeof(value)))
    {
        return_code = STATUS_CODE_ERROR_COPY_FAILED;
        goto cleanup;
    }

    // TODO: Check
    for (uint32_t byte_number = 0; byte_number < sizeof(value); ++byte_number)
    {
        uint8_t byte_value = (arithmetic_value >> (byte_number * BYTE_SIZE));

        uint8_t random_bits = 0 & TWO_BITS_MASK; // TODO: Add random bits

        // Insert the original byte bits and random bits into the array
        for (uint32_t bit_number = 0; bit_number < (BYTE_SIZE + NUMBER_OF_BITS_TO_INSERT_FOR_EACH_BYTE); ++bit_number)
        {
            ++output_bit;
			if (output_bit % BYTE_SIZE == 0)
			{
				++output_byte;
			}

            if (bit_number < BYTE_SIZE) 
            {
				current_working_byte = byte_value;
            }
            else
            {
				current_working_byte = random_bits;
            }
        
            // Check if current bit is set
            if (current_working_byte & (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE))))
            {
                // If set, using OR operator to set the bit in the byte array
                double_as_byte_array[output_byte] |= (1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)));
            }
            else
            {
                // If not set, using AND operator to clear the bit in the byte array
                double_as_byte_array[output_byte] &= ~(1 << (BYTE_SIZE - 1 - (bit_number % BYTE_SIZE)));
            }
        }
    }

    if (memcpy_s(out_value, sizeof(double), double_as_byte_array, new_size))
    {
        return_code = STATUS_CODE_ERROR_COPY_FAILED;
        goto cleanup;
    }

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}
