#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileValidation.h"
#include "StatusCodes.h"
#include "CipherUtils.h"

STATUS_CODE write_uint8_to_file(const char* filepath, const uint8_t* data, uint32_t size);
STATUS_CODE read_uint8_from_file(uint8_t** out_data, uint32_t* out_size, const char* filepath);
STATUS_CODE write_int64_to_file(const char* filepath, const int64_t* data, uint32_t size);
STATUS_CODE read_int64_from_file(int64_t** out_data, uint32_t* out_size, const char* filepath);
