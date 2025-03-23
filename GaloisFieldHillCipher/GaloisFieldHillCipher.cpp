// GaloisFieldHillCipher.cpp : Defines the entry point for the application.
//

#include "GaloisFieldHillCipher.h"

int main()
{
	long double** out_matrix = NULL;

	STATUS_CODE return_code = generate_encryption_matrix(&out_matrix, 4, DEFAULT_PRIME_GALOIS_FIELD);
	
	for (uint32_t row = 0; row < 4; ++row)
	{
		for (uint32_t column = 0; column < 4; ++column)
		{
			printf("%lf ", out_matrix[row][column]);
		}
		printf("\n");
	}

	return return_code;
}
