#include "Cipher/CipherParts/AsciiMapping.h"

STATUS_CODE ascii_char_to_digit(uint8_t* out_digit, char input, uint8_t** digit_to_ascii, uint32_t number_of_letters)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t variant = 0, digit = 0;

    if (!out_digit || !digit_to_ascii)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    for (digit = 0; digit <= MAX_DIGIT; ++digit)
    {
        for (variant = 0; variant < number_of_letters; ++variant)
        {
            if (digit_to_ascii[digit][variant] == input)
            {
                return_code = STATUS_CODE_SUCCESS;
                *out_digit = digit;
                goto cleanup;
            }
        }
    }

    return_code = STATUS_CODE_INVALID_ARGUMENT;
cleanup:
    return return_code;
}

STATUS_CODE map_from_int64_to_ascii(uint8_t** out_ascii, uint32_t* out_ascii_size, int64_t* data, uint32_t data_size, uint8_t** digit_to_ascii, uint32_t number_of_letters, uint32_t number_of_digits_per_field_element)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    uint32_t buffer_size = 0;
    uint32_t number_index = 0, digit_index = 0, buffer_index = 0;
    char digit_char = 0;
    uint8_t digit = 0;
    char number_string[number_of_digits_per_field_element + 1];
    uint32_t random_number = 0;

    if (!out_ascii || !out_ascii_size || !data || (data_size == 0) || (data_size > (UINT32_MAX / number_of_digits_per_field_element)) || !digit_to_ascii)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    buffer_size = data_size * number_of_digits_per_field_element;
    buffer = (uint8_t*)malloc(buffer_size);
    if (!buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    buffer_index = 0;
    for (number_index = 0; number_index < data_size; ++number_index)
    {
        // Pad with leading zeros to ensure the correct number of digits per number
        snprintf(number_string, sizeof(number_string), "%0*lld", number_of_digits_per_field_element, data[number_index]);

        for (digit_index = 0; digit_index < number_of_digits_per_field_element; ++digit_index)
        {
            digit_char = number_string[digit_index];
            digit = digit_char - '0';

            return_code = generate_secure_random_number(&random_number, 0, number_of_letters - 1);
            if (STATUS_FAILED(return_code))
            {
                goto cleanup;
            }

            buffer[buffer_index++] = (uint8_t)digit_to_ascii[digit][random_number];
        }
    }

    *out_ascii = buffer;
    buffer = NULL;
    *out_ascii_size = buffer_size;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(buffer);
    return return_code;
}

STATUS_CODE map_from_ascii_to_int64(int64_t** out_int64, uint32_t* out_int64_size, uint8_t* data, uint32_t data_size, uint8_t** digit_to_ascii, uint32_t number_of_letters, uint32_t number_of_digits_per_field_element)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t* buffer = NULL;
    uint32_t number_of_output_numbers = 0, number_index = 0, digit_index = 0, buffer_index = 0;
    char digit_str[number_of_digits_per_field_element + 1];
    uint8_t digit = 0;

    if (!out_int64 || !out_int64_size || !data || (data_size == 0) || !digit_to_ascii)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (data_size % number_of_digits_per_field_element != 0)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    number_of_output_numbers = data_size / number_of_digits_per_field_element;
    buffer = (int64_t*)malloc(number_of_output_numbers * sizeof(int64_t));
    if (!buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    buffer_index = 0;
    for (number_index = 0; number_index < number_of_output_numbers; ++number_index)
    {
        for (digit_index = 0; digit_index < number_of_digits_per_field_element; ++digit_index)
        {
            return_code = ascii_char_to_digit(&digit, (char)data[buffer_index++], digit_to_ascii, number_of_letters);
            if (STATUS_FAILED(return_code))
            {
                goto cleanup;
            }
            digit_str[digit_index] = (char)('0' + digit);
        }
        digit_str[number_of_digits_per_field_element] = '\0';
        buffer[number_index] = (int64_t)strtoll(digit_str, NULL, 10);
    }

    *out_int64 = buffer;
    buffer = NULL;
    *out_int64_size = number_of_output_numbers * sizeof(int64_t);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(buffer);
    return return_code;
}