#include "FileValidation.h"
#include <stdio.h>

STATUS_CODE validate_file_readable(const char* path) {
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

STATUS_CODE validate_file_is_binary(const char* path) {
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

STATUS_CODE validate_file_readable_and_binary(const char* path) {
    STATUS_CODE return_code = validate_file_readable(path);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = validate_file_is_binary(path);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}
