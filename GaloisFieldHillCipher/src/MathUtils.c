#include "MathUtils.h"

STATUS_CODE matrix_determinant(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, int64_t* out_determinant)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t** minor_matrix = NULL;

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

	*out_determinant = 0;

	// Expand along the first row (row 0)
	uint32_t row = 0;
	for (uint32_t column = 0; column < dimentaion; ++column)
	{
		if (matrix[row][column] == 0)
		{
			continue;
		}

		return_code = build_minor_matrix(matrix, dimentaion, row, column, &minor_matrix);
		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

		int64_t minor_matrix_determinant = 0;
		return_code = matrix_determinant(minor_matrix, dimentaion - 1, prime_field, &minor_matrix_determinant);

		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

		int64_t matrix_element = matrix[row][column];
		if (IS_ODD(row + column)) // If (row + column) is odd, sign change
		{
			matrix_element = negate_over_galois_field(matrix_element, prime_field);
		}

		int64_t cofactor = multiply_over_galois_field(matrix_element, minor_matrix_determinant, prime_field);
		*out_determinant = add_over_galois_field(*out_determinant, cofactor, prime_field);

		(void)free_matrix(minor_matrix, dimentaion - 1);
		minor_matrix = NULL;
	}

cleanup:
	if (minor_matrix != NULL)
	{
		(void)free_matrix(minor_matrix, dimentaion - 1);
	}
	return return_code;
}

STATUS_CODE matrix_determinant_over_galois_field(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, int64_t* out_determinant)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;

	if ((matrix == NULL) || (out_determinant == NULL))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	return_code = matrix_determinant(matrix, dimentaion, prime_field, &determinant);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	*out_determinant = align_to_galois_field(determinant, prime_field);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE inverse_square_matrix(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, int64_t*** out_inverse_matrix)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;
	bool is_invertible = false;
	int64_t** adjugate_matrix = NULL;
	int64_t inverse_determinant = 0;

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


	return_code = matrix_determinant_over_galois_field(matrix, dimentaion, prime_field, &determinant);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}
	inverse_determinant = raise_power_over_galois_field(determinant, prime_field - 2, prime_field);

	// Calculate the adjugate matrix
	adjugate_matrix = (int64_t**)malloc(dimentaion * sizeof(int64_t*));
	if (adjugate_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		adjugate_matrix[row] = (int64_t*)malloc(dimentaion * sizeof(int64_t));
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
			int64_t** minor_matrix = NULL;
			return_code = build_minor_matrix(matrix, dimentaion, row, column, &minor_matrix);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}
			int64_t minor_matrix_determinant = 0;
			return_code = matrix_determinant_over_galois_field(minor_matrix, dimentaion - 1, prime_field, &minor_matrix_determinant);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}

			if (IS_ODD(row + column)) // If the sum of the row and column is odd, the cofactor is negative
			{
				minor_matrix_determinant = negate_over_galois_field(minor_matrix_determinant, prime_field);
			}

			adjugate_matrix[column][row] = align_to_galois_field(minor_matrix_determinant, prime_field);

			(void)free_matrix(minor_matrix, dimentaion - 1);
			minor_matrix = NULL;
		}
	}

	// Calculate the inverse matrix
	*out_inverse_matrix = (int64_t**)malloc(dimentaion * sizeof(int64_t*));
	if (*out_inverse_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		(*out_inverse_matrix)[row] = (int64_t*)malloc(dimentaion * sizeof(int64_t));
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

			(*out_inverse_matrix)[row][column] = multiply_over_galois_field(adjugate_matrix[row][column], inverse_determinant, prime_field);
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

STATUS_CODE multiply_matrix_with_uint8_t_vector(int64_t** out_vector, int64_t** matrix, uint8_t* vector, uint32_t dimentaion, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t product = 0;
	int64_t temp_result = 0;

	*out_vector = (int64_t*)malloc(dimentaion * sizeof(int64_t) + 1);
	if (*out_vector == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		temp_result = 0;
		for (uint32_t column = 0; column < dimentaion; ++column)
		{
			product = multiply_over_galois_field(matrix[row][column], (int64_t)vector[column], prime_field);
			temp_result = add_over_galois_field(temp_result, product, prime_field);
		}
		(*out_vector)[row] = temp_result;
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

STATUS_CODE gcd(int64_t first_element, int64_t second_element, int64_t* out_gcd)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	// Euclidean algorithm
	while (second_element != 0) {
		int64_t temp = second_element;
		second_element = first_element % second_element;
		first_element = temp;
	}
	*out_gcd = first_element;

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE is_matrix_invertible(int64_t** matrix, uint32_t dimentaion, uint32_t prime_field, bool* out_is_invertible)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;
	int64_t gcd_result = 0;
	
	return_code = matrix_determinant_over_galois_field(matrix, dimentaion, prime_field, &determinant);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	return_code = gcd((int64_t)prime_field, determinant, &gcd_result);
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
 
STATUS_CODE multiply_matrix_with_int64_t_vector(uint8_t** out_vector, int64_t** matrix, int64_t* vector, uint32_t dimentaion, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint64_t temp_result = 0;
	int64_t product = 0;
	
	if ((NULL == out_vector) || (NULL == matrix) || (NULL == vector))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}
	
	*out_vector = (uint8_t*)malloc(dimentaion + MEMORY_BUFFER_FOR_PLAINTEXT_BLOCK);
	if (*out_vector == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (size_t row = 0; row < dimentaion; ++row)
	{
		temp_result = 0;
		for (size_t column = 0; column < dimentaion; ++column)
		{
			product = multiply_over_galois_field(matrix[row][column], (int64_t)vector[column], prime_field);
			temp_result = add_over_galois_field(temp_result, product, prime_field);
		}

		if (temp_result > UINT8_MAX)
		{
			return_code = STATUS_CODE_INVALID_RESULT_WIDTH;
			goto cleanup;
		}
		(*out_vector)[row] = (uint8_t)(temp_result);
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

STATUS_CODE free_matrix(int64_t** matrix, uint32_t dimentaion)
{
	for (uint32_t row = 0; row < dimentaion; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return STATUS_CODE_SUCCESS;
}

STATUS_CODE build_minor_matrix(int64_t** matrix, uint32_t dimentaion, uint32_t row, uint32_t column, int64_t*** out_matrix)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	int64_t** minor_matrix = (int64_t**)malloc((dimentaion - 1) * sizeof(int64_t*));
	if (minor_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (uint32_t sub_row = 0; sub_row < dimentaion - 1; ++sub_row)
	{
		minor_matrix[sub_row] = (int64_t*)malloc((dimentaion - 1) * sizeof(int64_t));
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
