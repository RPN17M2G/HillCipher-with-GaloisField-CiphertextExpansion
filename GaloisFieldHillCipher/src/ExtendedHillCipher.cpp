#include "ExtendedHillCipher.h"

STATUS_CODE encrypt(double** out_ciphertext, uint32_t* out_ciphertext_size, double** encryption_matrix, uint32_t dimentation, uint32_t prime_field, uint8_t* plaintext_vector, uint32_t vector_size)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	if ((NULL == out_ciphertext) || (NULL == out_ciphertext_size) || (NULL == encryption_matrix) || (NULL == plaintext_vector))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	uint8_t* padded_plaintext = NULL;
	uint32_t padded_plaintext_size = 0;

	if (STATUS_FAILED(add_random_bits_between_bytes(&padded_plaintext, &padded_plaintext_size, plaintext_vector, vector_size)))
	{
		return_code = STATUS_CODE_COULDNT_ADD_RANDOM_BITS;
		goto cleanup;
	}

	uint8_t** plaintext_blocks = NULL;
	uint32_t number_of_blocks = 0;

	if (STATUS_FAILED(divide_into_blocks(&plaintext_blocks, &number_of_blocks, plaintext_vector, vector_size, dimentation)))
	{
		return_code = STATUS_CODE_COULDNT_DIVIDE_TO_BLOCKS;
		goto cleanup;
	}

	*out_ciphertext = (double*)malloc(dimentation * number_of_blocks * sizeof(double));
	*out_ciphertext_size = dimentation * number_of_blocks * sizeof(double) * BYTE_SIZE;
	if (*out_ciphertext == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	double* ciphertext_block = NULL;

	for (uint32_t block_number = 0; block_number < number_of_blocks; ++block_number)
	{
		if (STATUS_FAILED(matrix_multipication_with_vector(&ciphertext_block, encryption_matrix, plaintext_blocks[block_number], dimentation, prime_field)))
		{
			return_code = STATUS_CODE_COULDNT_MULTIPLY_MATRIX_WITH_PLAINTEXT;
			goto cleanup;
		}

		memcpy_s(*out_ciphertext + (block_number * dimentation), dimentation, ciphertext_block, dimentation);

		free(ciphertext_block);
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if ((STATUS_FAILED(return_code)) && (*out_ciphertext != NULL) && (*out_ciphertext_size != NULL))
	{
		free(*out_ciphertext);
		*out_ciphertext = NULL;
		*out_ciphertext_size = 0;
	}
	return return_code;
}

STATUS_CODE decrypt(uint8_t** out_plaintext, uint32_t* out_plaintext_size, double** decryption_matrix, uint32_t dimentation, uint32_t prime_field, double* ciphertext_vector, uint32_t vector_size)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

cleanup:
	return return_code;
}
