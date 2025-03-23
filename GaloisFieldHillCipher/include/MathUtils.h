#pragma once

#include <stdint.h>

#include "StatusCodes.h"

#define IS_EVEN(number) (number % 2 == 0)
#define IS_ODD(number) (number % 2 != 0)

STATUS_CODE matrix_determinant(const long double** matrix, uint32_t dimentaion, long double* out_determinant);

// TODO : check implementation and maybe use GauessJordan instead.Moreover, can't use uint, use int
STATUS_CODE square_matrix_inverse(const long double** matrix, uint32_t dimentaion, uint32_t prime_field, long double*** out_inverse_matrix);

// TODO : Implement
STATUS_CODE matrix_multipication_with_vector(const long double** matrix, const long double* vector, uint32_t dimentaion, uint32_t prime_field, long double** out_vector);

STATUS_CODE free_matrix(long double** matrix, const uint32_t dimentaion);
STATUS_CODE build_minor_matrix(const long double** matrix, uint32_t dimentaion, uint32_t row, uint32_t column, long double*** out_matrix);

STATUS_CODE gcd(int64_t first_element, int64_t second_element, int64_t* out_gcd);
STATUS_CODE is_matrix_invertible(const long double** matrix, uint32_t dimentaion, uint32_t prime_field, bool* out_is_invertible);