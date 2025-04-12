#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sodium.h>

#include "StatusCodes.h"

STATUS_CODE initialize_sodium_library();
STATUS_CODE generate_secure_random_number(uint32_t* out_number, uint32_t minimum_value, uint32_t maximum_value);
STATUS_CODE generate_secure_random_double(double* out_number, uint32_t minimum_value, uint32_t maximum_value);
