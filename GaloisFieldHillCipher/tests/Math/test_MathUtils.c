#include "test_MathUtils.h"

static int64_t** allocate_matrix(uint32_t dimension) 
{
    int64_t** matrix = malloc(dimension * sizeof(int64_t*));
    for (uint32_t i = 0; i < dimension; i++) 
{
        matrix[i] = malloc(dimension * sizeof(int64_t));
    }
    return matrix;
}

void test_MathUtils_matrix_determinant_1x1()
{
    uint32_t dimension = 1;
    uint32_t prime_field = 101;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 42;

    int64_t actual_laplace_expansion = 0;
    int64_t actual_gauss_jordan = 0;
    int64_t expected = 42;

    STATUS_CODE laplace_status = matrix_determinant_over_galois_field_laplace_expansion(&actual_laplace_expansion, matrix, dimension, prime_field);
    STATUS_CODE gauss_status = matrix_determinant_over_galois_field_gauss_jordan(&actual_gauss_jordan, matrix, dimension, prime_field);

    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, laplace_status);
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, gauss_status);
    TEST_ASSERT_EQUAL_INT64(expected, actual_laplace_expansion);
    TEST_ASSERT_EQUAL_INT64(expected, actual_gauss_jordan);

    free_matrix(matrix, dimension);
}

void test_MathUtils_matrix_determinant_2x2()
{
    uint32_t dimension = 2;
    uint32_t prime_field = 11;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 1; matrix[0][1] = 2;
    matrix[1][0] = 3; matrix[1][1] = 4;

    int64_t actual_laplace_expansion = 0;
    int64_t actual_gauss_jordan = 0;
    int64_t expected = 9;

    STATUS_CODE laplace_status = matrix_determinant_over_galois_field_laplace_expansion(&actual_laplace_expansion, matrix, dimension, prime_field);
    STATUS_CODE gauss_status = matrix_determinant_over_galois_field_gauss_jordan(&actual_gauss_jordan, matrix, dimension, prime_field);

    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, laplace_status);
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, gauss_status);
    TEST_ASSERT_EQUAL_INT64(expected, actual_laplace_expansion);
    TEST_ASSERT_EQUAL_INT64(expected, actual_gauss_jordan);

    free_matrix(matrix, dimension);
}

void test_MathUtils_matrix_determinant_invalidArguments()
{
    uint32_t dimension = 2;
    uint32_t prime_field = 7;
    int64_t actual_laplace_expansion = 0;
    int64_t actual_gauss_jordan = 0;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 1; matrix[0][1] = 2;
    matrix[1][0] = 3; matrix[1][1] = 4;

    // Act
    STATUS_CODE laplace_expansion_null_status = matrix_determinant_over_galois_field_laplace_expansion(NULL, NULL, dimension, prime_field);
    STATUS_CODE gauss_jordan_null_status = matrix_determinant_over_galois_field_gauss_jordan(NULL, NULL, dimension, prime_field);
    STATUS_CODE laplace_expansion_invalid_dimension_status = matrix_determinant_over_galois_field_laplace_expansion(&actual_laplace_expansion, matrix, 0, prime_field);
    STATUS_CODE gauss_jordan_invalid_dimension_status = matrix_determinant_over_galois_field_gauss_jordan(&actual_gauss_jordan, matrix, 0, prime_field);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGUMENT, laplace_expansion_null_status);
    TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGUMENT, gauss_jordan_null_status);
    TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGUMENT, laplace_expansion_invalid_dimension_status);
    TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGUMENT, gauss_jordan_invalid_dimension_status);

    free_matrix(matrix, dimension);
}

void test_MathUtils_matrix_determinant_sanity() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 11;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;
    int64_t actual_laplace_expansion = 0;
    int64_t actual_gauss_jordan = 0;

    // Determinant mod 11: (1*4 - 2*3) = (4 - 6) mod 11 = 9
    int64_t expected = 9;

    // Act
    STATUS_CODE laplace_expansion_status = matrix_determinant_over_galois_field_laplace_expansion(&actual_laplace_expansion, matrix, dimension, prime_field);
    STATUS_CODE gauss_jordan_status = matrix_determinant_over_galois_field_gauss_jordan(&actual_gauss_jordan, matrix, dimension, prime_field);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, laplace_expansion_status);
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, gauss_jordan_status);
    TEST_ASSERT_EQUAL_INT64(expected, actual_laplace_expansion);
    TEST_ASSERT_EQUAL_INT64(expected, actual_gauss_jordan);

    free_matrix(matrix, dimension);
}

