#include "Parsing/ArgumentParser.h"

STATUS_CODE extract_arguments(ParsedArguments* out_args, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* input_file = NULL;
    const char* output_file = NULL;
    const char* key = NULL;
    uint32_t dimension = 0;
    uint32_t number_of_random_bits_between_bytes = 0;
    const char* output_format_string = NULL;
    uint32_t verbose = false;
    OPERATION_MODE mode = MODE_UNINITIALIZED;
    const char* mode_string = NULL;
    FILE_FORMAT output_format = OUTPUT_FORMAT_UNINITIALIZED;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING(*ARGUMENT_INPUT_SHORT, ARGUMENT_INPUT_LONG, &input_file, ARGUMENT_INPUT_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_OUTPUT_SHORT, ARGUMENT_OUTPUT_LONG, &output_file, ARGUMENT_OUTPUT_DOCUMENTATION),
        OPT_INTEGER(*ARGUMENT_DIMENSION_SHORT, ARGUMENT_DIMENSION_LONG, &dimension, ARGUMENT_DIMENSION_DOCUMENTATION),
        OPT_BOOLEAN(*ARGUMENT_VERBOSE_SHORT, ARGUMENT_VERBOSE_LONG, &verbose, ARGUMENT_VERBOSE_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_MODE_SHORT, ARGUMENT_MODE_LONG, &mode_string, ARGUMENT_MODE_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_KEY_SHORT, ARGUMENT_KEY_LONG, &key, ARGUMENT_KEY_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_OUTPUT_FORMAT_SHORT, ARGUMENT_OUTPUT_FORMAT_LONG, &output_format_string, ARGUMENT_OUTPUT_FORMAT_DOCUMENTATION),
        OPT_INTEGER(*ARGUMENT_RANDOM_BITS_SHORT, ARGUMENT_RANDOM_BITS_LONG, &number_of_random_bits_between_bytes, ARGUMENT_RANDOM_BITS_DOCUMENTATION),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, NULL, 0);
    argparse_describe(&argparse, USAGE, NULL);
    if (argparse_parse(&argparse, argc, (const char**)argv) < 0 || !mode_string)
        goto parse_error;

    if (!output_format_string || strcmp(output_format_string, "binary") == 0)
        output_format = OUTPUT_FORMAT_BINARY;
    else if (strcmp(output_format_string, "text") == 0)
        output_format = OUTPUT_FORMAT_TEXT;
    else
        goto parse_error;

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

    if (!number_of_random_bits_between_bytes)
        number_of_random_bits_between_bytes = DEFAULT_VALUE_OF_NUMBER_OF_RANDOM_BITS_TO_ADD;

    switch (mode)
    {
    case KEY_GENERATION_MODE:
        if (!output_file || !dimension ||
            STATUS_FAILED(validate_output_file(output_file, output_format)))
            goto parse_error;
        break;

    case DECRYPTION_KEY_GENERATION_MODE:
        if (!key || !output_file || !dimension ||
            STATUS_FAILED(validate_file_is_readable(key)) ||
            STATUS_FAILED(validate_file_is_binary(key)) ||
            STATUS_FAILED(validate_output_file(output_file, output_format)) )
            goto parse_error;
        break;

    case DECRYPT_MODE:
        if (!input_file || !output_file || !key || !dimension ||
            STATUS_FAILED(validate_file_is_readable(input_file)) ||
            STATUS_FAILED(validate_file_is_binary(input_file)) ||
            STATUS_FAILED(validate_file_is_readable(key)) ||
            STATUS_FAILED(validate_file_is_binary(key)) ||
            STATUS_FAILED(validate_output_file(output_file, output_format)))
            goto parse_error;
        break;

    case ENCRYPT_MODE:
        if (!input_file || !output_file || !key || !dimension ||
            STATUS_FAILED(validate_file_is_readable(key)) ||
            STATUS_FAILED(validate_file_is_binary(key)) ||
            STATUS_FAILED(validate_file_is_readable(input_file)) ||
            STATUS_FAILED(validate_output_file(output_file, output_format)))
            goto parse_error;
        break;

    case GENERATE_AND_ENCRYPT_MODE:
        if (!input_file || !output_file || !key || !dimension ||
            STATUS_FAILED(validate_file_is_readable(input_file)) ||
            STATUS_FAILED(validate_file_is_binary(key)) ||
            STATUS_FAILED(validate_output_file(output_file, output_format)))
            goto parse_error;
        break;

    case GENERATE_AND_DECRYPT_MODE:
        if (!input_file || !output_file || !key || !dimension ||
            STATUS_FAILED(validate_file_is_readable(key)) ||
            STATUS_FAILED(validate_file_is_binary(key)) ||
            STATUS_FAILED(validate_file_is_readable(input_file)) ||
            STATUS_FAILED(validate_file_is_binary(input_file)) ||
            STATUS_FAILED(validate_output_file(output_file, output_format)))
            goto parse_error;
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
    out_args->output_format = output_format;
    out_args->number_of_random_bits_between_bytes = number_of_random_bits_between_bytes;
    out_args->input_format = STATUS_SUCCESS(validate_file_is_binary(input_file)) ? OUTPUT_FORMAT_BINARY : OUTPUT_FORMAT_TEXT;

    return STATUS_CODE_SUCCESS;

parse_error:
    fprintf(stderr, USAGE);
    return STATUS_CODE_PARSE_ARGUMENTS_FAILED;
}

STATUS_CODE parse_arguments(ParsedArguments* out_args, int argc, char** argv)
{
    if (!out_args)
        return STATUS_CODE_INVALID_ARGUMENT;

    return extract_arguments(out_args, argc, argv);
}
