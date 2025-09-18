#include "IO/FileOperations.h"

STATUS_CODE write_uint8_to_file(const char* filepath, const uint8_t* data, uint32_t size)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    char* writing_mode = NULL;
    FILE* file = NULL;
    size_t size_written = 0;

    if (!filepath || !data || (0 == size))
    {
        log_error("Invalid arguments in write_uint8_to_file: %s", !filepath ? "filepath is NULL" :
            !data ? "data is NULL" : "size is 0");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    writing_mode = STATUS_SUCCESS(validate_file_is_binary(filepath)) ? "wb" : "w";
    log_debug("Writing to file %s in %s mode", filepath, writing_mode);

    file = fopen(filepath, writing_mode);
    if (!file)
    {
        log_error("Failed to create output file: %s", filepath);
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
        goto cleanup;
    }

    size_written = fwrite(data, 1, size, file);
    if (size_written != size)
    {
        log_error("Failed to write complete data to file %s (wrote %zu of %u bytes)",
            filepath, size_written, size);
        fclose(file);
        return_code = STATUS_CODE_COULDNT_WRITE_FILE;
        goto cleanup;
    }

    log_debug("Successfully wrote %zu bytes to %s", size_written, filepath);
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
        log_error("Invalid arguments in read_uint8_from_file: %s", !out_data ? "out_data is NULL" :
            !out_size ? "out_size is NULL" : "filepath is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    reading_mode = STATUS_SUCCESS(validate_file_is_binary(filepath)) ? "rb" : "r";
    log_debug("Reading from file %s in %s mode", filepath, reading_mode);

    file = fopen(filepath, reading_mode);
    if (!file)
    {
        log_error("Failed to open file for reading: %s", filepath);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        log_error("Failed to seek to end of file %s", filepath);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    size = (uint32_t)ftell(file);
    if (size == (uint32_t)-1)
    {
        log_error("Failed to get file size for %s", filepath);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        log_error("Failed to seek back to start of file %s", filepath);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    data = (uint8_t*)malloc(size);
    if (!data)
    {
        log_error("Failed to allocate memory for file contents (%u bytes)", size);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    size_read = (uint32_t)fread(data, 1, size, file);
    if (size_read != size)
    {
        log_error("Failed to read complete file %s (read %u of %u bytes)",
            filepath, size_read, size);
        return_code = STATUS_CODE_COULDNT_READ_FILE;
        goto cleanup;
    }

    log_debug("Successfully read %u bytes from %s", size_read, filepath);
    *out_data = data;
    data = NULL;
    *out_size = size;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (file)
    {
        fclose(file);
    }
    free(data);
    return return_code;
}
