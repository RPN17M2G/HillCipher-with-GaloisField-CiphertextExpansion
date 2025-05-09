#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "StatusCodes.h"

#define MAX_ERROR_MSG_LEN (256)

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

typedef struct {
    const char* input_file;
    const char* output_file;
    uint32_t dimension;
    bool verbose;
} ParsedArguments;

typedef struct {
	ParsedArguments* args;
    uint8_t* error;
} InternalState;

STATUS_CODE parse_arguments(int argc, char** argv, ParsedArguments* out_args);
