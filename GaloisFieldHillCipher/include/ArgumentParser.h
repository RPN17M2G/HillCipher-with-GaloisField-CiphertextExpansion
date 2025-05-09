#pragma once

#include <argparse.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "StatusCodes.h"

#define MAX_ERROR_MSG_LEN (256)
#define DECIMAL_BASE (10)

#define ARGUMENT_INPUT_SHORT 'i'
#define ARGUMENT_OUTPUT_SHORT 'o'
#define ARGUMENT_DIMENSION_SHORT 'd'
#define ARGUMENT_VERBOSE_SHORT 'v'

#define ARGUMENT_INPUT_LONG "input_file"
#define ARGUMENT_OUTPUT_LONG "output_file"
#define ARGUMENT_DIMENSION_LONG "dimension"
#define ARGUMENT_VERBOSE_LONG "verbose"

#define ARGUMENT_INPUT_DOCENTATION "Path to input file"
#define ARGUMENT_OUTPUT_DOCENTATION "Path to output file"
#define ARGUMENT_DIMENSION_DOCENTATION "Dimension (uint32_t)"
#define ARGUMENT_VERBOSE_DOCENTATION "Enable verbose output"

#define USAGE "GaloisFieldHillCipher [options]"

typedef struct {
    const char* input_file;
    const char* output_file;
    uint32_t dimension;
    bool verbose;
} ParsedArguments;


STATUS_CODE create_output_file_if_needed(const char* path);
STATUS_CODE validate_input_file(const char* path);
STATUS_CODE extract_arguments(int argc, char** argv, ParsedArguments* out_args);


STATUS_CODE parse_arguments(int argc, char** argv, ParsedArguments* out_args);
