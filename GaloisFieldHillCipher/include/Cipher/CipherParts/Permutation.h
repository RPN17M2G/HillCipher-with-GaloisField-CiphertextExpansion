#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdint.h>
#include <stdlib.h>

#include "StatusCodes.h"

/**
 * @brief Permutates a vector of uint8_t values based on a given permutation vector.
 *
 * @param out_vector - Pointer to the output vector that will hold the permutated values.
 * @param vector - The input vector to be permutated.
 * @param vector_size - The size of the input vector.
 * @param permutation_vector - The permutation vector that defines the order of elements in the output vector.
 * @param number_of_letters_per_element - The number of letters per element in the galois field.
 * @return
 */
STATUS_CODE permutate_uint8_vector(uint8_t** out_vector, uint8_t* vector, uint32_t vector_size, uint8_t* permutation_vector, uint32_t number_of_letters_per_element);

#endif //PERMUTATION_H
