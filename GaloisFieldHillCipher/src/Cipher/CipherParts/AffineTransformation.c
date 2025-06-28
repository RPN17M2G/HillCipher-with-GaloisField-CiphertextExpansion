#include "Cipher/CipherParts/AffineTransformation.h"

STATUS_CODE add_affine_transformation(int64_t** out_transformed_vector, int64_t** error_vectors, uint32_t number_of_error_vectors, int64_t* vector_to_transform, uint32_t dimension, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t* transformed_vector = NULL;
    int64_t* temp_vector = NULL;
    uint32_t error_vector_index = 0;

    if ((NULL == out_transformed_vector) || (NULL == vector_to_transform) || (NULL == error_vectors))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    transformed_vector = (int64_t*)malloc(dimension * sizeof(int64_t));
    if (NULL == transformed_vector)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memcpy_s(transformed_vector, dimension * sizeof(int64_t), vector_to_transform, dimension * sizeof(int64_t));
    for (error_vector_index = 0; error_vector_index < number_of_error_vectors; ++error_vector_index)
    {
        return_code = add_two_vectors_over_gf(&temp_vector, transformed_vector, error_vectors[error_vector_index], dimension, prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to add error vector %u in affine_transformation.", error_vector_index);
            goto cleanup;
        }
        free(transformed_vector);
        transformed_vector = temp_vector;
        temp_vector = NULL;
    }

    *out_transformed_vector = transformed_vector;
    transformed_vector = NULL;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(transformed_vector);
    return return_code;
}

STATUS_CODE substruct_affine_transformation(int64_t** out_transformed_vector, int64_t** error_vectors, uint32_t number_of_error_vectors, int64_t* vector_to_transform, uint32_t dimension, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t* transformed_vector = NULL;
    int64_t* temp_vector = NULL;
    uint32_t error_vector_index = 0;

    if ((NULL == out_transformed_vector) || (NULL == vector_to_transform) || (NULL == error_vectors))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    transformed_vector = (int64_t*)malloc(dimension * sizeof(int64_t));
    if (NULL == transformed_vector)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    memcpy_s(transformed_vector, dimension * sizeof(int64_t), vector_to_transform, dimension * sizeof(int64_t));
    for (error_vector_index = 0; error_vector_index < number_of_error_vectors; ++error_vector_index)
    {
        return_code = substruct_two_vectors_over_gf(&temp_vector, transformed_vector, error_vectors[error_vector_index], dimension, prime_field);
        if (STATUS_FAILED(return_code))
        {
            log_error("[!] Failed to substruct error vector %u in affine_transformation.", error_vector_index);
            goto cleanup;
        }
        free(transformed_vector);
        transformed_vector = temp_vector;
        temp_vector = NULL;
    }

    *out_transformed_vector = transformed_vector;
    transformed_vector = NULL;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(transformed_vector);
    return return_code;
}
