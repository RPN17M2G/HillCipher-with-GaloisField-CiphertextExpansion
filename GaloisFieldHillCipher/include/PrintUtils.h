#pragma once

#include <stdint.h>
#include <stdio.h>

void print_uint8_vector(const uint8_t* data, size_t size, const char* prefix);
void print_int64_vector(const int64_t* data, size_t size, const char* prefix);
void print_matrix(int64_t** matrix, uint32_t dimension);
