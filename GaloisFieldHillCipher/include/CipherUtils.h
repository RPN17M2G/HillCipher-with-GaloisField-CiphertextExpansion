#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "StatusCodes.h"

#define DEFAULT_PRIME_GALOIS_FIELD 16777619
#define BYTE_SIZE 8
#define NUMBER_OF_BITS_TO_INSERT_FOR_EACH_BYTE 2
#define TWO_BITS_MASK 0x03

STATUS_CODE add_random_bits_between_bytes(double* out_value, double value);
