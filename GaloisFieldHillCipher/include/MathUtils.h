#pragma once

#include <stdint.h>

#include "StatusCodes.h"

#define IS_EVEN(number) (number % 2 == 0)
#define IS_ODD(number) (number % 2 != 0)

STATUS_CODE matrix_determinant(const uint32_t** matrix, uint32_t dimentaion, int64_t* out_determinant);
STATUS_CODE square_matrix_inverse(const uint32_t** matrix, uint32_t dimentaion, uint32_t prime_field, uint32_t*** out_inverse_matrix);
STATUS_CODE matrix_multipication_with_vector(const uint32_t** matrix, const uint32_t* vector, uint32_t dimentaion, uint32_t prime_field, uint32_t** out_vector);

STATUS_CODE free_matrix(uint32_t** matrix, const uint32_t dimentaion);
STATYS_CODE build_minor_matrix(const uint32_t** matrix, uint32_t dimentaion, uint32_t row, uint32_t column, uint32_t*** out_matrix);

STATUS_CODE gcd(int64_t first_element, int64_t second_element, int64_t* out_gcd);
STATUS_CODE is_matrix_invertible(const uint32_t** matrix, uint32_t dimentaion, uint32_t prime_field, bool* out_is_invertible);