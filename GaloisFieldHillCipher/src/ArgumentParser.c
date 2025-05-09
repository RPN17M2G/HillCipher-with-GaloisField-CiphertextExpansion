#include "ArgumentParser.h"

static struct argp_option options[] = {
    {ARGUMENT_INPUT_LONG,     ARGUMENT_INPUT_SHORT,     "FILE",   0, ARGUMENT_INPUT_DOCENTATION},
    {ARGUMENT_OUTPUT_LONG,    ARGUMENT_OUTPUT_SHORT,    "FILE",   0, ARGUMENT_OUTPUT_DOCENTATION},
    {ARGUMENT_DIMENSION_LONG, ARGUMENT_DIMENSION_SHORT, "UINT32", 0, ARGUMENT_DIMENSION_DOCENTATION},
    {ARGUMENT_VERBOSE_LONG,   ARGUMENT_VERBOSE_SHORT,   0,        0, ARGUMENT_VERBOSE_DOCENTATION},
    {0}
};

static error_t parse_option(int key, char* arg, struct argp_state* state) {
    InternalState* internal = state->input;
    ParsedArguments* result = internal->args;

    switch (key) {
    case ARGUMENT_INPUT_SHORT:
        result->input_file = arg;
        break;
    case ARGUMENT_OUTPUT_SHORT:
        result->output_file = arg;
        break;
    case ARGUMENT_DIMENSION_SHORT: {
        char* end;
        unsigned long val = strtoul(arg, &end, 10);
        if (*end || val > UINT32_MAX) 
        {
            snprintf(internal->error, MAX_ERROR_MSG_LEN, "Invalid value for --%s", ARGUMENT_DIMENSION_LONG);
            return ARGP_ERR_UNKNOWN;
        }
        result->dimension = (uint32_t)val;
        break;
    }
    case ARGUMENT_VERBOSE_SHORT:
        result->verbose = true;
        break;
    case ARGP_KEY_END:
        if (!result->input_file || !result->output_file)
        {
            snprintf(internal->error, MAX_ERROR_MSG_LEN, "--%s and --%s are required",
                ARGUMENT_INPUT_LONG, ARGUMENT_OUTPUT_LONG);
            return ARGP_ERR_UNKNOWN;
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {
    .options = options,
    .parser = parse_option,
    .args_doc = "",
    .doc = ""
};

STATUS_CODE parse_arguments(int argc, char** argv, ParsedArguments* out_args) {
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	if (out_args == NULL)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	// Allocate memory for error message
	uint8_t* error_msg = (uint8_t*)malloc(MAX_ERROR_MSG_LEN);
	if (error_msg == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}
	memset(error_msg, 0, MAX_ERROR_MSG_LEN);

	*out_args = (ParsedArguments){ 0 };

    InternalState internal = { .args = out_args, .error = error_msg };
    
    int result = argp_parse(&argp, argc, argv, 0, 0, &internal);
    if (result != 0) 
    {
		return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    // Check input file exists and is readable
    if (access(out_args->input_file, R_OK) != 0) 
    {
        return_code = STATUS_CODE_INPUT_FILE_DOESNT_EXISTS_OR_NOT_READBLE;
        goto cleanup;
    }

    // Check output file: create if not exists
    int fd = open(out_args->output_file, O_CREAT | O_WRONLY, 0644);
    if (fd < 0) 
    {
        return_code = STATUS_CODE_COULDNT_CREATE_OUTPUT_FILE;
        goto cleanup;
    }
    close(fd);

cleanup:
    return return_code;
}
