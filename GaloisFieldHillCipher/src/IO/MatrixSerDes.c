#include "../../include/IO/MatrixSerDes.h"

#include "Cipher/CipherUtils.h"


STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    uint32_t size = 0;

    if (!out_data || !out_size || !matrix || (0 == dimension))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    size = dimension * dimension * NUMBER_OF_BYTES_PER_ELEMENT;
    buffer = (uint8_t*)malloc(size);
    if (!buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (size_t row = 0; row < dimension; ++row)
    {
        for (size_t column = 0; column < dimension; ++column)
        {
            int64_t value = matrix[row][column];

            // Store each value big endian
            for (size_t byte_index = 0; byte_index < NUMBER_OF_BYTES_PER_ELEMENT; ++byte_index)
            {
                buffer[(row * dimension + column) * NUMBER_OF_BYTES_PER_ELEMENT + byte_index] =
                    (value >> (BYTE_SIZE * (NUMBER_OF_BYTES_PER_ELEMENT - 1 - byte_index))) & 0xFF;
            }
        }
    }

    *out_data = buffer;
    *out_size = size;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(buffer);
    }
    return return_code;
}

STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** result = NULL;
    uint32_t expected_size = 0;
    size_t index = 0;

    if (!out_matrix || !data || (0 == dimension))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    expected_size = dimension * dimension * NUMBER_OF_BYTES_PER_ELEMENT * BYTE_SIZE;
    if (size != expected_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_FILE_SIZE;
        goto cleanup;
    }

    result = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!result)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (size_t row = 0; row < dimension; ++row)
    {
        result[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
        if (!result[row])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (size_t column = 0; column < dimension; ++column)
        {
            index = (row * dimension + column) * NUMBER_OF_BYTES_PER_ELEMENT;
            result[row][column] = 0;
            // Read key data as big endian
            for (size_t byte_index = 0; byte_index < NUMBER_OF_BYTES_PER_ELEMENT; ++byte_index)
            {
                result[row][column] |= ((int64_t)data[index + byte_index]) << (BYTE_SIZE * (NUMBER_OF_BYTES_PER_ELEMENT - 1 - byte_index));
            }
        }
    }

    *out_matrix = result;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (STATUS_FAILED(return_code) && result != NULL)
    {
        free_matrix(result, dimension);
    }
    return return_code;
}
