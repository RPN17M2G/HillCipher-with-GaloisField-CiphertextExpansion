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

	uint8_t value[] = {250, 40, 123, 1, 234, 55, 98, 23};
	uint32_t value_size = 8;

	printf("Original vector(size: %d): \n", value_size * BYTE_SIZE);
	for (uint32_t i = 0; i < value_size; ++i)
	{
		printf("%u ", value[i]);
	}
	printf("\n");

	uint8_t* out_value = NULL;
	uint32_t out_value_size = 0;
	add_random_bits_between_bytes(&out_value, &out_value_size, value, value_size * BYTE_SIZE);

	printf("Bit inserted vector(size: %d): \n", out_value_size);
	for (uint32_t i = 0; i < out_value_size / BYTE_SIZE; ++i)
	{
		printf("%u ", out_value[i]);
	}
	printf("\n");

	uint8_t* removed = NULL;
	uint32_t removed_size = 0;
	remove_random_bits_between_bytes(&removed, &removed_size, out_value, out_value_size);

	printf("Bit removed vector(size: %d): \n", removed_size);
	for (uint32_t i = 0; i < removed_size / BYTE_SIZE; ++i)
	{
		printf("%u ", removed[i]);
	}
	printf("\n");

	double* ciphertext = NULL;
	uint32_t ciphertext_size = 0;
	encrypt(&ciphertext, &ciphertext_size, out_matrix, matrix_dimentaion, DEFAULT_PRIME_GALOIS_FIELD, out_value, out_value_size);

	printf("Ciphertext(size %d): \n", ciphertext_size);
	for (uint32_t number = 0; number < ciphertext_size / BYTE_SIZE; ++number)
	{
		printf("%f ", ciphertext[number]);
	}
	printf("\n");

	free(out_value);
	free(removed);
	for (uint32_t row = 0; row < matrix_dimentaion; ++row)
	{
		free(out_matrix[row]);
	}
	free(out_matrix);

	return return_code;
}
