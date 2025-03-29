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

	uint8_t value[] = {250, 40, 123};
	uint8_t* out_value = NULL;
	uint32_t out_value_size = 0;
	add_random_bits_between_bytes(&out_value, &out_value_size, value, sizeof(value));

	uint32_t padded_out_size = 0;
	add_padding_to_match_size(&out_value, &padded_out_size, out_value_size, sizeof(value) * BYTE_SIZE);
	printf("%d", padded_out_size);
	printf("%d", out_value_size);

	free(out_value);
	for (uint32_t row = 0; row < matrix_dimentaion; ++row)
	{
		free(out_matrix[row]);
	}
	free(out_matrix);

	return return_code;
}
