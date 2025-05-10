#pragma once

#include <argparse.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Modes.h"
#include "StatusCodes.h"

#define MAX_ERROR_MSG_LEN (256)
#define DECIMAL_BASE (10)

#define ARGUMENT_INPUT_SHORT 'i'
#define ARGUMENT_OUTPUT_SHORT 'o'
#define ARGUMENT_DIMENSION_SHORT 'd'
#define ARGUMENT_VERBOSE_SHORT 'v'
#define ARGUMENT_MODE_SHORT 'm'
#define ARGUMENT_KEY_SHORT 'k'

#define ARGUMENT_INPUT_LONG "input_file"
#define ARGUMENT_OUTPUT_LONG "output_file"
#define ARGUMENT_DIMENSION_LONG "dimension"
#define ARGUMENT_VERBOSE_LONG "verbose"
#define ARGUMENT_MODE_LONG "mode"
#define ARGUMENT_KEY_LONG "key"

#define ARGUMENT_INPUT_DOCUMENTATION "Path to input file"
#define ARGUMENT_OUTPUT_DOCUMENTATION "Path to output file"
#define ARGUMENT_DIMENSION_DOCUMENTATION "Dimension (uint32_t)"
#define ARGUMENT_VERBOSE_DOCUMENTATION "Enable verbose output"
#define ARGUMENT_MODE_DOCUMENTATION "Cipher mode of operation(encrypt/decrypt/key generation)"
#define ARGUMENT_KEY_DOCUMENTATION "Path to key file"

typedef struct {
    const char* input_file;
    const char* output_file;
    const char* key;
    uint32_t dimension;
    bool verbose;
    OPERATION_MODE mode;
} ParsedArguments;


STATUS_CODE create_output_file_if_needed(const char* path);
STATUS_CODE validate_input_file(const char* path);
STATUS_CODE extract_arguments(int argc, char** argv, ParsedArguments* out_args);


STATUS_CODE parse_arguments(int argc, char** argv, ParsedArguments* out_args);
