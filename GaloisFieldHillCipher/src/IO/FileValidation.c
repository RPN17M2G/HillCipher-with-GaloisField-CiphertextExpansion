#include "IO/FileValidation.h"

STATUS_CODE validate_file_is_readable(const char* path) {
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    char* reading_mode = STATUS_SUCCESS(validate_file_is_binary(path)) ? "rb" : "r";

    FILE* file = fopen(path, reading_mode);
    if (!file)
    {
        return_code = STATUS_CODE_INPUT_FILE_DOESNT_EXISTS_OR_NOT_READBLE;
        goto cleanup;
    }
    fclose(file);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE validate_file_is_binary(const char* path)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* required_ext = BINARY_FILE_ENDING;
    size_t path_len, ext_len;

    if (!path)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    path_len = strlen(path);
    ext_len = strlen(required_ext);

    if (path_len < ext_len || (strcmp(path + path_len - ext_len, required_ext) != 0))
    {
        return_code = STATUS_CODE_FILE_NOT_BINARY;
        goto cleanup;
    }

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    return return_code;
}

STATUS_CODE validate_output_file(const char* path, FILE_FORMAT format)
{
    STATUS_CODE return_code =STATUS_CODE_UNINITIALIZED;
    char* writing_mode = NULL;
    FILE* file = NULL;
    if (!path)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }


    if (format == OUTPUT_FORMAT_BINARY)
        return_code = STATUS_SUCCESS(validate_file_is_binary(path)) ? STATUS_CODE_SUCCESS : STATUS_CODE_OUTPUT_FILE_NOT_BINARY;
    else if (format == OUTPUT_FORMAT_TEXT)
        return_code = STATUS_FAILED(validate_file_is_binary(path)) ? STATUS_CODE_SUCCESS : STATUS_CODE_OUTPUT_FILE_NOT_TEXT;

    writing_mode = STATUS_SUCCESS(validate_file_is_binary(path)) ? "wb" : "w";

    file = fopen(path, writing_mode);
    if (!file)
    {
        return_code = STATUS_CODE_INPUT_FILE_DOESNT_EXISTS_OR_NOT_WRITEBLE;
        goto cleanup;
    }
    fclose(file);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}
