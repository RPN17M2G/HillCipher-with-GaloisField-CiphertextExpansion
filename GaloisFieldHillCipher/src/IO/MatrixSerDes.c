#include "../../include/IO/MatrixSerDes.h"

#include "Cipher/CipherUtils.h"


STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    uint32_t size = 0;

    if (!out_data || !out_size || !matrix || (dimension == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    size = dimension * dimension * sizeof(int64_t);
    buffer = (uint8_t*)malloc(size);
    if (!buffer)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (size_t row = 0; row < dimension; ++row)
    {
        memcpy(buffer + row * dimension * sizeof(int64_t), matrix[row], dimension * sizeof(int64_t));
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

    if (!out_matrix || !data || dimension == 0)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    uint32_t expected_size = dimension * dimension * sizeof(int64_t) * BYTE_SIZE;
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

    for (size_t i = 0; i < dimension; ++i)
    {
        result[i] = (int64_t*)malloc(dimension * sizeof(int64_t));
        if (!result[i])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }
        memcpy(result[i], data + i * dimension * sizeof(int64_t), dimension * sizeof(int64_t));
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
