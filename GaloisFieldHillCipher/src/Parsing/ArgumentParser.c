#include "Parsing/ArgumentParser.h"

STATUS_CODE parse_generic_options(struct argparse_option* options, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    char** filtered_argv = NULL;
    int filtered_argc = 0;
    const char** relevant_flags = NULL;

    if (!options || !argv)
    {
        log_error("[!] Invalid arguments passed to parse_generic_options.");
        return STATUS_CODE_INVALID_ARGUMENT;
    }

    return_code = extract_relevant_flags(&relevant_flags, options);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to extract relevant flags from options.");
        goto cleanup;
    }

    return_code = filter_relevant_flags(&filtered_argv, &filtered_argc, argc, argv, relevant_flags);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to filter relevant flags from argv.");
        goto cleanup;
    }

    struct argparse argparse;
    argparse_init(&argparse, options, NULL, 0);
    argparse_describe(&argparse, USAGE_STRING, NULL);

    if (argparse_parse(&argparse, filtered_argc, (const char**)filtered_argv) < 0)
    {
        log_error("[!] Failed to parse command-line arguments.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(filtered_argv);
    free(relevant_flags);
    return return_code;
}

STATUS_CODE extract_relevant_flags(const char*** out_relevant_flags, struct argparse_option* options)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t number_of_options = 0;
    struct argparse_option* option = NULL;
    const char** relevant_flags = NULL;
    size_t index = 0;

    if (!options || !out_relevant_flags)
    {
        log_error("[!] Invalid arguments passed to extract_relevant_flags.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    for (option = options; option->type != ARGPARSE_OPT_END; ++option)
    {
        number_of_options++;
    }

    relevant_flags = (const char**)malloc((number_of_options * NUMBER_OF_FLAGS_FOR_EACH_OPTION + 1) * sizeof(char*));
    if (!relevant_flags)
    {
        log_error("[!] Memory allocation failed for relevant_flags.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (option = options; option->type != ARGPARSE_OPT_END; ++option)
    {
        if (option->long_name)
        {
            relevant_flags[index++] = option->long_name;
        }
        if (option->short_name)
        {
            relevant_flags[index++] = &option->short_name;
        }
    }
    relevant_flags[index] = NULL;

    *out_relevant_flags = relevant_flags;
    relevant_flags = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(relevant_flags);
    return return_code;
}

STATUS_CODE parse_mode(OPERATION_MODE* out_mode, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    OPERATION_MODE mode;
    const char* mode_string = NULL;

    struct argparse_option options[] = {
        OPT_STRING(*FLAG_MODE_SHORT, FLAG_MODE, &mode_string, FLAG_MODE_DESCRIPTION),
        OPT_END()
    };

    if (!out_mode || !argv)
    {
        log_error("[!] Invalid argument: out_mode or argv is NULL in parse_mode.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to parse mode from arguments.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (strcmp(mode_string, MODE_KEY_GENERATION) == 0)
    {
        mode = KEY_GENERATION_MODE;
    }
    else if (strcmp(mode_string, MODE_DECRYPTION_KEY_GENERATION) == 0)
    {
        mode = DECRYPTION_KEY_GENERATION_MODE;
    }
    else if (strcmp(mode_string, MODE_ENCRYPT) == 0)
    {
        mode = ENCRYPT_MODE;
    }
    else if (strcmp(mode_string, MODE_DECRYPT) == 0)
    {
        mode = DECRYPT_MODE;
    }
    else if (strcmp(mode_string, MODE_GENERATE_AND_ENCRYPT) == 0)
    {
        mode = GENERATE_AND_ENCRYPT_MODE;
    }
    else if (strcmp(mode_string, MODE_GENERATE_AND_DECRYPT) == 0)
    {
        mode = GENERATE_AND_DECRYPT_MODE;
    }
    else
    {
        log_error("[!] Invalid mode specified: %s. Available modes: %s, %s, %s, %s, %s, %s.",
                  mode_string,
                  MODE_KEY_GENERATION,
                  MODE_DECRYPTION_KEY_GENERATION,
                  MODE_ENCRYPT,
                  MODE_DECRYPT,
                  MODE_GENERATE_AND_ENCRYPT,
                  MODE_GENERATE_AND_DECRYPT);
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *out_mode = mode;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE parse_global_arguments(GlobalArguments** out_arguments, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    bool verbose = false;
    const char* log_file = NULL;
    GlobalArguments* parsed_arguments = NULL;

    struct argparse_option options[] = {
        OPT_BOOLEAN(*FLAG_VERBOSE_SHORT, FLAG_VERBOSE, &verbose, FLAG_VERBOSE_DESCRIPTION),
        OPT_STRING(*FLAG_LOG_FILE_SHORT, FLAG_LOG_FILE, &log_file, FLAG_LOG_FILE_DESCRIPTION),
        OPT_END()
    };

    if (!out_arguments || !argv)
    {
        log_error("[!] Invalid argument: output_verbose or argv is NULL in parse_verbose.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    return_code = parse_generic_options(options, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to parse verbose from arguments.");
        return_code = STATUS_CODE_PARSE_ARGUMENTS_FAILED;
        goto cleanup;
    }

    if (STATUS_FAILED(validate_file_is_writeable(log_file)))
    {
        log_error("[!] Log file is not writeable or does not exist: %s", log_file ? log_file : "(null)");
        return_code = STATUS_CODE_INPUT_FILE_DOESNT_EXISTS_OR_NOT_WRITEBLE;
        goto cleanup;
    }

    parsed_arguments = malloc(sizeof(GlobalArguments));
    if (!parsed_arguments)
    {
        log_error("[!] Memory allocation failed for GlobalArguments.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    parsed_arguments->verbose = verbose;
    parsed_arguments->log_file = log_file;

    *out_arguments = parsed_arguments;
    parsed_arguments = NULL;
    return_code = STATUS_CODE_SUCCESS;
cleanup:
    free(parsed_arguments);
    return return_code;
}

STATUS_CODE parse_mode_arguments(void** out_mode_arguments, OPERATION_MODE mode, int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

    if (!out_mode_arguments)
    {
        log_error("[!] Invalid argument: out_mode_arguments is NULL in parse_mode_arguments.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    switch (mode)
    {
        case KEY_GENERATION_MODE:
            KeyGenerationArguments* key_gen_args = NULL;
            return_code = parse_key_generation_arguments(&key_gen_args, argc, argv);
            *out_mode_arguments = (void*)key_gen_args;
            break;

        case DECRYPTION_KEY_GENERATION_MODE:
            DecryptionKeyGenerationArguments* decrypt_key_gen_args = NULL;
            return_code = parse_decryption_key_generation_arguments(&decrypt_key_gen_args, argc, argv);
            *out_mode_arguments = (void*)decrypt_key_gen_args;
            break;

        case DECRYPT_MODE:
            DecryptArguments* decrypt_args = NULL;
            return_code = parse_decrypt_arguments(&decrypt_args, argc, argv);
            *out_mode_arguments = (void*)decrypt_args;
            break;

        case ENCRYPT_MODE:
            EncryptArguments* encrypt_args = NULL;
            return_code = parse_encrypt_arguments(&encrypt_args, argc, argv);
            *out_mode_arguments = (void*)encrypt_args;
            break;

        case GENERATE_AND_ENCRYPT_MODE:
            GenerateAndEncryptArguments* gen_encrypt_args = NULL;
            return_code = parse_generate_and_encrypt_arguments(&gen_encrypt_args, argc, argv);
            *out_mode_arguments = (void*)gen_encrypt_args;
            break;

        case GENERATE_AND_DECRYPT_MODE:
            GenerateAndDecryptArguments* gen_decrypt_args = NULL;
            return_code = parse_generate_and_decrypt_arguments(&gen_decrypt_args, argc, argv);
            *out_mode_arguments = (void*)gen_decrypt_args;
            break;

        default:
            log_error("[!] Unknown operation mode in parse_mode_arguments.");
            return_code = STATUS_CODE_INVALID_ARGUMENT;
            break;
    }

cleanup:
    return return_code;
}

STATUS_CODE filter_relevant_flags(char*** out_filtered_argv, int* out_filtered_argc, int argc, char** argv, const char** relevant_flags)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    char** filtered_argv = NULL;
    char** filtered_argv_tmp = NULL;
    int filtered_argc = 0;
    size_t argument_index = 0;
    char* flag_to_compare = NULL;
    bool copy_flag = true;

    if (!out_filtered_argv || !out_filtered_argc || !argv || !relevant_flags)
    {
        log_error("[!] Invalid arguments passed to filter_relevant_flags.");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    filtered_argv = (char**)malloc(sizeof(char*));
    if (!filtered_argv)
    {
        log_error("[!] Memory allocation failed for filtered_argv.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (argument_index = 0; argument_index < argc; ++argument_index)
    {
        for (const char** flag = relevant_flags; *flag != NULL; ++flag)
        {
            if (argv[argument_index][0] == '-') // A flag argument
            {
                copy_flag = false;

                flag_to_compare = malloc((strlen(*flag) + MEMORY_FOR_FLAG_PREFIX) * sizeof(char));
                if (!flag_to_compare)
                {
                    log_error("[!] Memory allocation failed for flag_to_compare.");
                    return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
                    goto cleanup;
                }

                if (strlen(*flag) == 1)
                    snprintf(flag_to_compare, strlen(*flag) + MEMORY_FOR_FLAG_PREFIX, "-%s", *flag);
                else
                    snprintf(flag_to_compare, strlen(*flag) + MEMORY_FOR_FLAG_PREFIX, "--%s", *flag);

                if (strncmp(argv[argument_index], flag_to_compare, strlen(flag_to_compare)) == 0)
                {
                    copy_flag = true;
                }
                free(flag_to_compare);
                flag_to_compare = NULL;
            }

            if (copy_flag)
            {
                filtered_argv_tmp = (char**)realloc(filtered_argv, (filtered_argc + 1 + 1) * sizeof(char*));
                if (!filtered_argv_tmp)
                {
                    log_error("[!] Memory reallocation failed for filtered_argv.");
                    free(flag_to_compare);
                    return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
                    goto cleanup;
                }
                filtered_argv = filtered_argv_tmp;
                filtered_argv_tmp = NULL;

                filtered_argv[filtered_argc++] = argv[argument_index];
                break;
            }
        }
    }

    *out_filtered_argv = filtered_argv;
    filtered_argv = NULL;
    *out_filtered_argc = filtered_argc;

    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(flag_to_compare);
    free(filtered_argv);
    return return_code;
}