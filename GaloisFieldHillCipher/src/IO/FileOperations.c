#include "../../include/IO/FileOperations.h"


STATUS_CODE write_uint8_to_file(const char* filepath, const uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    char* reading_mode = NULL;
    FILE* file = NULL;
    size_t size_written = 0;

    if (!filepath || !data || (size == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    reading_mode = STATUS_FAILED(validate_file_is_binary(filepath)) ? "rb" : "r";

    file = fopen(filepath, "wb");
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
        goto cleanup;
    }

    size_written = fwrite(data, 1, size, file);
    if (size_written != size)
    {
        fclose(file);
        return_code = STATUS_CODE_COULDNT_WRITE_FILE;
        goto cleanup;
    }

    fclose(file);
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE read_uint8_from_file(uint8_t** out_data, uint32_t* out_size, const char* filepath)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE* file = NULL;
    uint8_t* data = NULL;
    uint32_t size = 0;
    uint32_t size_read = 0;
    char* reading_mode = NULL;

    if (!out_data || !out_size || !filepath)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    reading_mode = STATUS_SUCCESS(validate_file_is_binary(filepath)) ? "rb" : "r";

    file = fopen(filepath, reading_mode);
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_FILE_OPEN;
        goto cleanup;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file) / BYTE_SIZE;
    fseek(file, 0, SEEK_SET);

    data = (uint8_t*)malloc(size);
    if (!data)
    {
        fclose(file);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    size_read = fread(data, sizeof(uint8_t), size, file);
    if (((size_read == 0) && (size != 0)) || (size_read != size))
    {
        fclose(file);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    fclose(file);
    file = NULL;

    *out_data = data;
    *out_size = size * BYTE_SIZE;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(data);
    }
    return return_code;
}

STATUS_CODE write_int64_to_file(const char* filepath, const int64_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE* file = NULL;
    size_t size_written = 0;
    char* reading_mode = NULL;

    if (!filepath || !data || (size == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    reading_mode = STATUS_SUCCESS(validate_file_is_binary(filepath)) ? "wb" : "w";

    file = fopen(filepath, reading_mode);
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
        goto cleanup;
    }

    size_written = fwrite(data, sizeof(int64_t), size, file);
    if (((size_written == 0) && (size != 0)) || (size_written != size))
    {
        fclose(file);
        return_code = STATUS_CODE_COULDNT_WRITE_FILE;
        goto cleanup;
    }

    fclose(file);
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE read_int64_from_file(int64_t** out_data, uint32_t* out_size, const char* filepath)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE* file = NULL;
    int64_t* data = NULL;
    uint32_t size_in_int64_t = 0;
    size_t size_read = 0;
    char* reading_mode = NULL;

    if (!out_data || !out_size || !filepath)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    reading_mode = STATUS_SUCCESS(validate_file_is_binary(filepath)) ? "rb" : "r";

    file = fopen(filepath, reading_mode);
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_FILE_OPEN;
        goto cleanup;
    }

    fseek(file, 0, SEEK_END);
    size_in_int64_t = ftell(file) / sizeof(int64_t);
    fseek(file, 0, SEEK_SET);

    data = (int64_t*)malloc(size_in_int64_t * sizeof(int64_t));
    if (!data)
    {
        fclose(file);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    size_read = fread(data, sizeof(int64_t), size_in_int64_t, file);
    if (((size_read == 0) && (size_in_int64_t != 0)) || (size_read != size_in_int64_t))
    {
        fclose(file);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    fclose(file);
    file = NULL;

    *out_data = data;
    *out_size = size_in_int64_t * BYTE_SIZE * sizeof(int64_t);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(data);
    }
    return return_code;
}
