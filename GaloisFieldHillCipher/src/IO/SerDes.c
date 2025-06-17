#include "IO/SerDes.h"

STATUS_CODE serialize_square_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension)
{
    return serialize_matrix(out_data, out_size, matrix, dimension, dimension);
}

STATUS_CODE deserialize_square_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size)
{
    return deserialize_matrix(out_matrix, dimension, dimension, data, size);
}

STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t rows, uint32_t columns)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    uint32_t size = 0;
    size_t row = 0, column = 0, byte_index = 0;
    int64_t value = 0;

    if (!out_data || !out_size || !matrix || (0 == rows) || (0 == columns))
    {
        log_error("[!] Invalid argument in serialize_matrix.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    size = rows * columns * NUMBER_OF_BYTES_PER_ELEMENT;
    buffer = (uint8_t*)malloc(size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed in serialize_matrix.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (row = 0; row < rows; ++row)
    {
        for (column = 0; column < columns; ++column)
        {
            value = matrix[row][column];

            // Store each value big endian
            for (byte_index = 0; byte_index < NUMBER_OF_BYTES_PER_ELEMENT; ++byte_index)
            {
                buffer[(row * columns + column) * NUMBER_OF_BYTES_PER_ELEMENT + byte_index] =
                    (value >> (BYTE_SIZE * (NUMBER_OF_BYTES_PER_ELEMENT - 1 - byte_index))) & 0xFF;
            }
        }
    }

    *out_data = buffer;
    buffer = NULL;
    *out_size = size;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(buffer);
    return return_code;
}

STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t rows, uint32_t columns, const uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** result = NULL;
    uint32_t expected_size = 0;
    size_t index = 0;
    size_t row = 0, column = 0, byte_index = 0;

    if (!out_matrix || !data || (0 == rows) || (0 == columns))
    {
        log_error("[!] Invalid argument in deserialize_matrix.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    expected_size = rows * columns * NUMBER_OF_BYTES_PER_ELEMENT * BYTE_SIZE;
    if (size != expected_size)
    {
        log_error("[!] Invalid file size in deserialize_matrix.");
        return_code = STATUS_CODE_ERROR_INVALID_FILE_SIZE;
        goto cleanup;
    }

    result = (int64_t**)malloc(rows * sizeof(int64_t*));
    if (!result)
    {
        log_error("[!] Memory allocation failed in deserialize_matrix.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (row = 0; row < rows; ++row)
    {
        result[row] = (int64_t*)malloc(columns * sizeof(int64_t));
        if (!result[row])
        {
            log_error("[!] Memory allocation failed for row in deserialize_matrix.");
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (column = 0; column < columns; ++column)
        {
            index = (row * columns + column) * NUMBER_OF_BYTES_PER_ELEMENT;
            result[row][column] = 0;
            // Read key data as big endian
            for (byte_index = 0; byte_index < NUMBER_OF_BYTES_PER_ELEMENT; ++byte_index)
            {
                result[row][column] |= ((int64_t)data[index + byte_index]) << (BYTE_SIZE * (NUMBER_OF_BYTES_PER_ELEMENT - 1 - byte_index));
            }
        }
    }

    *out_matrix = result;
    result = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    (void)free_int64_matrix(result, rows);

    return return_code;
}

STATUS_CODE serialize_vector(uint8_t** out_data, uint32_t* out_size, int64_t* vector, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    size_t i = 0, byte_index = 0;
    int64_t value = 0;

    if (!out_data || !out_size || !vector || (0 == size))
    {
        log_error("[!] Invalid argument in serialize_vector.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    buffer = (uint8_t*)malloc(size * NUMBER_OF_BYTES_PER_ELEMENT);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed in serialize_vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (i = 0; i < size; ++i)
    {
        value = vector[i];
        for (byte_index = 0; byte_index < NUMBER_OF_BYTES_PER_ELEMENT; ++byte_index)
        {
            buffer[i * NUMBER_OF_BYTES_PER_ELEMENT + byte_index] =
                (value >> (BYTE_SIZE * (NUMBER_OF_BYTES_PER_ELEMENT - 1 - byte_index))) & 0xFF;
        }
    }

    *out_data = buffer;
    buffer = NULL;
    *out_size = (uint32_t)(size * NUMBER_OF_BYTES_PER_ELEMENT);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(buffer);
    return return_code;
}

STATUS_CODE deserialize_vector(int64_t** out_vector, uint32_t* out_size, const uint8_t* data, uint32_t data_size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t* result = NULL;
    size_t i = 0, byte_index = 0;

    if (!out_vector || !data || !out_size)
    {
        log_error("[!] Invalid argument in deserialize_vector.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (data_size % NUMBER_OF_BYTES_PER_ELEMENT != 0)
    {
        log_error("[!] Invalid file size in deserialize_vector.");
        return_code = STATUS_CODE_ERROR_INVALID_FILE_SIZE;
        goto cleanup;
    }

    result = (int64_t*)malloc((data_size / NUMBER_OF_BYTES_PER_ELEMENT) * sizeof(int64_t));
    if (!result)
    {
        log_error("[!] Memory allocation failed in deserialize_vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (i = 0; i < (data_size / NUMBER_OF_BYTES_PER_ELEMENT); ++i)
    {
        result[i] = 0;
        for (byte_index = 0; byte_index < NUMBER_OF_BYTES_PER_ELEMENT; ++byte_index)
        {
            result[i] |= ((int64_t)data[i * NUMBER_OF_BYTES_PER_ELEMENT + byte_index]) << (BYTE_SIZE * (NUMBER_OF_BYTES_PER_ELEMENT - 1 - byte_index));
        }
    }

    *out_vector = result;
    result = NULL;
    *out_size = (data_size / NUMBER_OF_BYTES_PER_ELEMENT) * sizeof(int64_t);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(result);
    return return_code;
}
