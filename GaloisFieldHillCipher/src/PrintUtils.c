#include "PrintUtils.h"

void print_hex_vector(const uint8_t* data, size_t size, const char* prefix) {
    if (!data || !prefix) return;

    printf("%s\n", prefix);
    for (size_t i = 0; i < size; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

void print_int64_vector(const int64_t* data, size_t size, const char* prefix) {
    if (!data || !prefix) return;

    printf("%s\n", prefix);
    for (size_t i = 0; i < size; ++i) {
        printf("%ld ", data[i]);
    }
    printf("\n");
}

void print_matrix(int64_t** matrix, uint32_t dimension)
{
    for (size_t row = 0; row < dimension; ++row)
    {
        for (size_t column = 0; column < dimension; ++column)
        {
            printf("%ld ", matrix[row][column]);
        }
        printf("\n");
    }
}
