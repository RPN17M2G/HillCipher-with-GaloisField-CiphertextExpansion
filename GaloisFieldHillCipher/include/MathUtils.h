#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "StatusCodes.h"

#define IS_EVEN(number) (number % 2 == 0)
#define IS_ODD(number) (number % 2 != 0)

STATUS_CODE matrix_determinant(const double** matrix, uint32_t dimentaion, double* out_determinant);
STATUS_CODE square_matrix_inverse(const double** matrix, uint32_t dimentaion, uint32_t prime_field, double*** out_inverse_matrix);
STATUS_CODE matrix_multipication_with_vector(const double** matrix, const double* vector, uint32_t dimentaion, uint32_t prime_field, double** out_vector);

STATUS_CODE free_matrix(double** matrix, const uint32_t dimentaion);
STATUS_CODE build_minor_matrix(const double** matrix, uint32_t dimentaion, uint32_t row, uint32_t column, double*** out_matrix);

STATUS_CODE gcd(double first_element, double second_element, double* out_gcd);
STATUS_CODE is_matrix_invertible(const double** matrix, uint32_t dimentaion, uint32_t prime_field, bool* out_is_invertible);