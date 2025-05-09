#include "ArgumentParser.h"

STATUS_CODE validate_input_file(const char* path) 
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

STATUS_CODE extract_arguments(int argc, char** argv, ParsedArguments* out_args) 
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* input_file = NULL;
    const char* output_file = NULL;
    uint32_t dimension = 0;
    bool verbose = false;

    struct argparse_option options[] = {
        OPT_STRING(ARGUMENT_INPUT_LONG,     ARGUMENT_INPUT_SHORT,     &input_file,  ARGUMENT_INPUT_DOCENTATION),
        OPT_STRING(ARGUMENT_OUTPUT_LONG,    ARGUMENT_OUTPUT_SHORT,    &output_file, ARGUMENT_OUTPUT_DOCENTATION),
        OPT_INTEGER(ARGUMENT_DIMENSION_LONG,ARGUMENT_DIMENSION_SHORT, &dimension,   ARGUMENT_DIMENSION_DOCENTATION),
        OPT_BOOLEAN(ARGUMENT_VERBOSE_LONG,  ARGUMENT_VERBOSE_SHORT,   &verbose,     ARGUMENT_VERBOSE_DOCENTATION),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, USAGE, 0);
    argparse_describe(&argparse,
        "A program to encrypt/decrypt a variant of the Hill cipher.\nExtended with Galois fields and ciphertext expansion.",
        NULL);
    argparse_parse(&argparse, argc, (const char**)argv);

    if (!input_file || !output_file || dimension == 0)
    {
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }
        
    out_args->input_file = input_file;
    out_args->output_file = output_file;
    out_args->dimension = dimension;
    out_args->verbose = verbose;

	return_code = STATUS_CODE_SUCCESS;  
cleanup:
    return return_code;
}

STATUS_CODE parse_arguments(int argc, char** argv, ParsedArguments* out_args) 
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!out_args)
    {
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }
        
    return_code = extract_arguments(argc, argv, out_args);
    if (STATUS_FAILED(return_code))
    {
        goto cleanup;
    }

    return_code = validate_input_file(out_args->input_file);
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
