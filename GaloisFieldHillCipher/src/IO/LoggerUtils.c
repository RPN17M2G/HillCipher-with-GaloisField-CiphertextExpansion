#include "IO/LoggerUtils.h"

void log_uint8_vector(const uint8_t* data, size_t size, const char* prefix, bool is_verbose_only)
{
    size_t i = 0;
    char* buffer = NULL;
    if (!data || !prefix)
    {
        log_error("[!] Invalid argument in log_uint8_vector: %s", !data ? "data is NULL" : "prefix is NULL");
        goto cleanup;;
    }

    log_debug("Logging uint8 vector: size=%zu bytes", size);

    size_t buffer_size = size * UINT8_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    buffer = (char*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed for print buffer (size: %zu)", buffer_size);
        goto cleanup;
    }

    size_t offset = 0;
    size_t partly_offset_temp = 0;
    partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%s\n", prefix);
    if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
    {
        log_error("[!] Buffer overflow detected while logging matrix prefix.");
        goto cleanup;
    }
    offset += partly_offset_temp;
    for (i = 0; i < size && offset < buffer_size - UINT8_HEX_CHARS_PER_ELEMENT; ++i)
    {
        partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%02x ", data[i]);
        if ((i + 1) % BYTES_LINE_SIZE == 0) {  // Line break every amount of bytes
            partly_offset_temp += snprintf(buffer + offset, buffer_size - offset, "\n");
        }
        if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
        {
            log_error("[!] Buffer overflow detected while logging uint8 vector.");
            goto cleanup;
        }
        offset += partly_offset_temp;
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");

    if (is_verbose_mode()) {
        log_debug("%s", buffer);
    } else {
        log_info("%s", buffer);
    }
cleanup:
    free(buffer);
}

void log_int64_vector(const int64_t* data, size_t size, const char* prefix, bool is_verbose_only)
{
    size_t i = 0;
    size_t partly_offset_temp = 0;
    char* buffer = NULL;
    if (!data || !prefix || (0 == size))
    {
        log_error("[!] Invalid argument in log_int64_vector: %s", !data ? "data is NULL" : (!prefix ? "prefix is NULL" : "size is 0"));
        goto cleanup;
    }

    log_debug("Logging int64 vector: size=%zu elements", size);

    size_t buffer_size = size * INT64_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    if ((buffer_size - PRINT_BUFFER_EXTRA) / INT64_HEX_CHARS_PER_ELEMENT != size)
    {
        log_error("[!] Integer overflow in buffer size calculation.");
        goto cleanup;
    }
    buffer = (char*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed for print buffer (size: %zu)", buffer_size);
        goto cleanup;
    }

    size_t offset = 0;
    partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%s\n", prefix);
    if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
    {
        log_error("[!] Buffer overflow detected while logging matrix prefix.");
        goto cleanup;
    }
    offset += partly_offset_temp;
    for (i = 0; i < size && offset < buffer_size - INT64_HEX_CHARS_PER_ELEMENT; ++i)
    {
        if (data[i] > 0)
        {
            partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%02llx ", (long long)data[i]);
        }
        if ((i + 1) % NUMBERS_LINE_SIZE == 0) {  // Line break every amount of numbers
            partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "\n");
        }

        if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
        {
            log_error("[!] Buffer overflow detected while logging int64 vector.");
            goto cleanup;
        }
        offset += partly_offset_temp;
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");

    if (is_verbose_mode()) {
        log_debug("%s", buffer);
    } else {
        log_info("%s", buffer);
    }
cleanup:
    free(buffer);
}

void log_matrix(int64_t** matrix, uint32_t dimension, const char* prefix, bool is_verbose_only)
{
    char* buffer = NULL;
    size_t partly_offset_temp = 0;
    if (!matrix)
    {
        log_error("[!] Invalid argument: matrix is NULL");
        goto cleanup;
    }

    log_debug("Logging matrix: dimension=%u", dimension);

    size_t buffer_size = dimension * dimension * MATRIX_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
    buffer = (char*)malloc(buffer_size);
    if (!buffer)
    {
        log_error("[!] Memory allocation failed for print buffer (size: %zu)", buffer_size);
        goto cleanup;
    }

    size_t offset = 0;
    partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%s\n", prefix);
    if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
    {
        log_error("[!] Buffer overflow detected while logging matrix prefix.");
        goto cleanup;
    }
    offset += partly_offset_temp;
    for (size_t row = 0; row < dimension; ++row)
    {
        if (!matrix[row]) {
            log_error("[!] Invalid matrix: row %zu is NULL", row);
            goto cleanup;
        }

        for (size_t column = 0; column < dimension; ++column)
        {
            partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%6ld ", matrix[row][column]);
            if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
            {
                log_error("[!] Buffer overflow detected while logging matrix at row %zu, column %zu.", row, column);
                goto cleanup;
            }
            offset += partly_offset_temp;
        }
        partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "\n");
        if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
        {
            log_error("[!] Buffer overflow detected while logging matrix at end of row %zu.", row);
            goto cleanup;
        }
        offset += partly_offset_temp;
    }

    if (is_verbose_mode() || !is_verbose_only)
    {
        log_info("%s", buffer);
    }
cleanup:
    free(buffer);
}
