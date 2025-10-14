#include "IO/PrintUtils.h"

#include "IO/LoggerUtils.h"

void print_uint8_vector(const uint8_t* data, size_t size, const char* prefix, bool is_verbose_only)
{
    size_t i = 0;
    char* buffer = NULL;
    size_t partly_offset_temp = 0;
    if (!data || !prefix)
    {
        log_error("[!] Invalid argument in log_uint8_vector: %s", !data ? "data is NULL" : "prefix is NULL");
        goto cleanup;
    }

    log_debug("Printing uint8 vector: size=%zu bytes", size);

    size_t buffer_size = size * UINT8_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
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
        log_error("[!] Buffer overflow detected while logging matrix at end of row %zu.", row);
        goto cleanup;
    }
    offset += partly_offset_temp;
    for (i = 0; i < size && offset < buffer_size - UINT8_HEX_CHARS_PER_ELEMENT; ++i)
    {
        offset += snprintf(buffer + offset, buffer_size - offset, "%02x ", data[i]);
        if ((i + 1) % BYTES_LINE_SIZE == 0) // Line break every amount of bytes
        {
            offset += snprintf(buffer + offset, buffer_size - offset, "\n");
        }
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");

    if (is_verbose_mode())
    {
        printf("%s", buffer);
    }
cleanup:
    free(buffer);
}

void print_int64_vector(const int64_t* data, size_t size, const char* prefix, bool is_verbose_only)
{
    size_t i = 0;
    char* buffer = NULL;
    size_t partly_offset_temp = 0;
    if (!data || !prefix)
    {
        log_error("[!] Invalid argument in print_int64_vector: %s", !data ? "data is NULL" : "prefix is NULL");
        goto cleanup;
    }

    log_debug("Printing int64 vector: size=%zu elements", size);

    size_t buffer_size = size * INT64_HEX_CHARS_PER_ELEMENT + PRINT_BUFFER_EXTRA;
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
        log_error("[!] Buffer overflow detected while logging matrix at end of row %zu.", row);
        goto cleanup;
    }
    offset += partly_offset_temp;
    for (i = 0; i < size && offset < buffer_size - INT64_HEX_CHARS_PER_ELEMENT; ++i)
    {
        if (data[i] > 0)
        {
            partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%02llx ", (long long)data[i]);
        }
        if ((i + 1) % NUMBERS_LINE_SIZE == 0) // Line break every 8 numbers
        {
            partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "\n");
        }
    }
    snprintf(buffer + offset, buffer_size - offset, "\n");

    if (is_verbose_mode())
    {
        printf("%s", buffer);
    }
cleanup:
    free(buffer);
}

void print_matrix(int64_t** matrix, uint32_t dimension, const char* prefix, bool is_verbose_only)
{
    char* buffer = NULL;
    size_t partly_offset_temp = 0;
    if (!matrix)
    {
        log_error("[!] Invalid argument: matrix is NULL");
        goto cleanup;
    }

    log_debug("Printing matrix: dimension=%u", dimension);

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
        log_error("[!] Buffer overflow detected while logging matrix at end of row %zu.", row);
        goto cleanup;
    }
    offset += partly_offset_temp;
    for (size_t row = 0; row < dimension; ++row)
    {
        if (!matrix[row])
        {
            log_error("[!] Invalid matrix: row %zu is NULL", row);
            free(buffer);
            goto cleanup;
        }

        for (size_t column = 0; column < dimension; ++column)
        {
            partly_offset_temp = snprintf(buffer + offset, buffer_size - offset, "%I64d ", matrix[row][column]);
            if ((partly_offset_temp < 0) || (partly_offset_temp >= (buffer_size - offset)))
            {
                log_error("[!] Buffer overflow detected while logging matrix at end of row %zu.", row);
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
        printf("%s", buffer);
    }
cleanup:
    free(buffer);
}
