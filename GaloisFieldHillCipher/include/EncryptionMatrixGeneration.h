#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "StatusCodes.h"

STATUS_CODE generate_encryption_matrix(long double*** out_matrix, uint32_t dimentation, uint32_t prime_field);
