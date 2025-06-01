#include "GaloisFieldHillCipher.h"


#include "include/Cipher/CipherModeHandlers.h"

int main(int argc, char** argv)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    ParsedArguments parsed_args = { 0 };
    return_code = parse_arguments(&parsed_args, argc, argv);
    if (STATUS_FAILED(return_code))
    {
        printf("[!] Failed to parse arguments. Command line: ");
        for (int i = 0; i < argc; ++i) {
            printf("%s ", argv[i]);
        }
        printf("\n");
        return return_code;
    }

    if (parsed_args.verbose)
    {
        printf("[*] Verbose mode is enabled.\n");
    }

    switch (parsed_args.mode)
    {
    case KEY_GENERATION_MODE:
        return_code = handle_key_generation_mode(&parsed_args);
        break;
    case DECRYPTION_KEY_GENERATION_MODE:
        return_code = handle_decryption_key_generation_mode(&parsed_args);
        break;
    case ENCRYPT_MODE:
        return_code = handle_encrypt_mode(&parsed_args);
        break;
    case DECRYPT_MODE:
        return_code = handle_decrypt_mode(&parsed_args);
        break;
    case GENERATE_AND_ENCRYPT_MODE:
        return_code = handle_generate_and_encrypt_mode(&parsed_args);
        break;
    case GENERATE_AND_DECRYPT_MODE:
        return_code = handle_generate_and_decrypt_mode(&parsed_args);
        break;
    default:
        printf("[!] Invalid mode specified.\n");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
    }

cleanup:
    return return_code;
}
