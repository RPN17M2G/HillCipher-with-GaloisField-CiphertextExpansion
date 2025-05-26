#include "test_CipherUtils.h"

void test_add_random_bits_between_bytes_sanity()
{
    // Arrange
    uint8_t input[] = {0xAA, 0xBB, 0xCC};
    uint32_t input_number_of_elements = 3;
    size_t input_len = input_number_of_elements * BYTE_SIZE;
    uint8_t lowest_six_mask = 0x3F; // 0b00111111
    uint8_t highest_six_mask = 0xFC; // 0b11111100
    uint8_t lowest_four_mask = 0x0F; // 0b00001111
    uint8_t highest_four_mask = 0xF0; // 0b11110000
    uint8_t* output = NULL;
    uint32_t output_length = 0;
    uint32_t expected_output_length = input_len + (input_number_of_elements * NUMBER_OF_RANDOM_BITS_TO_ADD);

    // Act
    STATUS_CODE status = add_random_bits_between_bytes(&output, &output_length, input, input_len);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(expected_output_length, output_length);
    TEST_ASSERT_EQUAL(output[0], input[0]);
    TEST_ASSERT_EQUAL(output[1] & lowest_six_mask, (input[1] & highest_six_mask) >> 2);
    TEST_ASSERT_EQUAL(output[2] & lowest_four_mask, (input[2] & highest_four_mask >> 4));

    free(output);
}

void test_add_random_bits_between_bytes_EmptyInput()
{
    // Arrange
    uint8_t* output = NULL;
    uint32_t output_bit_size = 0;
    uint8_t input[] = {};
    uint32_t input_bit_length = 0;

    // Act
    STATUS_CODE status = add_random_bits_between_bytes(&output, &output_bit_size, input, input_bit_length);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(0, output_bit_size);

    free(output);
}

void test_pad_to_length_sanity()
{
    // Arrange
    uint8_t input[] = {1, 2, 3, 4};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint32_t target_bit_length = 8 * BYTE_SIZE;
    uint32_t block_bit_size = 8 * BYTE_SIZE;
    uint8_t* output = NULL;
    uint32_t output_bit_length = 0;

    // Act
    STATUS_CODE status = pad_to_length(&output, &output_bit_length, input, input_bit_length, target_bit_length, block_bit_size);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(target_bit_length, output_bit_length);
    TEST_ASSERT_EQUAL(output[input_bit_length / BYTE_SIZE], PADDING_MAGIC);

    free(output);
}

void test_pad_to_length_ExactBlock()
{
    // Arrange
    uint8_t input[] = {1, 2, 3, 4};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint32_t block_bit_size = 4 * BYTE_SIZE;
    uint32_t target_bit_length = 4 * BYTE_SIZE;
    uint8_t* output = NULL;
    uint32_t output_bit_length = 0;

    // Act
    STATUS_CODE status = pad_to_length(&output, &output_bit_length, input, input_bit_length, target_bit_length, block_bit_size);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(8 * BYTE_SIZE, output_bit_length); // Will add a full block
    TEST_ASSERT_EQUAL(output[input_bit_length / BYTE_SIZE], PADDING_MAGIC);

    free(output);
}

void test_remove_padding_sanity()
{
    // Arrange
    uint8_t input[] = {1, 2, 3, PADDING_MAGIC, 0x00, 0x00, 0x00, 0x00};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint8_t* output = NULL;
    uint32_t output_bit_length = 0;

    // Act
    STATUS_CODE status = remove_padding(&output, &output_bit_length, input, input_bit_length);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(3 * BYTE_SIZE, output_bit_length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(input, output, 3);

    free(output);
}

void test_remove_padding_InvalidPadding()
{
    // Arrange
    uint8_t input[] = {1, 2, 3, 0x05, 0x06, 0x07};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint8_t* output = NULL;
    uint32_t output_bit_length = 0;

    // Act
    STATUS_CODE status = remove_padding(&output, &output_bit_length, input, input_bit_length);

    // Assert
    TEST_ASSERT_NOT_EQUAL(STATUS_CODE_SUCCESS, status);
    free(output);
}

void test_pad_to_length_BlockSize1()
{
    // Arrange
    uint8_t input[] = {0x00};
    uint32_t input_bit_length = 1 * BYTE_SIZE;
    uint32_t block_bit_size = 1 * BYTE_SIZE;
    uint32_t target_bit_length = 1 * BYTE_SIZE;
    uint8_t* output = NULL;
    uint32_t output_bit_length = 0;
    uint8_t expected[] = {0x00, PADDING_MAGIC};

    // Act
    STATUS_CODE status = pad_to_length(&output, &output_bit_length, input, input_bit_length, target_bit_length, block_bit_size);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(2 * BYTE_SIZE, output_bit_length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 2);

    free(output);
}

void test_pad_to_length_LargePadding()
{
    // Arrange
    uint8_t input[] = {0x01};
    uint32_t input_bit_length = 1 * BYTE_SIZE;
    uint32_t block_bit_size = 8 * BYTE_SIZE;
    uint32_t target_bit_length = 8 * BYTE_SIZE;
    uint8_t* output = NULL;
    uint32_t output_bit_length = 0;
    uint8_t expected[] = {0x01, PADDING_MAGIC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // Act
    STATUS_CODE status = pad_to_length(&output, &output_bit_length, input, input_bit_length, target_bit_length, block_bit_size);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(8 * BYTE_SIZE, output_bit_length);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, 8);

    free(output);
}

void test_remove_padding_ZeroLength()
{
    // Arrange
    uint8_t input[] = {};
    uint32_t input_bit_length = 0;
    uint8_t* output = NULL;
    uint32_t output_bit_length = 0;

    // Act
    STATUS_CODE status = remove_padding(&output, &output_bit_length, input, input_bit_length);

    // Assert
    TEST_ASSERT_NOT_EQUAL(STATUS_CODE_SUCCESS, status);
    free(output);
}

void test_divide_uint8_t_into_blocks_sanity()
{
    // Arrange
    uint8_t input[] = {1, 2, 3, 4, 5, 6};
    uint8_t expected[3][2] = {{1, 2}, {3, 4}, {5, 6}};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint32_t block_bit_size = 2 * BYTE_SIZE;
    uint8_t** blocks = NULL;
    uint32_t num_blocks = 0;

    // Act
    STATUS_CODE status = divide_uint8_t_into_blocks(&blocks, &num_blocks, input, input_bit_length, block_bit_size);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(3, num_blocks);
    for (uint32_t i = 0; i < num_blocks; ++i)
    {
        TEST_ASSERT_NOT_NULL(blocks[i]);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected[i], blocks[i], 2);
    }

    for (uint32_t i = 0; i < num_blocks; ++i)
    {
        free(blocks[i]);
    }
    free(blocks);
}

