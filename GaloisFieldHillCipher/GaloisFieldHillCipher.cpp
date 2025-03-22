// GaloisFieldHillCipher.cpp : Defines the entry point for the application.
//

#include "GaloisFieldHillCipher.h"

int main()
{
	uint32_t** out_matrix = NULL;
	if (out_matrix == NULL)
	{
		printf("Memory allocation failed.");
		return 1;
	}
	STATUS_CODE return_code = generate_encryption_matrix(&out_matrix, 4, PRIME_GALOIS_FIELD);
	
	for (uint32_t row = 0; row < 4; ++row)
	{
		for (uint32_t column = 0; column < 4; ++column)
		{
			printf("%d ", out_matrix[row][column]);
		}
		printf("\n");
	}

	return return_code;
}
