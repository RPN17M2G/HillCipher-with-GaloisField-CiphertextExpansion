#include "MathUtils.h"

STATUS_CODE matrix_determinant(double** matrix, uint32_t dimentaion, double* out_determinant)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	double** minor_matrix = NULL;

	if (matrix == NULL || out_determinant == NULL)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	// Stopping Condition: The determinant of a 1x1 matrix is the value of the only element in the matrix.
	if (1 == dimentaion)
	{
		*out_determinant = matrix[0][0];
		return_code = STATUS_CODE_SUCCESS;
		goto cleanup;
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		for (uint32_t column = 0; column < dimentaion; ++column)
		{
			if (matrix[row][column] == 0) // Irrelevant calculation because multiplication with 0 is always 0
			{
				continue;
			}
			else
			{
				return_code = build_minor_matrix(matrix, dimentaion, row, column, &minor_matrix);
				if (STATUS_FAILED(return_code))
				{
					goto cleanup;
				}

				double minor_matrix_determinant = 0;
				return_code = matrix_determinant(minor_matrix, dimentaion - 1, &minor_matrix_determinant);

				if (STATUS_FAILED(return_code))
				{
					goto cleanup;
				}

				// Calculate the cofactor of the element
				double cofactor = matrix[row][column] * minor_matrix_determinant;
				if (IS_ODD(row + column)) // If the sum of the row and column is odd, the cofactor is negative
				{
					cofactor *= -1;
				}
				*out_determinant += cofactor;

				(void)free_matrix(minor_matrix, dimentaion - 1);
				minor_matrix = NULL;
			}
		}
	}

cleanup:
	if (minor_matrix != NULL)
	{
		(void)free_matrix(minor_matrix, dimentaion - 1);
	}
	return return_code;
}

STATUS_CODE square_matrix_inverse(double** matrix, uint32_t dimentaion, uint32_t prime_field, double*** out_inverse_matrix)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	double determinant = 0;
	bool is_invertible = false;
	double** adjugate_matrix = NULL;

	return_code = is_matrix_invertible(matrix, dimentaion, prime_field, &is_invertible);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	if (!is_invertible)
	{
		return_code = STATUS_CODE_MATRIX_NOT_INVERTIBLE;
		goto cleanup;
	}

	return_code = matrix_determinant(matrix, dimentaion, &determinant);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	// Calculate the adjugate matrix
	adjugate_matrix = (double**)malloc(dimentaion * sizeof(double*));
	if (adjugate_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		adjugate_matrix[row] = (double*)malloc(dimentaion * sizeof(double));
		if (adjugate_matrix[row] == NULL)
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		for (uint32_t column = 0; column < dimentaion; ++column)
		{
			double** minor_matrix = NULL;
			return_code = build_minor_matrix(matrix, dimentaion, row, column, &minor_matrix);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}
			double minor_matrix_determinant = 0;
			return_code = matrix_determinant(minor_matrix, dimentaion - 1, &minor_matrix_determinant);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}

			if (IS_ODD(row + column)) // If the sum of the row and column is odd, the cofactor is negative
			{
				minor_matrix_determinant *= -1;
			}

			adjugate_matrix[column][row] = minor_matrix_determinant;

			(void)free_matrix(minor_matrix, dimentaion - 1);
			minor_matrix = NULL;
		}
	}

	// Calculate the inverse matrix
	*out_inverse_matrix = (double**)malloc(dimentaion * sizeof(double*));
	if (*out_inverse_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		(*out_inverse_matrix)[row] = (double*)malloc(dimentaion * sizeof(double));
		if ((*out_inverse_matrix)[row] == NULL)
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		for (uint32_t column = 0; column < dimentaion; ++column)
		{
			(*out_inverse_matrix)[row][column] = fmod(adjugate_matrix[row][column] / determinant, prime_field);
		}
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (adjugate_matrix != NULL)
	{
		for (uint32_t row = 0; row < dimentaion; ++row)
		{
			free(adjugate_matrix[row]);
		}
		free(adjugate_matrix);
	}
	if (STATUS_FAILED(return_code) && (*out_inverse_matrix != NULL))
	{
		for (uint32_t row = 0; row < dimentaion; ++row)
		{
			free((*out_inverse_matrix)[row]);
		}
		free(*out_inverse_matrix);
		*out_inverse_matrix = NULL;
	}
	return return_code;
}

STATUS_CODE matrix_multipication_with_vector(double** out_vector, double** matrix, uint8_t* vector, uint32_t dimentaion, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	*out_vector = (double*)malloc(dimentaion * sizeof(double));
	if (*out_vector == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		(*out_vector)[row] = 0;
		for (uint32_t column = 0; column < dimentaion; ++column)
		{
			(*out_vector)[row] += fmod(matrix[row][column] * vector[column], prime_field);
		}
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (STATUS_FAILED(return_code))
	{
		free(*out_vector);
		*out_vector = NULL;
	}
	return return_code;
}

STATUS_CODE gcd(double first_element, double second_element, double* out_gcd)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	// Euclidean algorithm
	while (second_element != 0) {
		double temp = second_element;
		second_element = fmod(first_element, second_element);
		first_element = temp;
	}
	*out_gcd = first_element;

	return return_code;
}

STATUS_CODE is_matrix_invertible(double** matrix, uint32_t dimentaion, uint32_t prime_field, bool* out_is_invertible)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	double determinant = 0;
	double gcd_result = 0;
	
	return_code = matrix_determinant(matrix, dimentaion, &determinant);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	return_code = gcd((double)prime_field, determinant, &gcd_result);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	// No common factor between the determinant and the prime field means the matrix is invertible
	*out_is_invertible = (1 == gcd_result);
	
	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE free_matrix(double** matrix, uint32_t dimentaion)
{
	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return STATUS_CODE_SUCCESS;
}

STATUS_CODE build_minor_matrix(double** matrix, uint32_t dimentaion, uint32_t row, uint32_t column, double*** out_matrix)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	double** minor_matrix = (double**)malloc((dimentaion - 1) * sizeof(double*));
	if (minor_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t sub_row = 0; sub_row < dimentaion - 1; ++sub_row)
	{
		minor_matrix[sub_row] = (double*)malloc((dimentaion - 1) * sizeof(double));
		if (minor_matrix[sub_row] == NULL)
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	uint32_t minor_matrix_row = 0;
	uint32_t minor_matrix_column = 0;
	for (uint32_t sub_row = 0; sub_row < dimentaion; ++sub_row)
	{
		if (sub_row == row) // Insert all rows except the current row
		{
			continue;
		}
		for (uint32_t sub_column = 0; sub_column < dimentaion; ++sub_column)
		{
			if (sub_column == column) // Insert all columns except the current column
			{
				continue;
			}
			minor_matrix[minor_matrix_row][minor_matrix_column] = matrix[sub_row][sub_column];
			++minor_matrix_column;
		}
		++minor_matrix_row;
		minor_matrix_column = 0;
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (STATUS_FAILED(return_code))
	{
		(void)free_matrix(minor_matrix, dimentaion - 1);
	}
	else
	{
		*out_matrix = minor_matrix;
	}
	return return_code;
}
