#ifndef SECRETS_H
#define SECRETS_H

#include <stdint.h>

#define NUMBER_OF_UINT32_SECRETS (5)

struct Secrets {
    int64_t** key_matrix;
    uint32_t dimension;
    int64_t** error_vectors;
    uint32_t number_of_error_vectors;
    uint32_t number_of_random_bits_to_add;
    uint32_t prime_field;
    uint8_t** ascii_mapping;
    uint32_t number_of_letters_for_each_digit_ascii_mapping;
    uint8_t* permutation_vector;
} typedef Secrets;

#endif //SECRETS_H
