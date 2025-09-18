#include "GaloisFieldHillCipher.h"

int main(int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    FILE* log_file = NULL;
    OPERATION_MODE mode = MODE_UNINITIALIZED;
    GlobalArguments* global_arguments = NULL;
    void* parsed_arguments = NULL;

    return_code = parse_mode(&mode, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to parse arguments.");
        goto cleanup;
    }

    return_code = parse_global_arguments(&global_arguments, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        printf("[!] Failed to parse global arguments.\n");
        goto cleanup;
    }

    set_verbose_mode(global_arguments->verbose);

    if (global_arguments->log_file)
    {
        log_file = fopen(global_arguments->log_file, "a");
        log_add_fp(log_file, LOG_TRACE);
    }

    return_code = parse_mode_arguments(&parsed_arguments, mode, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to parse mode-specific arguments.");
        goto cleanup;
    }

    switch (mode)
    {
    case KEY_GENERATION_MODE:
        return_code = handle_key_generation_mode((KeyGenerationArguments*)parsed_arguments);
        break;
    case DECRYPTION_KEY_GENERATION_MODE:
        return_code = handle_decryption_key_generation_mode((DecryptionKeyGenerationArguments*)parsed_arguments);
        break;
    case ENCRYPT_MODE:
        return_code = handle_encrypt_mode((EncryptArguments*)parsed_arguments);
        break;
    case DECRYPT_MODE:
        return_code = handle_decrypt_mode((DecryptArguments*)parsed_arguments);
        break;
    case GENERATE_AND_ENCRYPT_MODE:
        return_code = handle_generate_and_encrypt_mode((GenerateAndEncryptArguments*)parsed_arguments);
        break;
    case GENERATE_AND_DECRYPT_MODE:
        return_code = handle_generate_and_decrypt_mode((GenerateAndDecryptArguments*)parsed_arguments);
        break;
    default:
        printf("[!] Invalid mode specified.\n");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
    }

cleanup:
    if (log_file)
    {
        fclose(log_file);
    }
    free(global_arguments);
    return return_code;
}
