#include "IO/FileValidation.h"

STATUS_CODE validate_file_is_readable(const char* path)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!path)
    {
        log_error("Invalid argument: path is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Checking if file is readable: %s", path);
    char* reading_mode = STATUS_SUCCESS(validate_file_is_binary(path)) ? "rb" : "r";
    log_debug("Using read mode: %s", reading_mode);

    FILE* file = fopen(path, reading_mode);
    if (!file) {
        log_error("Input file does not exist or is not readable: %s", path);
        return_code = STATUS_CODE_INPUT_FILE_DOESNT_EXISTS_OR_NOT_READBLE;
        goto cleanup;
    }
    fclose(file);

    log_debug("File is readable: %s", path);
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    return return_code;
}

STATUS_CODE validate_file_is_binary(const char* path)
{
    STATUS_CODE return_code = STATUS_CODE_FILE_NOT_BINARY;
    const char* binary_extensions[] = {".bin", ".png"};
    size_t number_of_extensions = sizeof(binary_extensions) / sizeof(binary_extensions[0]);
    size_t path_length = 0, extension_length = 0, extension_number = 0;

    if (!path) {
        log_error("Invalid argument: path is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Checking if file is binary: %s", path);
    path_length = strlen(path);

    for (extension_number = 0; extension_number < number_of_extensions; ++extension_number) {
        extension_length = strlen(binary_extensions[extension_number]);
        if (path_length >= extension_length &&
            strcmp(path + path_length - extension_length, binary_extensions[extension_number]) == 0)
        {
            log_debug("File has binary extension %s", binary_extensions[extension_number]);
            return_code = STATUS_CODE_SUCCESS;
            goto cleanup;
        }
    }

    log_debug("File does not have a known binary extension");
    return_code = STATUS_CODE_FILE_NOT_BINARY;

cleanup:
    return return_code;
}

STATUS_CODE validate_file_is_writeable(const char* path)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!path) {
        log_error("Invalid argument: path is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Checking if file is writeable: %s", path);
    char* writing_mode = STATUS_SUCCESS(validate_file_is_binary(path)) ? "ab" : "a";
    log_debug("Using append mode: %s", writing_mode);

    FILE* file = fopen(path, writing_mode);
    if (!file) {
        log_error("Output file is not writeable: %s", path);
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
        goto cleanup;
    }
    fclose(file);

    log_debug("File is writeable: %s", path);
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    return return_code;
}
