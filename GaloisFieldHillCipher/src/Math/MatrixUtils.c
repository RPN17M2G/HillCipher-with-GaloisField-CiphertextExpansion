#include "Math/MatrixUtils.h"

STATUS_CODE free_int64_matrix(int64_t** matrix, uint32_t dimension)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t row = 0;

	if (NULL == matrix)
	{
		log_warn("[!] Matrix is NULL in free_int64_matrix.");
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	for (row = 0; row < dimension; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE free_uint8_matrix(uint8_t** matrix, uint32_t dimension)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t row = 0;

	if (NULL == matrix)
	{
		log_error("[!] Invalid argument: matrix is NULL in free_uint8_matrix.");
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	for (row = 0; row < dimension; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

