#include "Math/MathUtils.h"

STATUS_CODE gcd(int64_t* out_gcd, int64_t first_element, int64_t second_element)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t temp = 0;

	if ((0 == first_element) && (0 == second_element))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	second_element = (second_element > 0) ? second_element : (second_element * -1);
	first_element = (first_element > 0) ? first_element : (first_element * -1);

	// Euclidean algorithm
	while (second_element != 0)
	{
		temp = second_element;
		second_element = first_element % second_element;
		first_element = temp;
	}

	*out_gcd = first_element;

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE is_matrix_invertible(bool* out_is_invertible, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;
	int64_t gcd_result = 0;
	
	return_code = matrix_determinant_over_galois_field_gauss_jordan(&determinant, matrix, dimension, prime_field);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	return_code = gcd(&gcd_result, (int64_t)prime_field, determinant);
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

STATUS_CODE generate_square_matrix_over_field(int64_t*** out_matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint32_t secure_random_number = 0;
	size_t row = 0, column = 0;
	int64_t** out_matrix_buffer = NULL;

	if (NULL == out_matrix)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	out_matrix_buffer = (int64_t**)malloc(dimension * sizeof(int64_t*));
	if (NULL == out_matrix_buffer)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	// Generate random numbers mod prime_field to fill the matrix
	for (row = 0; row < dimension; ++row)
	{
		out_matrix_buffer[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if (NULL == out_matrix_buffer[row])
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
		for (column = 0; column < dimension; ++column)
		{
			secure_random_number = 0;
			return_code = generate_secure_random_number(&secure_random_number, (uint32_t)0, prime_field - 1);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}
			out_matrix_buffer[row][column] = (int64_t)(secure_random_number);
		}
	}

	*out_matrix = out_matrix_buffer;
	out_matrix_buffer = NULL;

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	(void)free_int64_matrix(out_matrix_buffer, dimension);
	return return_code;
}

STATUS_CODE build_minor_matrix(int64_t*** out_matrix, int64_t** matrix, uint32_t dimension, uint32_t row, uint32_t column)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint32_t minor_matrix_row = 0;
	uint32_t sub_row = 0, sub_column = 0;
	uint32_t minor_matrix_column = 0;

	int64_t** minor_matrix = (int64_t**)malloc((dimension - 1) * sizeof(int64_t*));
	if (NULL == minor_matrix)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (sub_row = 0; sub_row < dimension - 1; ++sub_row)
	{
		minor_matrix[sub_row] = (int64_t*)malloc((dimension - 1) * sizeof(int64_t));
		if (NULL == minor_matrix[sub_row])
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	minor_matrix_row = 0;
	minor_matrix_column = 0;
	for (sub_row = 0; sub_row < dimension; ++sub_row)
	{
		if (sub_row == row) // Insert all rows except the current row
		{
			continue;
		}
		for (sub_column = 0; sub_column < dimension; ++sub_column)
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

	*out_matrix = minor_matrix;
	minor_matrix = NULL;

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	(void)free_int64_matrix(minor_matrix, dimension - 1);
	return return_code;
}
