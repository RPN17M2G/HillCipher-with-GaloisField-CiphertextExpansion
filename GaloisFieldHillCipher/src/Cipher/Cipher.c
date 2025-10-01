#include "Cipher/Cipher.h"

STATUS_CODE encrypt(int64_t** out_ciphertext, uint32_t* out_ciphertext_bit_size, uint8_t* plaintext_vector, uint32_t vector_bit_size, Secrets secrets)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t block_number = 0, copy_index = 0;
	uint32_t block_size_in_bits = (BYTE_SIZE * secrets.dimension);
	uint8_t* random_inserted_plaintext = NULL;
	uint32_t random_inserted_plaintext_bit_size = 0;
	uint8_t* padded_plaintext = NULL;
	uint32_t padded_plaintext_bit_size = 0;
	uint8_t** plaintext_blocks = NULL;
	uint32_t number_of_blocks = 0;
	int64_t* ciphertext_block = NULL;
	int64_t* ciphertext_buffer = NULL;
	int64_t* original_hill_cipher_block = NULL;

	if ((secrets.dimension > (UINT32_MAX / BYTE_SIZE)) || (NULL == out_ciphertext) ||
        (NULL == out_ciphertext_bit_size) || (NULL == plaintext_vector) ||
        (NULL == secrets.key_matrix) || (NULL == secrets.error_vectors))
    {
        log_error("[!] Invalid arguments in encrypt: %s",
                 secrets.dimension > (UINT32_MAX / BYTE_SIZE) ? "dimension overflow" :
                 !out_ciphertext ? "out_ciphertext is NULL" :
                 !out_ciphertext_bit_size ? "out_ciphertext_bit_size is NULL" :
                 !plaintext_vector ? "plaintext_vector is NULL" :
                 !secrets.key_matrix ? "key_matrix is NULL" :
                 "error_vectors is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_info("Starting encryption: dimension=%u, input_size=%u bits", secrets.dimension, vector_bit_size);

    return_code = add_random_bits_between_bytes(&random_inserted_plaintext,
        &random_inserted_plaintext_bit_size, plaintext_vector, vector_bit_size,
        secrets.number_of_random_bits_to_add);
    if (STATUS_FAILED(return_code))
	{
		log_error("[!] Failed to add random bits between bytes");
		goto cleanup;
	}
	log_debug("Added random bits: original_size=%u bits, new_size=%u bits",
             vector_bit_size, random_inserted_plaintext_bit_size);

	return_code = pad_to_length(&padded_plaintext,
		&padded_plaintext_bit_size,
		random_inserted_plaintext,
		random_inserted_plaintext_bit_size,
		random_inserted_plaintext_bit_size + (block_size_in_bits - (random_inserted_plaintext_bit_size % block_size_in_bits)),
		block_size_in_bits);
	if (STATUS_FAILED(return_code))
	{
		log_error("[!] Failed to pad plaintext to block size");
		goto cleanup;
	}
	log_debug("Padded plaintext to length %u bits", padded_plaintext_bit_size);

	return_code = divide_uint8_t_into_blocks(&plaintext_blocks, &number_of_blocks, padded_plaintext,
		padded_plaintext_bit_size, block_size_in_bits);
	if (STATUS_FAILED(return_code))
	{
		log_error("[!] Failed to divide plaintext into blocks");
		goto cleanup;
	}
	log_debug("Divided plaintext into %u blocks of %u bits each", number_of_blocks, block_size_in_bits);

	ciphertext_buffer = (int64_t*)malloc(((block_size_in_bits * number_of_blocks) / BYTE_SIZE) * sizeof(int64_t));
	if (NULL == ciphertext_buffer)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (block_number = 0; block_number < number_of_blocks; ++block_number)
	{
		return_code = multiply_matrix_with_uint8_t_vector(&original_hill_cipher_block, secrets.key_matrix, plaintext_blocks[block_number], secrets.dimension, secrets.prime_field);
		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

		return_code = add_affine_transformation(&ciphertext_block, secrets.error_vectors, secrets.number_of_error_vectors, original_hill_cipher_block, secrets.dimension, secrets.prime_field);
		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

		for (copy_index = 0; copy_index < block_size_in_bits / BYTE_SIZE; ++copy_index)
		{
			(ciphertext_buffer + (block_number * secrets.dimension))[copy_index] = ciphertext_block[copy_index];
		}

		free(original_hill_cipher_block);
		free(ciphertext_block);
	}

	*out_ciphertext = ciphertext_buffer;
	ciphertext_buffer = NULL;
	*out_ciphertext_bit_size = block_size_in_bits * number_of_blocks * sizeof(int64_t);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	free(ciphertext_buffer);
	free(random_inserted_plaintext);
	free(padded_plaintext);
	(void)free_uint8_matrix(plaintext_blocks, number_of_blocks);

	return return_code;
}

STATUS_CODE decrypt(uint8_t** out_plaintext, uint32_t* out_plaintext_bit_size, int64_t* ciphertext_vector, uint32_t vector_bit_size, Secrets secrets)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t block_number = 0, block_index = 0;
	uint32_t vector_bit_size_aligned_to_uint8_t = vector_bit_size / sizeof(int64_t);
	uint32_t block_size_in_bits_aligned_to_uint8_t = (BYTE_SIZE * secrets.dimension);
	uint32_t block_size_in_bits_aligned_to_int64_t = block_size_in_bits_aligned_to_uint8_t * sizeof(int64_t);
	uint32_t number_of_blocks = vector_bit_size / block_size_in_bits_aligned_to_int64_t;
	int64_t** ciphertext_blocks = NULL;
	uint8_t* decrypted_plaintext_blocks = NULL;
	uint8_t* plaintext_block = NULL;
	uint8_t* unpadded_plaintext = NULL;
	uint32_t unpadded_plaintext_bit_size = 0;
	uint8_t* original_plaintext = NULL;
	uint32_t original_plaintext_bit_size = 0;
	int64_t* affine_subtracted_block = NULL;

	if ((NULL == out_plaintext) || (NULL == out_plaintext_bit_size) || (NULL == ciphertext_vector) ||
        (NULL == secrets.key_matrix) || (NULL == secrets.error_vectors))
    {
        log_error("[!] Invalid arguments in decrypt function");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Starting decryption process with dimension %u, random bits %u", secrets.dimension, secrets.number_of_random_bits_to_add);

    return_code = divide_int64_t_into_blocks(&ciphertext_blocks, &number_of_blocks, ciphertext_vector,
        vector_bit_size, block_size_in_bits_aligned_to_int64_t);
    if (STATUS_FAILED(return_code))
	{
		log_error("[!] Failed to divide ciphertext into blocks");
		goto cleanup;
	}
	log_debug("Divided ciphertext into %u blocks of %u bits each", number_of_blocks, block_size_in_bits_aligned_to_int64_t);

	decrypted_plaintext_blocks = (uint8_t*)malloc(vector_bit_size_aligned_to_uint8_t / BYTE_SIZE);
	if (NULL == decrypted_plaintext_blocks)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (block_number = 0; block_number < number_of_blocks; ++block_number)
	{
		return_code = substruct_affine_transformation(&affine_subtracted_block, secrets.error_vectors, secrets.number_of_error_vectors, ciphertext_blocks[block_number], secrets.dimension, secrets.prime_field);
		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

		return_code = multiply_matrix_with_int64_t_vector(&plaintext_block, secrets.key_matrix, affine_subtracted_block, secrets.dimension, secrets.prime_field);
		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}
		for (block_index = 0; block_index < (block_size_in_bits_aligned_to_uint8_t / BYTE_SIZE); ++block_index)
		{
			decrypted_plaintext_blocks[(block_number * secrets.dimension) + block_index] = plaintext_block[block_index];
		}
		free(plaintext_block);
		plaintext_block = NULL;
		free(affine_subtracted_block);
		affine_subtracted_block = NULL;
	}

	return_code = remove_padding(&unpadded_plaintext, &unpadded_plaintext_bit_size,
        decrypted_plaintext_blocks, vector_bit_size_aligned_to_uint8_t);
	if (STATUS_FAILED(return_code))
	{
		log_error("[!] Failed to remove padding from decrypted plaintext");
		goto cleanup;
	}
	log_debug("Removed padding: size after removal %u bits", unpadded_plaintext_bit_size);

	return_code = remove_random_bits_between_bytes(&original_plaintext, &original_plaintext_bit_size,
        unpadded_plaintext, unpadded_plaintext_bit_size, secrets.number_of_random_bits_to_add);
    if (STATUS_FAILED(return_code))
    {
        log_error("[!] Failed to remove random bits between bytes");
        goto cleanup;
    }
    log_debug("Removed random bits: final plaintext size %u bits", original_plaintext_bit_size);

	*out_plaintext = original_plaintext;
	original_plaintext = NULL;
	*out_plaintext_bit_size = original_plaintext_bit_size;

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	free(original_plaintext);
	free(decrypted_plaintext_blocks);
	free(unpadded_plaintext);
	free(affine_subtracted_block);
	free(plaintext_block);
	(void)free_int64_matrix(ciphertext_blocks, number_of_blocks);
	return return_code;
}
