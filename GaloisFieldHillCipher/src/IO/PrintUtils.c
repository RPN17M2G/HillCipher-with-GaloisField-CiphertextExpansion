#include "../../include/IO/PrintUtils.h"

void print_uint8_vector(const uint8_t* data, size_t size, const char* prefix)
{
    size_t i = 0;
    if (!data || !prefix) return;

    printf("%s\n", prefix);
    for (i = 0; i < size; ++i)
    {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

void print_int64_vector(const int64_t* data, size_t size, const char* prefix)
{
    size_t i = 0;
    if (!data || !prefix) return;

    printf("%s\n", prefix);
    for (i = 0; i < size; ++i)
    {
        if (data[i] > 0)
        {
            printf("%02x ", data[i]);
        }
    }
    printf("\n");
}

void print_matrix(int64_t** matrix, uint32_t dimension)
{
    size_t row = 0, column = 0;
    for (row = 0; row < dimension; ++row)
    {
        for (column = 0; column < dimension; ++column)
        {
            printf("%ld ", matrix[row][column]);
        }
        printf("\n");
    }
}
