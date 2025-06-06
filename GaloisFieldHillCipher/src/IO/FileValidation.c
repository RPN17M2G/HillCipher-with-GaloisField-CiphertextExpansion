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
    STATUS_CODE return_code = STATUS_CODE_FILE_NOT_BINARY;
    const char* binary_extensions[] = {".bin", ".png"};
    size_t number_of_extensions = sizeof(binary_extensions) / sizeof(binary_extensions[0]);
    size_t path_length = 0, extension_length = 0, extension_number = 0;

    if (!path)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    path_length = strlen(path);

    for (extension_number = 0; extension_number < number_of_extensions; ++extension_number) {
        extension_length = strlen(binary_extensions[extension_number]);
        if (path_length >= extension_length &&
            strcmp(path + path_length - extension_length, binary_extensions[extension_number]) == 0)
            {
                return_code = STATUS_CODE_SUCCESS;
                goto cleanup;
            }
    }

    return_code = STATUS_CODE_FILE_NOT_BINARY;
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
