#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StatusCodes.h"

STATUS_CODE write_to_file(const char* filename, const uint8_t* data, uint32_t size);
STATUS_CODE read_from_file(uint8_t** out_data, uint32_t* out_size, const char* filename);
