#include "MatrixUtils.h"

void print_matrix(int64_t** matrix, uint32_t dimension)
{
    for (size_t row = 0; row < dimension; ++row)
    {
        for (size_t column = 0; column < dimension; ++column)
        {
            printf("%ld ", matrix[row][column]);
        }
        printf("\n");
    }
}

STATUS_CODE free_matrix(int64_t** matrix, uint32_t dimentaion)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	if (matrix == NULL)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	for (size_t row = 0; row < dimentaion; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}
