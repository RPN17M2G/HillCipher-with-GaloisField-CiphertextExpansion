#include "../../include/Math/MatrixUtils.h"

STATUS_CODE free_matrix(int64_t** matrix, uint32_t dimentaion)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t row = 0;

	if (NULL == matrix)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	for (row = 0; row < dimentaion; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}
