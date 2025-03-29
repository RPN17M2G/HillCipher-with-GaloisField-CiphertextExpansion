#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "StatusCodes.h"

#define DEFAULT_PRIME_GALOIS_FIELD 16777619
#define BYTE_SIZE 8
#define TWO_BITS_MASK 0x03
#define NUMBER_OF_RANDOM_BITS_TO_ADD 2

STATUS_CODE add_random_bits_between_bytes(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length);
STATUS_CODE remove_random_bits_between_bytes(uint8_t** out, uint32_t* out_size, uint8_t* value, uint32_t value_length);
