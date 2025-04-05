#include "GaloisFieldHillCipher.h"

int main()
{
	double** out_matrix = NULL;
	uint32_t matrix_dimentation = 4 + ((NUMBER_OF_RANDOM_BITS_TO_ADD * BYTE_SIZE) / sizeof(**out_matrix)) + 1;

	STATUS_CODE return_code = generate_encryption_matrix(&out_matrix, matrix_dimentation, DEFAULT_PRIME_GALOIS_FIELD);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	printf("Encryption matrix(dimentation: %d): \n", matrix_dimentation);
	for (uint32_t row = 0; row < matrix_dimentation; ++row)
	{
		for (uint32_t column = 0; column < matrix_dimentation; ++column)
		{
			printf("%f ", out_matrix[row][column]);
		}
		printf("\n");
	}

	uint8_t plaintext[] = { 'H', 'e', 'l', 'l', 'o', 'H', 'e', 'l', 'l', 'o' };
	uint32_t plaintext_bit_size = 10 * BYTE_SIZE;

	double* ciphertext = NULL;
	uint32_t ciphertext_bit_size = 0;
	return_code = encrypt(&ciphertext, &ciphertext_bit_size, out_matrix, matrix_dimentation, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_bit_size);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	printf("\nCiphertext(size %d, in bytes: %d): \n", ciphertext_bit_size, ciphertext_bit_size / BYTE_SIZE);
	for (uint32_t number = 0; number < ciphertext_bit_size / BYTE_SIZE; ++number)
	{
		printf("%f ", ciphertext[number]);
	}
	printf("\n\n");

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (out_matrix != NULL)
	{
		for (uint32_t row = 0; row < matrix_dimentation; ++row)
		{
			free(out_matrix[row]);
		}
		free(out_matrix);
	}

	if (ciphertext != NULL)
	{
		free(ciphertext);
	}
	return return_code;
}
