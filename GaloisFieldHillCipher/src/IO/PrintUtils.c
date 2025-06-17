#include "IO/PrintUtils.h"

void print_uint8_vector(const uint8_t* data, size_t size, const char* prefix)
{
    size_t i = 0;
    if (!data || !prefix) {
        log_error("[!] Invalid argument in print_uint8_vector.");
        return;
    }

    size_t buffer_size = size * UINT8_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    char* buffer = (char*)malloc(buffer_size);
    if (!buffer) {
        log_error("[!] Memory allocation failed in print_uint8_vector.");
        return;
    }

    size_t offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "%s\n", prefix);
    for (i = 0; i < size && offset < buffer_size - UINT8_HEX_CHARS_PER_ELEMENT; ++i)
    {
        offset += snprintf(buffer + offset, buffer_size - offset, "%02x ", data[i]);
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");
    log_info("%s", buffer);
    free(buffer);
}

void print_int64_vector(const int64_t* data, size_t size, const char* prefix)
{
    size_t i = 0;
    if (!data || !prefix) {
        log_error("[!] Invalid argument in print_int64_vector.");
        return;
    }

    size_t buffer_size = size * INT64_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    char* buffer = (char*)malloc(buffer_size);
    if (!buffer) {
        log_error("[!] Memory allocation failed in print_int64_vector.");
        return;
    }

    size_t offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "%s\n", prefix);
    for (i = 0; i < size && offset < buffer_size - INT64_HEX_CHARS_PER_ELEMENT; ++i)
    {
        if (data[i] > 0)
        {
            offset += snprintf(buffer + offset, buffer_size - offset, "%02llx ", (long long)data[i]);
        }
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");
    log_info("%s", buffer);
    free(buffer);
}

void print_matrix(int64_t** matrix, uint32_t dimension)
{
    size_t row = 0, column = 0;
    if (!matrix) {
        log_error("[!] Invalid argument in print_matrix.");
        return;
    }
    size_t buffer_size = dimension * dimension * MATRIX_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    char* buffer = (char*)malloc(buffer_size);
    if (!buffer) {
        log_error("[!] Memory allocation failed in print_matrix.");
        return;
    }

    size_t offset = 0;
    for (row = 0; row < dimension; ++row)
    {
        for (column = 0; column < dimension && offset < buffer_size - MATRIX_HEX_CHARS_PER_ELEMENT; ++column)
        {
            offset += snprintf(buffer + offset, buffer_size - offset, "%ld ", matrix[row][column]);
        }
        offset += snprintf(buffer + offset, buffer_size - offset, "\n");
    }
    log_info("%s", buffer);
    free(buffer);
}
