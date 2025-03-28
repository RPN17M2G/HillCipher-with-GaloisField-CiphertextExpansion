// GaloisFieldHillCipher.cpp : Defines the entry point for the application.
//

#include "GaloisFieldHillCipher.h"

int main()
{
	double** out_matrix = NULL;
	uint32_t matrix_dimentaion = 4 + (NUMBER_OF_RANDOM_BYTES_TO_ADD / sizeof(**out_matrix)) + 1;

	STATUS_CODE return_code = generate_encryption_matrix(&out_matrix, matrix_dimentaion, DEFAULT_PRIME_GALOIS_FIELD);
	
	for (uint32_t row = 0; row < matrix_dimentaion; ++row)
	{
		for (uint32_t column = 0; column < matrix_dimentaion; ++column)
		{
			printf("%f ", out_matrix[row][column]);
		}
		printf("\n");
	}

	double value = -16777619.4556; 
	CIPHERTEXT_EXPANSION_VECTOR out_value;
	add_random_bits_between_bytes(&out_value, value);

	printf("Value: %f\n", value);
	printf("Out Value: %f\n", out_value);

	return return_code;
}
