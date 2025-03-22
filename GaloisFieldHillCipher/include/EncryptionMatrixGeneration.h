#pragma once

#include <stdint.h>

#include "StatusCodes.h"
#include <sodium.h>


STATUS_CODE generate_encryption_matrix(uint32_t*** out_matrix, uint32_t dimentation, uint32_t prime_field);
