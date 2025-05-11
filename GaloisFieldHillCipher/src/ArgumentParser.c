#include "ArgumentParser.h"

STATUS_CODE validate_readable_file(const char* path)
{
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

STATUS_CODE create_output_file_if_needed(const char* path) 
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    FILE* file = fopen(path, "ab+");
    if (!file)
    {
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
		goto cleanup;
    }
    fclose(file);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE extract_arguments(ParsedArguments* out_args, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    uint32_t parse_result = 0;
    const char* input_file = NULL;
    const char* output_file = NULL;
    const char* key = NULL;
    uint32_t dimension = 0;
    bool verbose = false;
    OPERATION_MODE mode = MODE_UNINITIALIZED;
    const char* mode_string = NULL;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING(ARGUMENT_INPUT_LONG, ARGUMENT_INPUT_SHORT, &input_file, ARGUMENT_INPUT_DOCUMENTATION),
        OPT_STRING(ARGUMENT_OUTPUT_LONG, ARGUMENT_OUTPUT_SHORT, &output_file, ARGUMENT_OUTPUT_DOCUMENTATION),
        OPT_INTEGER(ARGUMENT_DIMENSION_LONG, ARGUMENT_DIMENSION_SHORT, &dimension, ARGUMENT_DIMENSION_DOCUMENTATION),
        OPT_BOOLEAN(ARGUMENT_VERBOSE_LONG, ARGUMENT_VERBOSE_SHORT, &verbose, ARGUMENT_VERBOSE_DOCUMENTATION),
        OPT_STRING(ARGUMENT_MODE_LONG, ARGUMENT_MODE_SHORT, &mode_string, ARGUMENT_MODE_DOCUMENTATION),
        OPT_STRING(ARGUMENT_KEY_LONG, ARGUMENT_KEY_SHORT, &key, ARGUMENT_KEY_DOCUMENTATION),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, NULL, 0);
    argparse_describe(&argparse,
        "A program to encrypt/decrypt a variant of the Hill cipher.\nExtended with Galois fields and ciphertext expansion.",
        NULL);
    parse_result = argparse_parse(&argparse, argc, (const char**)argv);
    if ((parse_result < 0) || (!mode_string))
    {
        goto parse_error;
    }

    if (strcmp(mode_string, MODE_KEY_KEY_GENERATION_SHORT) == 0)
        mode = KEY_GENERATION_MODE;
    else if (strcmp(mode_string, MODE_KEY_DECRYPTION_KEY_GENERATION_SHORT) == 0)
        mode = DECRYPTION_KEY_GENERATION_MODE;
    else if (strcmp(mode_string, MODE_KEY_DECRYPT_SHORT) == 0)
        mode = DECRYPT_MODE;
    else if (strcmp(mode_string, MODE_KEY_ENCRYPT_SHORT) == 0)
        mode = ENCRYPT_MODE;
    else if (strcmp(mode_string, MODE_KEY_GENERATE_AND_ENCRYPT_SHORT) == 0)
        mode = GENERATE_AND_ENCRYPT_MODE;
    else if (strcmp(mode_string, MODE_KEY_GENERATE_AND_DECRYPT_SHORT) == 0)
        mode = GENERATE_AND_DECRYPT_MODE;
    else
        goto parse_error;

    switch (mode)
    {
    case KEY_GENERATION_MODE:
        if (!output_file || dimension == 0)
        {
            goto parse_error;
        }
        break;

    case DECRYPTION_KEY_GENERATION_MODE:
        if (!key)
        {
            goto parse_error;
        }
        break;

    case DECRYPT_MODE:
    case ENCRYPT_MODE:
        if (!input_file || !output_file || !key)
        {
            goto parse_error;
        }
        break;

    case GENERATE_AND_ENCRYPT_MODE:
        if (!input_file || !output_file || !key || (dimension == 0))
        {
            goto parse_error;
        }
        break;

    case GENERATE_AND_DECRYPT_MODE:
        if (!input_file || !output_file || !key)
        {
            goto parse_error;
        }
        break;

    default:
        goto parse_error;
    }

    out_args->input_file = input_file;
    out_args->output_file = output_file;
    out_args->dimension = dimension;
    out_args->verbose = verbose;
    out_args->mode = mode;
    out_args->key = key;

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;

parse_error:
    fprintf(stderr, USAGE);
    return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
    goto cleanup;
}

STATUS_CODE parse_arguments(ParsedArguments* out_args, int argc, char** argv)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!out_args)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }
        
    return_code = extract_arguments(out_args, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = validate_readable_file(out_args->input_file);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

	return_code = create_output_file_if_needed(out_args->output_file);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}
