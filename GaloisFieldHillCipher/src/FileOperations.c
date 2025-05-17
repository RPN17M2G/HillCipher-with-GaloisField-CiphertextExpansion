#include "FileOperations.h"


STATUS_CODE write_to_file(const char* filename, const uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!filename || !data || (size == 0))
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    FILE* file = fopen(filename, "wb");
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
        goto cleanup;
    }

    size_t written = fwrite(data, 1, size, file);
    if (written != size)
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

STATUS_CODE read_from_file(uint8_t** out_data, uint32_t* out_size, const char* filename)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE* file = NULL;
    uint8_t* data = NULL;
    uint32_t size = 0;

    if (!out_data || !out_size || !filename)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    file = fopen(filename, "rb");
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_FILE_OPEN;
        goto cleanup;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = (uint8_t*)malloc(size);
    if (!data)
    {
        fclose(file);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    if (fread(data, 1, size, file) != size)
    {
        fclose(file);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    fclose(file);
    file = NULL;

    *out_data = data;
    *out_size = size;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    if (STATUS_FAILED(return_code))
    {
        free(data);
    }
    return return_code;
}
