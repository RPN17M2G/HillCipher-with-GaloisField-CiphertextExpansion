// GaloisFieldHillCipher.cpp : Defines the entry point for the application.
//

#include "GaloisFieldHillCipher.h"

int main()
{
	double** out_matrix = NULL;
	uint32_t matrix_dimentaion = 4 + ((NUMBER_OF_RANDOM_BITS_TO_ADD * BYTE_SIZE) / sizeof(**out_matrix)) + 1;

	STATUS_CODE return_code = generate_encryption_matrix(&out_matrix, matrix_dimentaion, DEFAULT_PRIME_GALOIS_FIELD);
	
	for (uint32_t row = 0; row < matrix_dimentaion; ++row)
	{
		for (uint32_t column = 0; column < matrix_dimentaion; ++column)
		{
			printf("%f ", out_matrix[row][column]);
		}
		printf("\n");
	}

	uint8_t plaintext[] = {'H', 'e', 'l', 'l', 'o'};
	uint32_t plaintext_size = 5 * sizeof(uint8_t);
	
	double* ciphertext = NULL;
	uint32_t ciphertext_size = 0;
	encrypt(&ciphertext, &ciphertext_size, out_matrix, matrix_dimentaion, DEFAULT_PRIME_GALOIS_FIELD, plaintext, plaintext_size);

	printf("Ciphertext(size %d): \n", ciphertext_size);
	for (uint32_t number = 0; number < ciphertext_size / BYTE_SIZE; ++number)
	{
		printf("%f ", ciphertext[number]);
	}
	printf("\n");
	free(ciphertext);

	for (uint32_t row = 0; row < matrix_dimentaion; ++row)
	{
		free(out_matrix[row]);
	}
	free(out_matrix);

	return return_code;
}
