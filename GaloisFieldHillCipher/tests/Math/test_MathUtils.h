#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "unity.h"
#include "Math/MathUtils.h"
#include "Math/MatrixUtils.h"

void run_all_MathUtils_tests();

// Helper function
static int64_t** allocate_matrix(uint32_t dimension);

// Tests
void test_MathUtils_matrix_determinant_1x1();
void test_MathUtils_matrix_determinant_2x2();
void test_MathUtils_matrix_determinant_invalidArguments();
void test_MathUtils_matrix_determinant_sanity();

void test_MathUtils_build_minor_matrix_sanity();
void test_MathUtils_build_minor_matrix_3x3();

void test_MathUtils_gcd_sanity();
void test_MathUtils_gcd_large_numbers();
void test_MathUtils_gcd_multiple_edge_cases();

void test_MathUtils_is_matrix_invertible_true();
void test_MathUtils_is_matrix_invertible_false();
void test_MathUtils_is_matrix_invertible_null_arguments();

void test_MathUtils_inverse_square_matrix_2x2();
void test_MathUtils_inverse_square_matrix_noninvertible_matrix();

void test_MathUtils_multiply_matrix_with_uint8_t_vector();
void test_MathUtils_multiply_matrix_with_uint8_t_vector_negative_and_not_aligned_values();

void test_MathUtils_multiply_matrix_with_int64_t_vector();
void test_MathUtils_multiply_matrix_with_int64_t_vector_negative_and_not_aligned_values();
void test_MathUtils_multiply_matrix_with_int64_t_vector_negative_and_not_aligned_values();