void test_MathUtils_build_minor_matrix_3x3() 
{
    // Arrange
    uint32_t dimension = 3;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[0][2] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    matrix[1][2] = 6;
    matrix[2][0] = 7;
    matrix[2][1] = 8;
    matrix[2][2] = 9;

    int64_t** minor_matrix = NULL;

    // Act
    STATUS_CODE status = build_minor_matrix(&minor_matrix, matrix, dimension, 0, 0);

    // Assert minor matrix is 2x2 without row 0 and column 0
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(minor_matrix);
    TEST_ASSERT_EQUAL_INT64(5, minor_matrix[0][0]);
    TEST_ASSERT_EQUAL_INT64(6, minor_matrix[0][1]);
    TEST_ASSERT_EQUAL_INT64(8, minor_matrix[1][0]);
    TEST_ASSERT_EQUAL_INT64(9, minor_matrix[1][1]);

    free_matrix(minor_matrix, dimension - 1);
    free_matrix(matrix, dimension);
}

void test_MathUtils_gcd_sanity() 
{
    // Arrange
    int64_t gcd_result = 0;

    // Act
    STATUS_CODE status = gcd(&gcd_result, 48, 18);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL_INT64(6, gcd_result);
}

void test_MathUtils_is_matrix_invertible_true() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 11;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 1; matrix[0][1] = 2;
    matrix[1][0] = 3; matrix[1][1] = 4;
    bool invertible = false;

    // Act
    STATUS_CODE status = is_matrix_invertible(&invertible, matrix, dimension, prime_field);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_TRUE(invertible);

    free_matrix(matrix, dimension);
}

void test_MathUtils_is_matrix_invertible_false() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 7;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 2; matrix[0][1] = 4;
    matrix[1][0] = 1; matrix[1][1] = 2;
    bool invertible = true;

    // Act
    STATUS_CODE status = is_matrix_invertible(&invertible, matrix, dimension, prime_field);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_FALSE(invertible);

    free_matrix(matrix, dimension);
}

void test_MathUtils_inverse_square_matrix_2x2() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 11;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 4;
    int64_t** inverse_matrix = NULL;

    // Act
    STATUS_CODE status = inverse_square_matrix_adjugate_method(&inverse_matrix, matrix, dimension, prime_field);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(inverse_matrix);

    TEST_ASSERT_EQUAL_INT64(9, inverse_matrix[0][0]);
    TEST_ASSERT_EQUAL_INT64(1, inverse_matrix[0][1]);
    TEST_ASSERT_EQUAL_INT64(7, inverse_matrix[1][0]);
    TEST_ASSERT_EQUAL_INT64(5, inverse_matrix[1][1]);

    free_matrix(inverse_matrix, dimension);
    free_matrix(matrix, dimension);
}

void test_MathUtils_multiply_matrix_with_uint8_t_vector() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 11;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 2;
    matrix[0][1] = 3;
    matrix[1][0] = 4;
    matrix[1][1] = 5;
    uint8_t vector[2] = 
{1, 2};
    int64_t* result_vector = NULL;
    // Expected:
    // row 0: (2*1 + 3*2) mod 11 = (2 + 6) = 8
    // row 1: (4*1 + 5*2) mod 11 = (4 + 10) mod 11 = 3
    int64_t expected[2] = 
{8, 3};

    // Act
    STATUS_CODE status = multiply_matrix_with_uint8_t_vector(&result_vector, matrix, vector, dimension, prime_field);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(result_vector);
    TEST_ASSERT_EQUAL_INT64(expected[0], result_vector[0]);
    TEST_ASSERT_EQUAL_INT64(expected[1], result_vector[1]);

    free(result_vector);
    free_matrix(matrix, dimension);
}

