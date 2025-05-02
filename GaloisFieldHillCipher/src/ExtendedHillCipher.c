#include "ExtendedHillCipher.h"

STATUS_CODE encrypt(int64_t** out_ciphertext, uint32_t* out_ciphertext_bit_size, int64_t** encryption_matrix, uint32_t dimentation, uint32_t prime_field, uint8_t* plaintext_vector, uint32_t vector_bit_size)
{
	uint32_t block_size_in_bits = (BYTE_SIZE * dimentation);

	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	if ((NULL == out_ciphertext) || (NULL == out_ciphertext_bit_size) || (NULL == encryption_matrix) || (NULL == plaintext_vector))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	uint8_t* random_inserted_plaintext = NULL;
	uint32_t random_inserted_plaintext_bit_size = 0;

	if (STATUS_FAILED(add_random_bits_between_bytes(&random_inserted_plaintext, &random_inserted_plaintext_bit_size, plaintext_vector, vector_bit_size)))
	{
		return_code = STATUS_CODE_COULDNT_ADD_RANDOM_BITS;
		goto cleanup;
	}

	uint8_t* padded_plaintext = NULL;
	uint32_t padded_plaintext_bit_size = 0;

	if (STATUS_FAILED(pad_to_length(&padded_plaintext,
		&padded_plaintext_bit_size, 
		random_inserted_plaintext, 
		random_inserted_plaintext_bit_size, 
		random_inserted_plaintext_bit_size + (block_size_in_bits - (random_inserted_plaintext_bit_size % block_size_in_bits)), 
		block_size_in_bits)))
	{
		return_code = STATUS_CODE_COULDNT_PAD_VECTOR;
		goto cleanup;
	}

	uint8_t** plaintext_blocks = NULL;
	uint32_t number_of_blocks = 0;

	if (STATUS_FAILED(divide_uint8_t_into_blocks(&plaintext_blocks, &number_of_blocks, padded_plaintext, padded_plaintext_bit_size, block_size_in_bits)))
	{
		return_code = STATUS_CODE_COULDNT_DIVIDE_TO_BLOCKS;
		goto cleanup;
	}

	*out_ciphertext = (int64_t*)malloc(((block_size_in_bits * number_of_blocks) / BYTE_SIZE) * sizeof(int64_t));
	*out_ciphertext_bit_size = block_size_in_bits * number_of_blocks;
	if (*out_ciphertext == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	int64_t* ciphertext_block = NULL;

	for (uint32_t block_number = 0; block_number < number_of_blocks; ++block_number)
	{
		if (STATUS_FAILED(multiply_matrix_with_uint8_t_vector(&ciphertext_block, encryption_matrix, plaintext_blocks[block_number], dimentation, prime_field)))
		{
			return_code = STATUS_CODE_COULDNT_MULTIPLY_MATRIX_WITH_PLAINTEXT;
			goto cleanup;
		}

		memcpy_s(*out_ciphertext + (block_number * dimentation), block_size_in_bits, ciphertext_block, block_size_in_bits);

		free(ciphertext_block);
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if ((STATUS_FAILED(return_code)) && (out_ciphertext != NULL) && (*out_ciphertext != NULL) && (out_ciphertext_bit_size != NULL) && (*out_ciphertext_bit_size != NULL))
	{
		free(*out_ciphertext);
		*out_ciphertext = NULL;
		*out_ciphertext_bit_size = 0;
	}
	return return_code;
}

STATUS_CODE decrypt(uint8_t** out_plaintext, uint32_t* out_plaintext_bit_size, int64_t** decryption_matrix, uint32_t dimentation, uint32_t prime_field, int64_t* ciphertext_vector, uint32_t vector_bit_size)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	if ((NULL == out_plaintext) || (NULL == out_plaintext_bit_size) || (NULL == decryption_matrix) || (NULL == ciphertext_vector))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	uint32_t block_size_in_bits = (BYTE_SIZE * dimentation);
	uint32_t number_of_blocks = vector_bit_size / block_size_in_bits;

	int64_t** ciphertext_blocks = NULL;

	if (STATUS_FAILED(divide_int64_t_into_blocks(&ciphertext_blocks, &number_of_blocks, ciphertext_vector, vector_bit_size, block_size_in_bits)))
	{
		return_code = STATUS_CODE_COULDNT_DIVIDE_TO_BLOCKS;
		goto cleanup;
	}

	uint8_t* decrypted_plaintext = (uint8_t*)malloc(vector_bit_size / BYTE_SIZE);
	if (decrypted_plaintext == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	uint8_t* plaintext_block = NULL;

	for (uint32_t block_number = 0; block_number < number_of_blocks; ++block_number)
	{
		if (STATUS_FAILED(multiply_matrix_with_int64_t_vector(&plaintext_block, decryption_matrix, ciphertext_blocks[block_number], dimentation, prime_field)))
		{
			return_code = STATUS_CODE_COULDNT_MULTIPLY_MATRIX_WITH_CIPHERTEXT;
			goto cleanup;
		}


		for (uint32_t block_index = 0; block_index < (block_size_in_bits / BYTE_SIZE); ++block_index)
		{
			decrypted_plaintext[(block_number * dimentation) + block_index] = plaintext_block[block_index];
		}

		free(plaintext_block);
	}

	uint8_t* unpadded_plaintext = NULL;
	uint32_t unpadded_plaintext_bit_size = 0;

	if (STATUS_FAILED(remove_padding(&unpadded_plaintext, &unpadded_plaintext_bit_size, decrypted_plaintext, vector_bit_size)))
	{	
		return_code = STATUS_CODE_COULDNT_REMOVE_PADDING;
		goto cleanup;
	}

	uint8_t* original_plaintext = NULL;
	uint32_t original_plaintext_bit_size = 0;

	if (STATUS_FAILED(remove_random_bits_between_bytes(&original_plaintext, &original_plaintext_bit_size, unpadded_plaintext, unpadded_plaintext_bit_size)))
	{
		return_code = STATUS_CODE_COULDNT_REMOVE_RANDOM_BITS;
		goto cleanup;
	}

	*out_plaintext = original_plaintext;
	*out_plaintext_bit_size = original_plaintext_bit_size;

	return_code = STATUS_CODE_SUCCESS;

cleanup:
	if ((STATUS_FAILED(return_code)) && (out_plaintext != NULL) && (*out_plaintext != NULL) && (out_plaintext_bit_size != NULL) && (*out_plaintext_bit_size != NULL))
	{
		free(*out_plaintext);
		*out_plaintext = NULL;
		*out_plaintext_bit_size = 0;
	}
	return return_code;
}
