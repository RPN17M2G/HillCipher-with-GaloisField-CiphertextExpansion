#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "StatusCodes.h"

#define DEFAULT_PRIME_GALOIS_FIELD 16777619
#define BYTE_SIZE 8

STATUS_CODE add_random_bits_between_bytes(double* out_value, double value);
