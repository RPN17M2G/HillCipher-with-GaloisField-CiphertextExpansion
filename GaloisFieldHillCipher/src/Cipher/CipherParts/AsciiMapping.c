#include "Cipher/CipherParts/AsciiMapping.h"

STATUS_CODE ascii_char_to_digit(uint8_t* out_digit, uint8_t input, uint8_t** digit_to_ascii, uint32_t number_of_letters)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t variant = 0, digit = 0;

    if (!out_digit || !digit_to_ascii)
    {
        log_error("Invalid arguments in ascii_char_to_digit: %s",
                  !out_digit ? "out_digit is NULL" : "digit_to_ascii is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Converting ASCII char '%c' (0x%02x) to digit", input, input);

    for (digit = 0; digit <= MAX_DIGIT; ++digit)
    {
        for (variant = 0; variant < number_of_letters; ++variant)
        {
            if (digit_to_ascii[digit][variant] == input)
            {
                log_debug("Found match: ASCII '%c' maps to digit %zu", input, digit);
                *out_digit = digit;
                return_code = STATUS_CODE_SUCCESS;
                goto cleanup;
            }
        }
    }

    log_error("No mapping found for ASCII character '%c' (0x%02x)", input, input);
    return_code = STATUS_CODE_INVALID_ARGUMENT;

cleanup:
    return return_code;
}

STATUS_CODE map_from_int64_to_ascii(uint8_t** out_ascii, uint32_t* out_ascii_size, int64_t* data,
    uint32_t data_size, uint8_t** digit_to_ascii, uint32_t number_of_letters,
    uint32_t number_of_digits_per_field_element)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    uint32_t buffer_size = 0;
    uint32_t number_index = 0, digit_index = 0, buffer_index = 0;
    char digit_char = 0;
    uint8_t digit = 0;
    char number_string[number_of_digits_per_field_element + 1];
    uint32_t random_number = 0;

    if (!out_ascii || !out_ascii_size || !data || (data_size == 0) ||
        (data_size > (UINT32_MAX / number_of_digits_per_field_element)) || !digit_to_ascii)
    {
        log_error("Invalid arguments in map_from_int64_to_ascii");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Converting %u int64 values to ASCII (digits per element: %u)",
              data_size, number_of_digits_per_field_element);

    buffer_size = data_size * number_of_digits_per_field_element;
    buffer = (uint8_t*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("Memory allocation failed for ASCII buffer (size: %u)", buffer_size);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (number_index = 0; number_index < data_size; ++number_index)
    {
        snprintf(number_string, sizeof(number_string), "%0*lld",
                 number_of_digits_per_field_element, data[number_index]);
        log_debug("Processing number %u: %s", number_index, number_string);

        for (digit_index = 0; digit_index < number_of_digits_per_field_element; ++digit_index)
        {
            digit_char = number_string[digit_index];
            digit = digit_char - '0';

            return_code = generate_secure_random_number(&random_number, 0, number_of_letters - 1);
            if (STATUS_FAILED(return_code))
            {
                log_error("Failed to generate random mapping for digit %c", digit_char);
                goto cleanup;
            }

            buffer[buffer_index++] = digit_to_ascii[digit][random_number];
            log_debug("Mapped digit %c to ASCII '%c'", digit_char, buffer[buffer_index - 1]);
        }
    }

    *out_ascii = buffer;
    buffer = NULL;
    *out_ascii_size = buffer_size;
    log_debug("Successfully mapped %u numbers to %u ASCII characters", data_size, buffer_size);
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(buffer);
    return return_code;
}

STATUS_CODE map_from_ascii_to_int64(int64_t** out_numbers, uint32_t* out_size, uint8_t* data,
    uint32_t data_size, uint8_t** digit_to_ascii, uint32_t number_of_letters,
    uint32_t number_of_digits_per_field_element)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t* buffer = NULL;
    uint32_t buffer_size = 0;
    uint32_t number_index = 0, digit_index = 0;
    uint8_t digit = 0;
    char number_string[number_of_digits_per_field_element + 1];

    if (!out_numbers || !out_size || !data || (data_size == 0) || !digit_to_ascii ||
        (data_size % number_of_digits_per_field_element != 0))
    {
        log_error("Invalid arguments in map_from_ascii_to_int64");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Converting %u ASCII characters to int64 values (digits per element: %u)",
              data_size, number_of_digits_per_field_element);

    buffer_size = data_size / number_of_digits_per_field_element;
    buffer = (int64_t*)malloc(buffer_size * sizeof(int64_t));
    if (!buffer)
    {
        log_error("Memory allocation failed for number buffer (size: %u)", buffer_size);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (number_index = 0; number_index < buffer_size; ++number_index)
    {
        for (digit_index = 0; digit_index < number_of_digits_per_field_element; ++digit_index)
        {
            return_code = ascii_char_to_digit(&digit,
                data[number_index * number_of_digits_per_field_element + digit_index],
                digit_to_ascii, number_of_letters);

            if (STATUS_FAILED(return_code))
            {
                log_error("Failed to map ASCII char '%c' to digit at position %u",
                    data[number_index * number_of_digits_per_field_element + digit_index],
                    digit_index);
                goto cleanup;
            }
            number_string[digit_index] = '0' + digit;
        }
        number_string[number_of_digits_per_field_element] = '\0';

        buffer[number_index] = atoll(number_string);
        log_debug("Mapped ASCII sequence to number %ld", buffer[number_index]);
    }

    *out_numbers = buffer;
    buffer = NULL;
    *out_size = buffer_size * sizeof(int64_t);
    log_debug("Successfully mapped %u ASCII characters to %u numbers", data_size, buffer_size);
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(buffer);
    return return_code;
}
