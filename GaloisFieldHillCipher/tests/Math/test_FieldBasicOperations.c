#include "test_FieldBasicOperations.h"

void setUp() {}
void tearDown() {}

void test_FieldBasicOperations_Addition_ZeroElement()
{
    // Arrange
    int64_t first_element = 0;
    int64_t second_element = 5;

    // Act
    int64_t result = add_over_galois_field(first_element, second_element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(second_element, result);
}

void test_FieldBasicOperations_Addition_WrapAround()
{
    // Arrange
    int64_t first_element = 6;
    int64_t second_element = 3;

    // Act
    int64_t result = add_over_galois_field(first_element, second_element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64((TEST_FIELD + first_element + second_element) % TEST_FIELD, result);
}

void test_FieldBasicOperations_Multiplication_ZeroElement()
{
    // Arrange
    int64_t zero_element = 0;
    int64_t second_element = 6;

    // Act
    int64_t result = multiply_over_galois_field(zero_element, second_element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(zero_element, result);
}

void test_FieldBasicOperations_Multiplication_WrapAround()
{
    // Arrange
    int64_t first_element = 5;
    int64_t second_element = 5;

    // Act
    int64_t result = multiply_over_galois_field(first_element, second_element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64((first_element * second_element) % TEST_FIELD, result);
}

void test_FieldBasicOperations_Negation_ZeroElement()
{
    // Arrange
    int64_t element = 0;

    // Act
    int64_t result = negate_over_galois_field(element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(element, result);
}

void test_FieldBasicOperations_Negation_ExactMultipleOfField()
{
    // Arrange
    int64_t element = 14;

    // Act
    int64_t result = negate_over_galois_field(element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(0, result);
}

void test_FieldBasicOperations_Alignment_PositiveElement()
{
    // Arrange
    int64_t element = 10;
    int64_t expected_result = 3;

    // Act
    int64_t result = align_to_galois_field(element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(expected_result, result);
}

void test_FieldBasicOperations_Alignment_NegativeElement()
{
    // Arrange
    int64_t element = -8;

    // Act
    int64_t result = align_to_galois_field(element, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64((TEST_FIELD + element) % TEST_FIELD, result);
}

void test_FieldBasicOperations_Power_ExponentIsZero()
{
    // Arrange
    int64_t base = 5;
    int64_t exponent = 0;
    int64_t expected_result = 1;

    // Act
    int64_t result = raise_power_over_galois_field(base, exponent, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(expected_result, result);
}

void test_FieldBasicOperations_Power_BaseIsZero()
{
    // Arrange
    int64_t base = 0;
    int64_t exponent = 5;

    // Act
    int64_t result = raise_power_over_galois_field(base, exponent, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(0, result);
}

void test_FieldBasicOperations_Power_BaseEqualsField()
{
    // Arrange
    int64_t base = TEST_FIELD;
    int64_t exponent = 2;
    int64_t expected_result = 0;

    // Act
    int64_t result = raise_power_over_galois_field(base, exponent, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(expected_result, result);
}

void test_FieldBasicOperations_Power_LargeExponent()
{
    // Arrange
    int64_t base = 2;
    int64_t exponent = 20;
    int64_t expected_result = 1;
    for (int64_t i = 0; i < exponent; ++i)
        expected_result = (expected_result * base) % TEST_FIELD;

    // Act
    int64_t result = raise_power_over_galois_field(base, exponent, TEST_FIELD);

    // Assert
    TEST_ASSERT_EQUAL_INT64(expected_result, result);
}

void run_all_FieldBasicOperations_tests()
{
    RUN_TEST(test_FieldBasicOperations_Addition_ZeroElement);
    RUN_TEST(test_FieldBasicOperations_Addition_WrapAround);

    RUN_TEST(test_FieldBasicOperations_Multiplication_ZeroElement);
    RUN_TEST(test_FieldBasicOperations_Multiplication_WrapAround);

    RUN_TEST(test_FieldBasicOperations_Negation_ZeroElement);
    RUN_TEST(test_FieldBasicOperations_Negation_ExactMultipleOfField);

    RUN_TEST(test_FieldBasicOperations_Alignment_PositiveElement);
    RUN_TEST(test_FieldBasicOperations_Alignment_NegativeElement);

    RUN_TEST(test_FieldBasicOperations_Power_ExponentIsZero);
    RUN_TEST(test_FieldBasicOperations_Power_BaseIsZero);
    RUN_TEST(test_FieldBasicOperations_Power_BaseEqualsField);
    RUN_TEST(test_FieldBasicOperations_Power_LargeExponent);
}