void test_divide_uint8_t_into_blocks_UnevenSize()
{
    // Arrange
    uint8_t input[] = {1, 2, 3, 4, 5};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint32_t block_bit_size = 2 * BYTE_SIZE;
    uint8_t** blocks = NULL;
    uint32_t num_blocks = 0;

    // Act
    STATUS_CODE status = divide_uint8_t_into_blocks(&blocks, &num_blocks, input, input_bit_length, block_bit_size);

    // Assert
    TEST_ASSERT_NOT_EQUAL(STATUS_CODE_SUCCESS, status);
}

void test_divide_int64_t_into_blocks_sanity()
{
    // Arrange
    int64_t input[] = {1, 2, 3, 4, 5, 6};
    int64_t expected[3][2] = {{1, 2}, {3, 4}, {5, 6}};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint32_t block_size = 2 * BYTE_SIZE * sizeof(int64_t);
    int64_t** blocks = NULL;
    uint32_t num_blocks = 0;

    // Act
    STATUS_CODE status = divide_int64_t_into_blocks(&blocks, &num_blocks, input, input_bit_length, block_size);

    // Assert
    TEST_ASSERT_EQUAL(STATUS_CODE_SUCCESS, status);
    TEST_ASSERT_EQUAL(3, num_blocks);
    for (uint32_t i = 0; i < num_blocks; ++i)
    {
        TEST_ASSERT_NOT_NULL(blocks[i]);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected[i], blocks[i], 2);
    }

    for (uint32_t i = 0; i < num_blocks; ++i)
    {
        free(blocks[i]);
    }
    free(blocks);
}

void test_divide_int64_t_into_blocks_UnevenSize()
{
    // Arrange
    int64_t input[] = {1, 2, 3, 4, 5};
    uint32_t input_bit_length = sizeof(input) * BYTE_SIZE;
    uint32_t block_bit_size = 2 * sizeof(int64_t) * BYTE_SIZE;
    int64_t** blocks = NULL;
    uint32_t num_blocks = 0;

    // Act
    STATUS_CODE status = divide_int64_t_into_blocks(&blocks, &num_blocks, input, input_bit_length, block_bit_size);

    // Assert
    TEST_ASSERT_NOT_EQUAL(STATUS_CODE_SUCCESS, status);
}

void run_all_CipherUtils_tests()
{
    RUN_TEST(test_add_random_bits_between_bytes_sanity);
    RUN_TEST(test_add_random_bits_between_bytes_EmptyInput);
    RUN_TEST(test_pad_to_length_sanity);
    RUN_TEST(test_pad_to_length_ExactBlock);
    RUN_TEST(test_remove_padding_sanity);
    RUN_TEST(test_remove_padding_InvalidPadding);
    RUN_TEST(test_pad_to_length_BlockSize1);
    RUN_TEST(test_pad_to_length_LargePadding);
    RUN_TEST(test_remove_padding_ZeroLength);
    RUN_TEST(test_divide_uint8_t_into_blocks_sanity);
    RUN_TEST(test_divide_uint8_t_into_blocks_UnevenSize);
    RUN_TEST(test_divide_int64_t_into_blocks_sanity);
    RUN_TEST(test_divide_int64_t_into_blocks_UnevenSize);
}
