#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "StatusCodes.h"

#define DEFAULT_PRIME_GALOIS_FIELD 16777619
#define BYTE_SIZE 8
#define TWO_BITS_MASK 0x03
#define NUMBER_OF_RANDOM_BYTES_TO_ADD 4

typedef struct {
    uint8_t* data;
} CIPHERTEXT_EXPANSION_VECTOR;

STATUS_CODE add_random_bits_between_bytes(CIPHERTEXT_EXPANSION_VECTOR* out_value, double value);


