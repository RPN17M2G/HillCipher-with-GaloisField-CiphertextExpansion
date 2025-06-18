#include "IO/SerDes.h"

uint32_t calculate_bytes_per_element(uint32_t prime_field)
{
    return (uint32_t)ceil(log2(prime_field) / BYTE_SIZE);
}

uint32_t calculate_digits_per_element(uint32_t prime_field)
{
    return (uint32_t)ceil(log10(prime_field));
}

STATUS_CODE serialize_secrets(uint8_t** out_data, uint32_t* out_size, Secrets secrets)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint32_t digits_per_element = calculate_digits_per_element(secrets.prime_field);
    uint8_t* key_matrix_data = NULL;
    uint32_t key_matrix_size = 0;
    uint8_t* error_vectors_data = NULL;
    uint32_t error_vectors_size = 0;
    uint8_t* ascii_mapping_data = NULL;
    uint32_t ascii_mapping_size = 0;
    uint8_t* permutation_vector_data = NULL;
    uint32_t permutation_vector_size = 0;
    uint8_t* buffer = NULL;
    uint32_t buffer_size = 0;
    size_t offset = 0;

    if (!out_data || !out_size || !secrets.key_matrix || !secrets.error_vectors || !secrets.ascii_mapping || !secrets.permutation_vector || secrets.dimension == 0 || secrets.number_of_error_vectors == 0)
    {
        log_error("[!] Invalid argument in serialize_secrets.");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    return_code = serialize_square_matrix(&key_matrix_data, &key_matrix_size, secrets.key_matrix, secrets.dimension, secrets.prime_field);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = serialize_matrix(&error_vectors_data, &error_vectors_size, secrets.error_vectors, secrets.number_of_error_vectors, secrets.dimension, secrets.prime_field);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = serialize_uint8_matrix(&ascii_mapping_data, &ascii_mapping_size, secrets.ascii_mapping, NUMBER_OF_DIGITS, secrets.number_of_letters_for_each_digit_ascii_mapping, secrets.prime_field);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    permutation_vector_size = digits_per_element * secrets.number_of_digits_per_field_element_permutation;
    permutation_vector_data = (uint8_t*)malloc(permutation_vector_size);
    if (!permutation_vector_data)
    {
        log_error("[!] Memory allocation failed for permutation vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }
    memcpy_s(permutation_vector_data, permutation_vector_size, secrets.permutation_vector, permutation_vector_size);

    buffer_size = key_matrix_size + error_vectors_size + ascii_mapping_size + permutation_vector_size + (sizeof(uint32_t) * 5);
    buffer = (uint8_t*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed in serialize_secrets.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    if (offset + sizeof(uint32_t) > buffer_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(buffer + offset, buffer_size - offset, &secrets.dimension, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (offset + sizeof(uint32_t) > buffer_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(buffer + offset, buffer_size - offset, &secrets.number_of_error_vectors, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (offset + sizeof(uint32_t) > buffer_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(buffer + offset, buffer_size - offset, &secrets.prime_field, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (offset + key_matrix_size > buffer_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(buffer + offset, buffer_size - offset, key_matrix_data, key_matrix_size);
    offset += key_matrix_size;

    if (offset + error_vectors_size > buffer_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(buffer + offset, buffer_size - offset, error_vectors_data, error_vectors_size);
    offset += error_vectors_size;

    if (offset + ascii_mapping_size > buffer_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(buffer + offset, buffer_size - offset, ascii_mapping_data, ascii_mapping_size);
    offset += ascii_mapping_size;

    if (offset + permutation_vector_size > buffer_size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(buffer + offset, buffer_size - offset, permutation_vector_data, permutation_vector_size);

    *out_data = buffer;
    buffer = NULL;
    *out_size = buffer_size;

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(key_matrix_data);
    free(error_vectors_data);
    free(ascii_mapping_data);
    free(permutation_vector_data);
    free(buffer);
    return return_code;
}

STATUS_CODE deserialize_secrets(Secrets* out_secrets, uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    Secrets secrets = {0};
    size_t offset = 0;
    uint32_t dimension = 0, number_of_error_vectors = 0, prime_field = 0;
    uint32_t number_of_letters_for_each_digit_ascii_mapping = 0, number_of_digits_per_field_element_permutation = 0;
    uint32_t bytes_per_element = 0, digits_per_element = 0;
    int64_t** key_matrix_buffer = NULL;
    int64_t** error_vectors_buffer = NULL;
    uint8_t** ascii_mapping_buffer = NULL;
    uint8_t* permutation_vector_buffer = NULL;

    if (!out_secrets || !data || size < sizeof(uint32_t) * 5)
    {
        log_error("[!] Invalid argument in deserialize_secrets.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (offset + sizeof(uint32_t) > size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(&dimension, sizeof(uint32_t), data + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (offset + sizeof(uint32_t) > size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(&number_of_error_vectors, sizeof(uint32_t), data + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (offset + sizeof(uint32_t) > size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(&prime_field, sizeof(uint32_t), data + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (offset + sizeof(uint32_t) > size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(&number_of_letters_for_each_digit_ascii_mapping, sizeof(uint32_t), data + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    if (offset + sizeof(uint32_t) > size)
    {
        return_code = STATUS_CODE_ERROR_INVALID_SIZE;
        goto cleanup;
    }
    memcpy_s(&number_of_digits_per_field_element_permutation, sizeof(uint32_t), data + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    bytes_per_element = calculate_bytes_per_element(prime_field);
    digits_per_element = calculate_digits_per_element(prime_field);

    if (size < offset + (dimension * dimension * bytes_per_element) + (number_of_error_vectors * dimension * bytes_per_element) + (NUMBER_OF_DIGITS * number_of_letters_for_each_digit_ascii_mapping * bytes_per_element) + (digits_per_element * number_of_digits_per_field_element_permutation))
    {
        log_error("[!] Invalid data size in deserialize_secrets.");
        return_code = STATUS_CODE_ERROR_INVALID_FILE_SIZE;
        goto cleanup;
    }

    return_code = deserialize_square_matrix(&key_matrix_buffer, dimension, data + offset, dimension * dimension * calculate_bytes_per_element(prime_field), prime_field);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    offset += dimension * dimension * bytes_per_element;

    return_code = deserialize_matrix(&error_vectors_buffer, number_of_error_vectors, dimension, data + offset, number_of_error_vectors * dimension * calculate_bytes_per_element(prime_field), prime_field);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    offset += number_of_error_vectors * dimension * bytes_per_element;

    return_code = deserialize_uint8_matrix(&ascii_mapping_buffer, NUMBER_OF_DIGITS, number_of_letters_for_each_digit_ascii_mapping, data + offset, NUMBER_OF_DIGITS * number_of_letters_for_each_digit_ascii_mapping * bytes_per_element, prime_field);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    offset += NUMBER_OF_DIGITS * number_of_letters_for_each_digit_ascii_mapping * bytes_per_element;

    permutation_vector_buffer = (uint8_t*)malloc(digits_per_element * number_of_digits_per_field_element_permutation);
    if (!permutation_vector_buffer)
    {
        log_error("[!] Memory allocation failed for permutation vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }
    memcpy_s(permutation_vector_buffer, digits_per_element * number_of_digits_per_field_element_permutation, data + offset, digits_per_element * number_of_digits_per_field_element_permutation);

    secrets.dimension = dimension;
    secrets.number_of_error_vectors = number_of_error_vectors;
    secrets.prime_field = prime_field;
    secrets.number_of_letters_for_each_digit_ascii_mapping = number_of_letters_for_each_digit_ascii_mapping;
    secrets.number_of_digits_per_field_element_permutation = number_of_digits_per_field_element_permutation;
    secrets.key_matrix = key_matrix_buffer;
    key_matrix_buffer = NULL;
    secrets.error_vectors = error_vectors_buffer;
    error_vectors_buffer = NULL;
    secrets.ascii_mapping = ascii_mapping_buffer;
    ascii_mapping_buffer = NULL;
    secrets.permutation_vector = permutation_vector_buffer;
    permutation_vector_buffer = NULL;

    *out_secrets = secrets;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    (void)free_int64_matrix(key_matrix_buffer, secrets.dimension);
    (void)free_int64_matrix(error_vectors_buffer, secrets.number_of_error_vectors);
    (void)free_uint8_matrix(ascii_mapping_buffer, 10);
    free(permutation_vector_buffer);

    return return_code;
}

STATUS_CODE serialize_square_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
    return serialize_matrix(out_data, out_size, matrix, dimension, dimension, prime_field);
}

STATUS_CODE deserialize_square_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size, uint32_t prime_field)
{
    return deserialize_matrix(out_matrix, dimension, dimension, data, size, prime_field);
}

STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t rows, uint32_t columns, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    uint32_t size = 0;
    size_t row = 0, column = 0, byte_index = 0;
    int64_t value = 0;
    uint32_t bytes_per_element = calculate_bytes_per_element(prime_field);

    if (!out_data || !out_size || !matrix || (0 == rows) || (0 == columns))
    {
        log_error("[!] Invalid argument in serialize_matrix.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    size = rows * columns * bytes_per_element;
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
            for (byte_index = 0; byte_index < bytes_per_element; ++byte_index)
            {
                buffer[(row * columns + column) * bytes_per_element + byte_index] =
                    (value >> (BYTE_SIZE * (bytes_per_element - 1 - byte_index))) & 0xFF;
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

STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t rows, uint32_t columns, const uint8_t* data, uint32_t size, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** result = NULL;
    uint32_t expected_size = 0;
    size_t index = 0;
    size_t row = 0, column = 0, byte_index = 0;
    uint32_t bytes_per_element = calculate_bytes_per_element(prime_field);

    if (!out_matrix || !data || (0 == rows) || (0 == columns))
    {
        log_error("[!] Invalid argument in deserialize_matrix.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    expected_size = rows * columns * bytes_per_element;
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
            index = (row * columns + column) * bytes_per_element;
            result[row][column] = 0;
            // Read key data as big endian
            for (byte_index = 0; byte_index < bytes_per_element; ++byte_index)
            {
                result[row][column] |= ((int64_t)data[index + byte_index]) << (BYTE_SIZE * (bytes_per_element - 1 - byte_index));
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

STATUS_CODE serialize_vector(uint8_t** out_data, uint32_t* out_size, int64_t* vector, uint32_t size, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint8_t* buffer = NULL;
    size_t i = 0, byte_index = 0;
    int64_t value = 0;
    uint32_t bytes_per_element = calculate_bytes_per_element(prime_field);

    if (!out_data || !out_size || !vector || (0 == size))
    {
        log_error("[!] Invalid argument in serialize_vector.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    buffer = (uint8_t*)malloc(size * bytes_per_element);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed in serialize_vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (i = 0; i < size; ++i)
    {
        value = vector[i];
        for (byte_index = 0; byte_index < bytes_per_element; ++byte_index)
        {
            buffer[i * bytes_per_element + byte_index] =
                (value >> (BYTE_SIZE * (bytes_per_element - 1 - byte_index))) & 0xFF;
        }
    }

    *out_data = buffer;
    buffer = NULL;
    *out_size = (uint32_t)(size * bytes_per_element);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(buffer);
    return return_code;
}

STATUS_CODE deserialize_vector(int64_t** out_vector, uint32_t* out_size, const uint8_t* data, uint32_t data_size, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t* result = NULL;
    size_t i = 0, byte_index = 0;
    uint32_t bytes_per_element = calculate_bytes_per_element(prime_field);

    if (!out_vector || !data || !out_size)
    {
        log_error("[!] Invalid argument in deserialize_vector.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (data_size % bytes_per_element != 0)
    {
        log_error("[!] Invalid file size in deserialize_vector.");
        return_code = STATUS_CODE_ERROR_INVALID_FILE_SIZE;
        goto cleanup;
    }

    result = (int64_t*)malloc((data_size / bytes_per_element) * sizeof(int64_t));
    if (!result)
    {
        log_error("[!] Memory allocation failed in deserialize_vector.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (i = 0; i < (data_size / bytes_per_element); ++i)
    {
        result[i] = 0;
        for (byte_index = 0; byte_index < bytes_per_element; ++byte_index)
        {
            result[i] |= ((int64_t)data[i * bytes_per_element + byte_index]) << (BYTE_SIZE * (bytes_per_element - 1 - byte_index));
        }
    }

    *out_vector = result;
    result = NULL;
    *out_size = (data_size / bytes_per_element) * sizeof(int64_t);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(result);
    return return_code;
}

STATUS_CODE serialize_uint8_matrix(uint8_t** out_data, uint32_t* out_size, uint8_t** matrix, uint32_t rows, uint32_t columns, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** int64_matrix = NULL;
    uint8_t* buffer = NULL;
    size_t size = 0;

    if (!out_data || !out_size || !matrix || rows == 0 || columns == 0)
    {
        log_error("[!] Invalid argument in serialize_uint8_matrix.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    size = rows * columns;
    buffer = (uint8_t*)malloc(size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed in serialize_uint8_matrix.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    return_code = copy_uint8_matrix_to_int64_matrix(&int64_matrix, matrix, rows, columns);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to copy uint8_t matrix to int64_t matrix.");
        goto cleanup;
    }

    return_code = serialize_matrix(out_data, out_size, int64_matrix, rows, columns, prime_field);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to serialize int64_t matrix.");
        goto cleanup;
    }

    (void)free_int64_matrix(int64_matrix, rows);

cleanup:
    return return_code;
}

STATUS_CODE deserialize_uint8_matrix(uint8_t*** out_matrix, uint32_t rows, uint32_t columns, const uint8_t* data, uint32_t size, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** int64_matrix = NULL;
    uint8_t** uint8_matrix = NULL;

    if (!out_matrix || !data || rows == 0 || columns == 0 || size == 0)
    {
        log_error("[!] Invalid argument in deserialize_uint8_matrix.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = deserialize_matrix(&int64_matrix, rows, columns, data, size, prime_field);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to deserialize data into int64_t matrix.");
        goto cleanup;
    }

    uint8_matrix = (uint8_t**)malloc(rows * sizeof(uint8_t*));
    if (!uint8_matrix)
    {
        log_error("[!] Memory allocation failed for uint8_t matrix.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (size_t row = 0; row < rows; ++row)
    {
        uint8_matrix[row] = (uint8_t*)malloc(columns * sizeof(uint8_t));
        if (!uint8_matrix[row])
        {
            log_error("[!] Memory allocation failed for uint8_t matrix row.");
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (size_t column = 0; column < columns; ++column)
        {
            uint8_matrix[row][column] = (uint8_t)int64_matrix[row][column];
        }
    }

    *out_matrix = uint8_matrix;
    uint8_matrix = NULL;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    (void)free_int64_matrix(int64_matrix, rows);
    (void)free_uint8_matrix(uint8_matrix, rows);
    return return_code;
}

STATUS_CODE copy_uint8_matrix_to_int64_matrix(int64_t*** out_matrix, uint8_t** matrix, uint32_t rows, uint32_t columns)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** result = NULL;
    size_t row = 0, column = 0;

    if (!out_matrix || !matrix || rows == 0 || columns == 0)
    {
        log_error("[!] Invalid argument in copy_uint8_matrix_to_int64_matrix.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    result = (int64_t**)malloc(rows * sizeof(int64_t*));
    if (!result)
    {
        log_error("[!] Memory allocation failed in copy_uint8_matrix_to_int64_matrix.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (row = 0; row < rows; ++row)
    {
        result[row] = (int64_t*)malloc(columns * sizeof(int64_t));
        if (!result[row])
        {
            log_error("[!] Memory allocation failed for row in copy_uint8_matrix_to_int64_matrix.");
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (column = 0; column < columns; ++column)
        {
            result[row][column] = (int64_t)matrix[row][column];
        }
    }

    *out_matrix = result;
    result = NULL;

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    (void)free_int64_matrix(result, rows);

    return return_code;
}
;