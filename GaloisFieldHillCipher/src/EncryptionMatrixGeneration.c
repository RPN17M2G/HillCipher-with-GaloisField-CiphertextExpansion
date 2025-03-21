#include "EncryptionMatrixGeneration.h"

STATUS_CODE generate_encryption_matrix(uint32_t*** out_matrix, uint32_t dimentation, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE::STATUS_CODE_UNINITIALIZED;

    if (out_matrix == NULL)
    {
        return_code = STATUS_CODE::STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    *out_matrix = (decltype(*out_matrix))malloc(dimentation * sizeof(decltype(*out_matrix));
    if (*out_matrix == NULL)
    {
        return_code = STATUS_CODE::STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    // Generate random numbers mod prime_field to fill the matrix
    for (uint32_t row = 0; row < dimentation; ++row)
    {
		(*out_matrix)[row] = (decltype((*out_matrix)[row]))malloc(dimentation * sizeof(decltype((*out_matrix)[row]));
		if ((*out_matrix)[row] == NULL)
		{
			return_code = STATUS_CODE::STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
		for (uint32_t column = 0; column < dimentation; ++column)
		{
			// Generating a cryptography secure random number using sodium.
			uint32_t secure_random_value = static_cast<uint32_t>(randombytes_uniform(prime_field)); // randombytes_uniform returns a number between 0 and prime_field - 1
			(*out_matrix)[row][column] = secure_random_value;
		}
    }

	return_code = STATUS_CODE::STATUS_CODE_SUCCESS;
cleanup:
	if (STATUS_FIALURE(return_code))
	{
		for (uint32_t row = 0; row < dimentation; ++row)
		{
			free((*out_matrix)[row]);
			(*out_matrix)[row] = NULL;
		}
		free(*out_matrix);
		*out_matrix = NULL;
	}
    return return_code;
}
