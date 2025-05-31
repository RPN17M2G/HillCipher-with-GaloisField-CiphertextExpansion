#include "../../include/IO/FileValidation.h"

STATUS_CODE validate_file_is_readable(const char* path) {
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE* file = fopen(path, "rb");
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