void test_MathUtils_multiply_matrix_with_int64_t_vector() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 11;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = 2; matrix[0][1] = 3;
    matrix[1][0] = 4; matrix[1][1] = 5;
    int64_t vector[2] = 
{1, 2};
    uint8_t* result_vector = NULL;
    // Expected:
    // row 0: (2*1 + 3*2) mod 11 = (2 + 6) = 8
    // row 1: (4*1 + 5*2) mod 11 = (4 + 10) mod 11 = 3
    uint8_t expected[2] = 
{8, 3};

    // Act
    STATUS_CODE status = multiply_matrix_with_int64_t_vector(&result_vector, matrix, vector, dimension, prime_field);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(result_vector);
    TEST_ASSERT_EQUAL_UINT8(expected[0], result_vector[0]);
    TEST_ASSERT_EQUAL_UINT8(expected[1], result_vector[1]);

    free(result_vector);
    free_matrix(matrix, dimension);
}

void test_MathUtils_gcd_multiple_edge_cases() 
{
    // Arrange
    int64_t gcd_result = 0;
    STATUS_CODE status;

    // gcd(0, 0) undefined, expect failure or 0
    status = gcd(&gcd_result, 0, 0);
    TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGUMENT, status);

    // gcd(a, 0) = a
    status = gcd(&gcd_result, 42, 0);
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL_INT64(42, gcd_result);

    // gcd(0, b) = b
    status = gcd(&gcd_result, 0, 30);
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL_INT64(30, gcd_result);

    // gcd negative inputs, should treat abs values
    status = gcd(&gcd_result, -54, 24);
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL_INT64(6, gcd_result);
}

void test_MathUtils_is_matrix_invertible_null_arguments()
{
    // Arrange
    bool invertible;
    STATUS_CODE status;

    // Act & Assert
    status = is_matrix_invertible(NULL, NULL, 0, 0);
    TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGUMENT, status);

    int64_t** matrix = allocate_matrix(1);
    status = is_matrix_invertible(&invertible, NULL, 1, 2);
    TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGUMENT, status);

    free_matrix(matrix, 1);
}

void test_MathUtils_inverse_square_matrix_noninvertible_matrix() 
{
    // Arrange
    int64_t** matrix = allocate_matrix(2);
    matrix[0][0] = 2;
    matrix[0][1] = 4;
    matrix[1][0] = 1;
    matrix[1][1] = 2;
    int64_t** inverse = NULL;

    // Act
    STATUS_CODE status = inverse_square_matrix_adjugate_method(&inverse, matrix, 2, 7);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_MATRIX_NOT_INVERTIBLE, status);
    TEST_ASSERT_NULL(inverse);

    free_matrix(matrix, 2);
}

void test_MathUtils_multiply_matrix_with_uint8_t_vector_negative_and_not_aligned_values() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 5;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = -1; matrix[0][1] = 6; // -1 mod 5 = 4, 6 mod 5 = 1
    matrix[1][0] = 7; matrix[1][1] = -3; // 7 mod 5 = 2, -3 mod 5 = 2
    uint8_t vector[2] = 
{255, 0}; // 255 mod 5 = 0 (since 255 % 5 = 0)
    int64_t* result_vector = NULL;

    // Act
    STATUS_CODE status = multiply_matrix_with_uint8_t_vector(&result_vector, matrix, vector, dimension, prime_field);

    // (4*0 + 1*0) mod 5 = 0
    // (2*0 + 2*0) mod 5 = 0
    int64_t expected[] = {0, 0};

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(result_vector);
    TEST_ASSERT_EQUAL_INT64(expected[0], result_vector[0]);
    TEST_ASSERT_EQUAL_INT64(expected[1], result_vector[1]);

    free(result_vector);
    free_matrix(matrix, dimension);
}

