#include "GaloisFieldHillCipher.h"

int main()
{
	int64_t** encryption_matrix = NULL;
	int64_t** decryption_matrix = NULL;
	uint32_t matrix_dimentation = 2;


	uint8_t plaintext[] = { 'H', 'e', 'l', 'l', 'o', 'H', 'e', 'l', 'l', 'o' };
	uint32_t plaintext_bit_size = 10 * BYTE_SIZE;

	int64_t* ciphertext = NULL;
	uint32_t ciphertext_bit_size = 0;

	STATUS_CODE return_code = generate_encryption_matrix(&encryption_matrix, matrix_dimentation, DEFAULT_PRIME_GALOIS_FIELD);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	printf("Encryption matrix(dimentation: %d): \n", matrix_dimentation);
	for (uint32_t row = 0; row < matrix_dimentation; ++row)
	{
		for (uint32_t column = 0; column < matrix_dimentation; ++column)
		{
			printf("%d ", encryption_matrix[row][column]);
		}
		printf("\n");
	}

	return_code = encrypt(&ciphertext, &ciphertext_bit_size, encryption_matrix, matrix_dimentation, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_bit_size);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	printf("\nCiphertext(size %d, in bytes: %d): \n", ciphertext_bit_size, ciphertext_bit_size / BYTE_SIZE);
	for (uint32_t number = 0; number < ciphertext_bit_size / BYTE_SIZE; ++number)
	{
		printf("%d ", ciphertext[number]);
	}
	printf("\n\n");

	return_code = generate_decryption_matrix(&decryption_matrix, matrix_dimentation, encryption_matrix, DEFAULT_PRIME_GALOIS_FIELD);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	printf("Decryption matrix(dimentation: %d): \n", matrix_dimentation);
	for (uint32_t row = 0; row < matrix_dimentation; ++row)
	{
		for (uint32_t column = 0; column < matrix_dimentation; ++column)
		{
			printf("%d ", decryption_matrix[row][column]);
		}
		printf("\n");
	}

	uint8_t* decrypted_text = NULL;
	uint32_t decrypted_size = 0;
	return_code = decrypt(&decrypted_text, &decrypted_size, decryption_matrix, matrix_dimentation, DEFAULT_PRIME_GALOIS_FIELD, ciphertext, ciphertext_bit_size);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	printf("\nDecrypted Plaintext(size %d, in bytes: %d): \n", decrypted_size, decrypted_size / BYTE_SIZE);
	for (uint32_t number = 0; number < decrypted_size / BYTE_SIZE; ++number)
	{
		printf("%c ", decrypted_text[number]);
	}
	printf("\n\n");

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (encryption_matrix != NULL)
	{
		for (uint32_t row = 0; row < matrix_dimentation; ++row)
		{
			free(encryption_matrix[row]);
		}
		free(encryption_matrix);
	}

	if (decryption_matrix != NULL)
	{
		for (uint32_t row = 0; row < matrix_dimentation; ++row)
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
