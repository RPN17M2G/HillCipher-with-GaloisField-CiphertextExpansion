#include "Math/MatrixMultiplication.h"

STATUS_CODE multiply_matrix_with_uint8_t_vector(int64_t** out_vector, int64_t** matrix, uint8_t* vector, uint32_t dimension, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t product = 0;
    int64_t temp_result = 0;
    size_t row = 0, column = 0;
    int64_t* out_vector_buffer = NULL;

    if ((NULL == out_vector) || (NULL == matrix) || (NULL == vector))
    {
        log_error("[!] Invalid argument in multiply_matrix_with_uint8_t_vector.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    out_vector_buffer = (int64_t*)malloc(dimension * sizeof(int64_t));
    if (NULL == out_vector_buffer)
    {
        log_error("[!] Memory allocation failed in multiply_matrix_with_uint8_t_vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (row = 0; row < dimension; ++row)
    {
        temp_result = 0;
        for (column = 0; column < dimension; ++column)
        {
            product = multiply_over_galois_field(matrix[row][column], (int64_t)vector[column], prime_field);
            temp_result = add_over_galois_field(temp_result, product, prime_field);
        }
        out_vector_buffer[row] = temp_result;
    }

    *out_vector = out_vector_buffer;
    out_vector_buffer = NULL;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_vector_buffer);
    return return_code;
}

STATUS_CODE multiply_matrix_with_int64_t_vector(uint8_t** out_vector, int64_t** matrix, int64_t* vector, uint32_t dimension, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint64_t temp_result = 0;
    int64_t product = 0;
    size_t row = 0, column = 0;
    uint8_t* out_vector_buffer = NULL;

    if ((NULL == out_vector) || (NULL == matrix) || (NULL == vector))
    {
        log_error("[!] Invalid argument in multiply_matrix_with_int64_t_vector.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    out_vector_buffer = (uint8_t*)malloc(dimension + MEMORY_BUFFER_FOR_PLAINTEXT_BLOCK);
    if (NULL == out_vector_buffer)
    {
        log_error("[!] Memory allocation failed in multiply_matrix_with_int64_t_vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (row = 0; row < dimension; ++row)
    {
        temp_result = 0;
        for (column = 0; column < dimension; ++column)
        {
            product = multiply_over_galois_field(matrix[row][column], vector[column], prime_field);
            temp_result = add_over_galois_field(temp_result, product, prime_field);
        }

        if (temp_result > UINT8_MAX)
        {
            log_error("[!] Result width too large in multiply_matrix_with_int64_t_vector.");
            return_code = STATUS_CODE_INVALID_RESULT_WIDTH;
            goto cleanup;
        }
        out_vector_buffer[row] = (uint8_t)(temp_result);
    }

    *out_vector = out_vector_buffer;
    out_vector_buffer = NULL;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(out_vector_buffer);
    return return_code;
}