void test_MathUtils_multiply_matrix_with_int64_t_vector_negative_and_not_aligned_values() 
{
    // Arrange
    uint32_t dimension = 2;
    uint32_t prime_field = 5;
    int64_t** matrix = allocate_matrix(dimension);
    matrix[0][0] = -1; matrix[0][1] = 6;
    matrix[1][0] = 7; matrix[1][1] = -3;
    int64_t vector[] = {-1, 6};
    uint8_t* result_vector = NULL;

    // Act
    STATUS_CODE status = multiply_matrix_with_int64_t_vector(&result_vector, matrix, vector, dimension, prime_field);

    // Calculations:
    // row0: (4 * 4 + 1 * 1) mod 5 = (16 + 1) mod 5 = 17 mod 5 = 2
    // row1: (2 * 4 + 2 * 1) mod 5 = (8 + 2) mod 5 = 10 mod 5 = 0
    uint8_t expected[] = {2, 0};

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(result_vector);
    TEST_ASSERT_EQUAL_UINT8(expected[0], result_vector[0]);
    TEST_ASSERT_EQUAL_UINT8(expected[1], result_vector[1]);

    free(result_vector);
    free_matrix(matrix, dimension);
}

void test_MathUtils_build_minor_matrix_sanity() 
{
    // Arrange
    uint32_t dimension = 3;
    int64_t** matrix = allocate_matrix(dimension);
    int64_t** minor_matrix = NULL;
    STATUS_CODE status;
    matrix[0][0] = 1; matrix[0][1] = 2; matrix[0][2] = 3;
    matrix[1][0] = 4; matrix[1][1] = 5; matrix[1][2] = 6;
    matrix[2][0] = 7; matrix[2][1] = 8; matrix[2][2] = 9;

    // Act
    status = build_minor_matrix(&minor_matrix, matrix, dimension, 0, 1);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_NOT_NULL(minor_matrix);

    TEST_ASSERT_EQUAL_INT64(4, minor_matrix[0][0]);
    TEST_ASSERT_EQUAL_INT64(6, minor_matrix[0][1]);
    TEST_ASSERT_EQUAL_INT64(7, minor_matrix[1][0]);
    TEST_ASSERT_EQUAL_INT64(9, minor_matrix[1][1]);

    free_matrix(minor_matrix, dimension - 1);
    free_matrix(matrix, dimension);
}

void test_MathUtils_gcd_large_numbers() 
{
    // Arrange
    int64_t gcd_result = 0;
    STATUS_CODE status;
    int64_t expected_result = 9;

    // Act
    status = gcd(&gcd_result, 1234567890, 987654321);

    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL_INT64(expected_result, gcd_result);
}

void run_all_MathUtils_tests()
{
    RUN_TEST(test_MathUtils_matrix_determinant_1x1);
    RUN_TEST(test_MathUtils_matrix_determinant_2x2);
    RUN_TEST(test_MathUtils_matrix_determinant_invalidArguments);
    RUN_TEST(test_MathUtils_matrix_determinant_sanity);

    RUN_TEST(test_MathUtils_build_minor_matrix_sanity);
    RUN_TEST(test_MathUtils_build_minor_matrix_3x3);

    RUN_TEST(test_MathUtils_gcd_sanity);
    RUN_TEST(test_MathUtils_gcd_large_numbers);
    RUN_TEST(test_MathUtils_gcd_multiple_edge_cases);

    RUN_TEST(test_MathUtils_is_matrix_invertible_true);
    RUN_TEST(test_MathUtils_is_matrix_invertible_false);
    RUN_TEST(test_MathUtils_is_matrix_invertible_null_arguments);

    RUN_TEST(test_MathUtils_inverse_square_matrix_2x2);
    RUN_TEST(test_MathUtils_inverse_square_matrix_noninvertible_matrix);

    RUN_TEST(test_MathUtils_multiply_matrix_with_uint8_t_vector);
    RUN_TEST(test_MathUtils_multiply_matrix_with_uint8_t_vector_negative_and_not_aligned_values);

    RUN_TEST(test_MathUtils_multiply_matrix_with_int64_t_vector);
    RUN_TEST(test_MathUtils_multiply_matrix_with_int64_t_vector_negative_and_not_aligned_values);
    RUN_TEST(test_MathUtils_multiply_matrix_with_int64_t_vector_negative_and_not_aligned_values);
}
