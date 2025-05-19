#pragma once
#include <stdint.h>

#include "unity.h"
#include "Math/FieldBasicOperations.h"

#define TEST_FIELD 7

void setUp();
void tearDown();

void run_all_FieldBasicOperations_tests();

void test_FieldBasicOperations_Addition_ZeroElement();
void test_FieldBasicOperations_Addition_WrapAround();
void test_FieldBasicOperations_Multiplication_ZeroElement();
void test_FieldBasicOperations_Multiplication_WrapAround();
void test_FieldBasicOperations_Negation_ZeroElement();
void test_FieldBasicOperations_Negation_ExactMultipleOfField();
void test_FieldBasicOperations_Alignment_PositiveElement();
void test_FieldBasicOperations_Alignment_NegativeElement();
void test_FieldBasicOperations_Power_ExponentIsZero();
void test_FieldBasicOperations_Power_BaseIsZero();
void test_FieldBasicOperations_Power_BaseEqualsField();
void test_FieldBasicOperations_Power_LargeExponent();

