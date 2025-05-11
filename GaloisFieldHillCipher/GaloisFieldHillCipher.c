#include "GaloisFieldHillCipher.h"

int main(int argc, char** argv)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint32_t matrix_dimension = 5;
	uint32_t max_attempts = 3;

	uint8_t* decrypted_text = NULL;
	uint32_t decrypted_size = 0;
	int64_t** encryption_matrix = NULL;
	int64_t** decryption_matrix = NULL;
	int64_t* ciphertext = NULL;
	uint32_t ciphertext_bit_size = 0;

	uint32_t plaintext_size = 200;
	uint32_t plaintext_bit_size = 0;

	ParsedArguments parsed_args = { 0 };
	return_code = parse_arguments(&parsed_args, argc, argv);
	if (STATUS_FAILED(return_code))
	{
		printf("[!] Failed to parse arguments. See status code for details.\n");
		goto cleanup;
	}

	uint8_t* plaintext = (uint8_t*)malloc(plaintext_size * BYTE_SIZE);
	if (plaintext == NULL)
	{
		printf("[!] Couldn't allocate memory for plaintext.\n");
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	// Generate plaintext
	for (size_t i = 0; i < plaintext_size; ++i)
	{
		plaintext[i] = (uint8_t)(i % 25 + 'A');
	}
	plaintext_bit_size = plaintext_size * BYTE_SIZE;

	printf("[*] Plaintext(size %d, in bytes: %d): \n", plaintext_bit_size, plaintext_bit_size / BYTE_SIZE);
	for (size_t plaintext_index = 0; plaintext_index < plaintext_bit_size / BYTE_SIZE; ++plaintext_index)
	{
		printf("%c ", plaintext[plaintext_index]);
	}
	printf("\n");

	printf("\n[*] Searching for an invertible matrix over %d galois field in the dimension of %d. This may take a few minutes...\n", DEFAULT_PRIME_GALOIS_FIELD, matrix_dimension);
	return_code = generate_encryption_matrix(&encryption_matrix, matrix_dimension, DEFAULT_PRIME_GALOIS_FIELD, max_attempts);
	if (STATUS_FAILED(return_code))
	{
		printf("[!] Couldn't generate encryption matrix. Maybe not found an invertible matrix in the requested dimension on %d attempts...\n", max_attempts);
		goto cleanup;
	}

	printf("[*] Encryption matrix(dimension: %d): \n", matrix_dimension);
	for (size_t row = 0; row < matrix_dimension; ++row)
	{
		for (size_t column = 0; column < matrix_dimension; ++column)
		{
			printf("%d ", encryption_matrix[row][column]);
		}
		printf("\n");
	}

	return_code = encrypt(&ciphertext, &ciphertext_bit_size, encryption_matrix, matrix_dimension, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_bit_size);
	if (STATUS_FAILED(return_code))
	{
		printf("[!] Couldn't encrypt the plaintext. See status code for details.\n");
		goto cleanup;
	}

	printf("\n[*] Ciphertext(size %d, in bytes: %d): \n", ciphertext_bit_size, ciphertext_bit_size / BYTE_SIZE);
	for (size_t number = 0; number < ciphertext_bit_size / BYTE_SIZE; ++number)
	{
		printf("%d ", ciphertext[number]);
	}
	printf("\n\n");

	return_code = generate_decryption_matrix(&decryption_matrix, matrix_dimension, encryption_matrix, DEFAULT_PRIME_GALOIS_FIELD);
	if (STATUS_FAILED(return_code))
	{
		printf("[!] Couldn't generate decryption matrix. Maybe not found an invertible matrix in the requested dimension...\n");
		goto cleanup;
	}

	printf("[*] Decryption matrix(dimension: %d): \n", matrix_dimension);
	for (size_t row = 0; row < matrix_dimension; ++row)
	{
		for (size_t column = 0; column < matrix_dimension; ++column)
		{
			printf("%d ", decryption_matrix[row][column]);
		}
		printf("\n");
	}

	return_code = decrypt(&decrypted_text, &decrypted_size, decryption_matrix, matrix_dimension, DEFAULT_PRIME_GALOIS_FIELD, ciphertext, ciphertext_bit_size);
	if (STATUS_FAILED(return_code))
	{
		printf("[!] Couldn't decrypt the ciphertext. See status code for details.\n");
		goto cleanup;
	}

	printf("\n[*] Decrypted Plaintext(size %d, in bytes: %d): \n", decrypted_size, decrypted_size / BYTE_SIZE);
	for (size_t index = 0; index < decrypted_size / BYTE_SIZE; ++index)
	{
		printf("%c ", decrypted_text[index]);
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (encryption_matrix != NULL)
	{
		for (size_t row = 0; row < matrix_dimension; ++row)
		{
			free(encryption_matrix[row]);
		}
		free(encryption_matrix);
	}

	if (decryption_matrix != NULL)
	{
		for (size_t row = 0; row < matrix_dimension; ++row)
		{
			free(decryption_matrix[row]);
		}
		free(decryption_matrix);
	}

	if (ciphertext != NULL)
	{
		free(ciphertext);
	}
	return return_code;
}
