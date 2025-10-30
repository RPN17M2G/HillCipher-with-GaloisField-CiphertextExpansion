#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "MatrixUtils.h"
#include "FieldBasicOperations.h"
#include "Cipher/CipherParts/CSPRNG.h"
#include "Math/MatrixDeterminant.h"
#include "log.h"

/**
 * @brief Calculates the greatest common divisor (GCD) of two elements.
 *
 * @param out_gcd - Pointer to the output GCD value.
 * @param first_element - First element.
 * @param second_element - Second element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE gcd(int64_t* out_gcd, int64_t first_element, int64_t second_element);

/**
 * @brief Adding two vectors over finite field.
 *
 * @param out_vector - Pointer to the output vector.
 * @param first_vector - First vector to add.
 * @param second_vector - Second vector to add.
 * @param length - Length of vectors.
 * @param prime_field - The prime field to operate on.
 * @return
 */
STATUS_CODE add_two_vectors_over_gf(int64_t** out_vector, int64_t* first_vector, int64_t* second_vector, uint32_t length, uint32_t prime_field);

/**
 * @brief Substructing two vectors over finite field.
 *
 * @param out_vector - Pointer to the output vector.
 * @param first_vector - First vector to substruct from.
 * @param second_vector - Second vector to substruct.
 * @param length - Length of vectors.
 * @param prime_field - The prime field to operate on.
 * @return
 */
STATUS_CODE substruct_two_vectors_over_gf(int64_t** out_vector, int64_t* first_vector, int64_t* second_vector, uint32_t length, uint32_t prime_field);

/**
 * @brief Checks if a number is prime.
 *
 * @param number - The number to check.
 * @return true if the number is prime, false otherwise.
 */
bool is_prime(int64_t number);

#endif
