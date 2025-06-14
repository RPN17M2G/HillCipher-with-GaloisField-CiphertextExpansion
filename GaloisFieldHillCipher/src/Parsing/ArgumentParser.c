#include "Parsing/ArgumentParser.h"

STATUS_CODE parse_mode(OPERATION_MODE* out_mode, const char* mode_string)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    OPERATION_MODE mode_buffer = MODE_UNINITIALIZED;

    if (!mode_string || !out_mode)
    {
        log_error("[!] Invalid argument: mode_string or out_mode is NULL in parse_mode.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    if (strcmp(mode_string, MODE_KEY_KEY_GENERATION_SHORT) == 0)
        mode_buffer = KEY_GENERATION_MODE;
    else if (strcmp(mode_string, MODE_KEY_DECRYPTION_KEY_GENERATION_SHORT) == 0)
        mode_buffer = DECRYPTION_KEY_GENERATION_MODE;
    else if (strcmp(mode_string, MODE_KEY_DECRYPT_SHORT) == 0)
        mode_buffer = DECRYPT_MODE;
    else if (strcmp(mode_string, MODE_KEY_ENCRYPT_SHORT) == 0)
        mode_buffer = ENCRYPT_MODE;
    else if (strcmp(mode_string, MODE_KEY_GENERATE_AND_ENCRYPT_SHORT) == 0)
        mode_buffer = GENERATE_AND_ENCRYPT_MODE;
    else if (strcmp(mode_string, MODE_KEY_GENERATE_AND_DECRYPT_SHORT) == 0)
        mode_buffer = GENERATE_AND_DECRYPT_MODE;
    else
    {
        log_error("[!] Invalid mode string: '%s' in parse_mode.", mode_string ? mode_string : "(null)");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }
    *out_mode = mode_buffer;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE validate_mode_args(OPERATION_MODE mode, const char* input_file, const char* output_file, const char* key, uint32_t dimension)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    switch (mode)
    {
    case KEY_GENERATION_MODE:
        if (output_file && (dimension > 0)) {
            return_code = STATUS_CODE_SUCCESS;
            goto cleanup;
        }
        log_error("[!] Missing output_file or dimension for KEY_GENERATION_MODE.");
        break;
    case DECRYPTION_KEY_GENERATION_MODE:
        if (key && output_file &&
            STATUS_SUCCESS(validate_file_is_readable(key)) &&
            STATUS_SUCCESS(validate_file_is_binary(key))) {
            return_code = STATUS_CODE_SUCCESS;
            goto cleanup;
        }
        log_error("[!] Invalid arguments or key file for DECRYPTION_KEY_GENERATION_MODE.");
        break;
    case DECRYPT_MODE:
        if (input_file && output_file && key &&
            STATUS_SUCCESS(validate_file_is_readable(input_file)) &&
            STATUS_SUCCESS(validate_file_is_readable(key)) &&
            STATUS_SUCCESS(validate_file_is_binary(key))) {
            return_code = STATUS_CODE_SUCCESS;
            goto cleanup;
        }
        log_error("[!] Invalid arguments or files for DECRYPT_MODE.");
        break;
    case ENCRYPT_MODE:
        if (input_file && output_file && key &&
            STATUS_SUCCESS(validate_file_is_readable(key)) &&
            STATUS_SUCCESS(validate_file_is_binary(key)) &&
            STATUS_SUCCESS(validate_file_is_readable(input_file))) {
            return_code = STATUS_CODE_SUCCESS;
            goto cleanup;
        }
        log_error("[!] Invalid arguments or files for ENCRYPT_MODE.");
        break;
    case GENERATE_AND_ENCRYPT_MODE:
        if (input_file && output_file && key && (dimension > 0) &&
            STATUS_SUCCESS(validate_file_is_readable(input_file)) &&
            STATUS_SUCCESS(validate_file_is_binary(key))) {
            return_code = STATUS_CODE_SUCCESS;
            goto cleanup;
        }
        log_error("[!] Invalid arguments or files for GENERATE_AND_ENCRYPT_MODE.");
        break;
    case GENERATE_AND_DECRYPT_MODE:
        if (input_file && output_file && key &&
            STATUS_SUCCESS(validate_file_is_readable(key)) &&
            STATUS_SUCCESS(validate_file_is_binary(key)) &&
            STATUS_SUCCESS(validate_file_is_readable(input_file))) {
            return_code = STATUS_CODE_SUCCESS;
            goto cleanup;
        }
        log_error("[!] Invalid arguments or files for GENERATE_AND_DECRYPT_MODE.");
        break;
    default:
        log_error("[!] Unknown operation mode in validate_mode_args.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        break;
    }
    return_code = STATUS_CODE_INVALID_ARGUMENT;
cleanup:
    return return_code;
}

STATUS_CODE get_file_format(FILE_FORMAT* out_format, const char* file)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE_FORMAT out_format_buffer = OUTPUT_FORMAT_UNINITIALIZED;
    if (!out_format) {
        log_error("[!] Invalid argument: out_format is NULL in get_file_format.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }
    out_format_buffer = OUTPUT_FORMAT_UNINITIALIZED;
    if (!file) {
        return_code = STATUS_CODE_SUCCESS;
        goto cleanup;
    }
    if (STATUS_SUCCESS(validate_file_is_binary(file)))
        out_format_buffer = OUTPUT_FORMAT_BINARY;
    else
        out_format_buffer = OUTPUT_FORMAT_TEXT;

    *out_format = out_format_buffer;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

static STATUS_CODE parse_argparse_options(
    const char** input_file, const char** output_file, const char** key, const char** log_file,
    uint32_t* dimension, uint32_t* number_of_random_bits_between_bytes, uint32_t* verbose,
    const char** mode_string, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    if (!input_file || !output_file || !key || !log_file
         || !number_of_random_bits_between_bytes || !verbose || !mode_string || !argv)
    {
        log_error("[!] Invalid argument: NULL pointer in parse_argparse_options.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_STRING(*ARGUMENT_INPUT_SHORT, ARGUMENT_INPUT_LONG, input_file, ARGUMENT_INPUT_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_OUTPUT_SHORT, ARGUMENT_OUTPUT_LONG, output_file, ARGUMENT_OUTPUT_DOCUMENTATION),
        OPT_INTEGER(*ARGUMENT_DIMENSION_SHORT, ARGUMENT_DIMENSION_LONG, dimension, ARGUMENT_DIMENSION_DOCUMENTATION),
        OPT_BOOLEAN(*ARGUMENT_VERBOSE_SHORT, ARGUMENT_VERBOSE_LONG, verbose, ARGUMENT_VERBOSE_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_MODE_SHORT, ARGUMENT_MODE_LONG, mode_string, ARGUMENT_MODE_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_KEY_SHORT, ARGUMENT_KEY_LONG, key, ARGUMENT_KEY_DOCUMENTATION),
        OPT_INTEGER(*ARGUMENT_RANDOM_BITS_SHORT, ARGUMENT_RANDOM_BITS_LONG, number_of_random_bits_between_bytes, ARGUMENT_RANDOM_BITS_DOCUMENTATION),
        OPT_STRING(*ARGUMENT_LOG_SHORT, ARGUMENT_LOG_LONG, log_file, ARGUMENT_LOG_DOCUMENTATION),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, NULL, 0);
    argparse_describe(&argparse, USAGE, NULL);
    if (argparse_parse(&argparse, argc, (const char**)argv) < 0 || !*mode_string)
    {
        log_error("[!] Failed to parse command-line arguments in parse_argparse_options.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE parse_arguments(ParsedArguments* out_args, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    const char* input_file = NULL;
    const char* output_file = NULL;
    const char* key = NULL;
    const char* log_file = NULL;
    uint32_t dimension = 0;
    uint32_t number_of_random_bits_between_bytes = 0;
    uint32_t verbose = false;
    OPERATION_MODE mode = MODE_UNINITIALIZED;
    const char* mode_string = NULL;
    FILE_FORMAT output_format = OUTPUT_FORMAT_UNINITIALIZED;
    FILE_FORMAT input_format = OUTPUT_FORMAT_UNINITIALIZED;

    if (!out_args || !argv) {
        log_error("[!] Invalid argument: out_args or argv is NULL in parse_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_argparse_options(&input_file, &output_file, &key, &log_file,
                                         &dimension, &number_of_random_bits_between_bytes, &verbose, &mode_string,
                                         argc, argv);
    if (STATUS_FAILED(return_code))
        goto parse_error;

    return_code = parse_mode(&mode, mode_string);
    if (STATUS_FAILED(return_code))
        goto parse_error;

    if (!number_of_random_bits_between_bytes)
        number_of_random_bits_between_bytes = DEFAULT_VALUE_OF_NUMBER_OF_RANDOM_BITS_TO_ADD;

    if (number_of_random_bits_between_bytes < 0)
    {
        log_error("[!] Invalid number of random bits between bytes: %u. Must be non-negative.", number_of_random_bits_between_bytes);
        goto parse_error;
    }

    return_code = validate_mode_args(mode, input_file, output_file, key, dimension);
    if (STATUS_FAILED(return_code))
        goto parse_error;

    if (log_file && STATUS_SUCCESS(validate_file_is_binary(log_file))) {
        log_error("[!] Log file must be a text file, but a binary file was provided: %s", log_file);
        goto parse_error;
    }

    return_code = get_file_format(&output_format, output_file);
    if (STATUS_FAILED(return_code))
        goto parse_error;
    return_code = get_file_format(&input_format, input_file);
    if (STATUS_FAILED(return_code))
        goto parse_error;

    out_args->input_file = input_file;
    out_args->output_file = output_file;
    out_args->dimension = dimension;
    out_args->verbose = verbose;
    out_args->mode = mode;
    out_args->key = key;
    out_args->number_of_random_bits_between_bytes = number_of_random_bits_between_bytes;
    out_args->output_format = output_format;
    out_args->input_format = input_format;
    out_args->log_file = log_file;

    return_code = STATUS_CODE_SUCCESS;
    goto cleanup;

parse_error:
    log_error("[!] Argument parsing failed. See usage below.");
    fprintf(stderr, USAGE);
    return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
cleanup:
    return return_code;
}
