#include "IO/PrintUtils.h"

void log_uint8_vector(const uint8_t* data, size_t size, const char* prefix, bool is_verbose_only)
{
    size_t i = 0;
    if (!data || !prefix)
    {
        log_error("[!] Invalid argument in log_uint8_vector: %s", !data ? "data is NULL" : "prefix is NULL");
        return;
    }

    log_debug("Printing uint8 vector: size=%zu bytes", size);

    size_t buffer_size = size * UINT8_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    char* buffer = (char*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed for print buffer (size: %zu)", buffer_size);
        return;
    }

    size_t offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "%s\n", prefix);
    for (i = 0; i < size && offset < buffer_size - UINT8_HEX_CHARS_PER_ELEMENT; ++i)
    {
        offset += snprintf(buffer + offset, buffer_size - offset, "%02x ", data[i]);
        if ((i + 1) % 16 == 0) {  // Line break every 16 bytes
            offset += snprintf(buffer + offset, buffer_size - offset, "\n");
        }
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");

    if (is_verbose_mode())
    {
        printf("%s", buffer);
    }
    free(buffer);
}

void print_int64_vector(const int64_t* data, size_t size, const char* prefix, bool is_verbose_only)
{
    size_t i = 0;
    if (!data || !prefix)
    {
        log_error("[!] Invalid argument in print_int64_vector: %s", !data ? "data is NULL" : "prefix is NULL");
        return;
    }

    log_debug("Printing int64 vector: size=%zu elements", size);

    size_t buffer_size = size * INT64_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    char* buffer = (char*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed for print buffer (size: %zu)", buffer_size);
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
        if ((i + 1) % 8 == 0) {  // Line break every 8 numbers
            offset += snprintf(buffer + offset, buffer_size - offset, "\n");
        }
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");

    if (is_verbose_mode())
    {
        printf("%s", buffer);
    }
    free(buffer);
}

void print_matrix(int64_t** matrix, uint32_t dimension, const char* prefix, bool is_verbose_only)
{
    if (!matrix)
    {
        log_error("[!] Invalid argument: matrix is NULL");
        return;
    }

    log_debug("Printing matrix: dimension=%u", dimension);

    size_t buffer_size = dimension * dimension * MATRIX_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    char* buffer = (char*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed for print buffer (size: %zu)", buffer_size);
        return;
    }

    size_t offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "%s\n", prefix);

    for (size_t row = 0; row < dimension; ++row)
    {
        if (!matrix[row]) {
            log_error("[!] Invalid matrix: row %zu is NULL", row);
            free(buffer);
            return;
        }

        for (size_t column = 0; column < dimension; ++column)
        {
            offset += snprintf(buffer + offset, buffer_size - offset, "%6ld ", matrix[row][column]);
        }
        offset += snprintf(buffer + offset, buffer_size - offset, "\n");
    }

    if (is_verbose_mode() || !is_verbose_only)
    {
        printf("%s", buffer);
    }
    free(buffer);
}
