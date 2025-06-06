#pragma once
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"
#include "Cipher/BlockDividing.h"

void test_add_random_bits_between_bytes_Sanity();
void test_add_random_bits_between_bytes_EmptyInput();
void test_pad_to_length_sanity();
void test_pad_to_length_ExactBlock();
void test_remove_padding_sanity();
void test_remove_padding_InvalidPadding();
void test_pad_to_length_BlockSize1();
void test_pad_to_length_LargePadding();
void test_remove_padding_ZeroLength();
void test_divide_uint8_t_into_blocks_sanity();
void test_divide_uint8_t_into_blocks_UnevenSize();
void test_divide_int64_t_into_blocks_sanity();
void test_divide_int64_t_into_blocks_UnevenSize();

void run_all_CipherUtils_tests();

