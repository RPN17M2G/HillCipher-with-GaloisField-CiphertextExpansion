// GaloisFieldHillCipher.cpp : Defines the entry point for the application.
//

#include "GaloisFieldHillCipher.h"

int main()
{
	double** out_matrix = NULL;

	STATUS_CODE return_code = generate_encryption_matrix(&out_matrix, 4, DEFAULT_PRIME_GALOIS_FIELD);
	
	for (uint32_t row = 0; row < 4; ++row)
	{
		for (uint32_t column = 0; column < 4; ++column)
		{
			printf("%f ", out_matrix[row][column]);
		}
		printf("\n");
	}

	double value = -16777619.4556; 
	double out_value = 0;
	add_random_bits_between_bytes(&out_value, value);

	printf("Value: %f\n", value);
	printf("Out Value: %f\n", out_value);

	return return_code;
}
