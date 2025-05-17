#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StatusCodes.h"
#include "MatrixUtils.h"

STATUS_CODE serialize_matrix(uint8_t** out_data, uint32_t* out_size, int64_t** matrix, uint32_t dimension);
STATUS_CODE deserialize_matrix(int64_t*** out_matrix, uint32_t dimension, const uint8_t* data, uint32_t size);

