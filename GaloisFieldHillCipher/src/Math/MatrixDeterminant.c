#include "Math/MatrixDeterminant.h"

STATUS_CODE matrix_determinant_laplace_expansion(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t** minor_matrix = NULL;
	uint32_t row = 0;
	size_t column = 0;
	int64_t minor_matrix_determinant = 0;
	int64_t matrix_element = 0;
	int64_t cofactor = 0;
	int64_t determinant_buffer = 0;

	if ((NULL == matrix) || (NULL == out_determinant))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	// Stopping Condition: The determinant of a 1x1 matrix is the value of the only element in the matrix.
	if (1 == dimension)
	{
		determinant_buffer = matrix[0][0];
		*out_determinant = determinant_buffer;
		return_code = STATUS_CODE_SUCCESS;
		goto cleanup;
	}

	determinant_buffer = 0;

	// Expand along the first row
	row = 0;
	for (column = 0; column < dimension; ++column)
	{
		if (0 == matrix[row][column])
		{
			continue;
		}

		return_code = build_minor_matrix(&minor_matrix, matrix, dimension, row, column);
		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

		minor_matrix_determinant = 0;
		return_code = matrix_determinant_laplace_expansion(&minor_matrix_determinant, minor_matrix, dimension - 1, prime_field);

		if (STATUS_FAILED(return_code))
		{
			goto cleanup;
		}

		matrix_element = matrix[row][column];
		if (IS_ODD(row + column)) // If (row + column) is odd, sign change
		{
			matrix_element = negate_over_galois_field(matrix_element, prime_field);
		}
		cofactor = multiply_over_galois_field(matrix_element, minor_matrix_determinant, prime_field);
		determinant_buffer = add_over_galois_field(determinant_buffer, cofactor, prime_field);

		(void)free_int64_matrix(minor_matrix, dimension - 1);
		minor_matrix = NULL;
	}

	*out_determinant = determinant_buffer;
	return_code = STATUS_CODE_SUCCESS;
cleanup:
	(void)free_int64_matrix(minor_matrix, dimension - 1);
	return return_code;
}

STATUS_CODE matrix_determinant_over_galois_field_laplace_expansion(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;

	if ((NULL == matrix) || (NULL == out_determinant) || (0 == dimension) || (0 == prime_field))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	return_code = matrix_determinant_laplace_expansion(&determinant, matrix, dimension, prime_field);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	*out_determinant = align_to_galois_field(determinant, prime_field);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE matrix_determinant_over_galois_field_gauss_jordan(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t** matrix_copy = NULL;
	int64_t determinant = 1;
	uint32_t pivot_row = 0;
	int64_t inverse_pivot = 0;
	int64_t* temp_row = NULL;
	int64_t factor = 0;
	int64_t product = 0;
	size_t copy_row_index = 0, copy_column_index = 0, row_iteration = 0, free_index = 0, row = 0, column = 0;

	if ((NULL == matrix) || (NULL == out_determinant) || (0 == dimension) || (0 == prime_field))
    {
		return_code = STATUS_CODE_INVALID_ARGUMENT;
    	goto cleanup;
    }

    matrix_copy = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!matrix_copy)
    {
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
    	goto cleanup;
    }

    for (copy_row_index = 0; copy_row_index < dimension; copy_row_index++)
    {
        matrix_copy[copy_row_index] = (int64_t*)malloc(dimension * sizeof(int64_t));
        if (!matrix_copy[copy_row_index])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        	goto cleanup;
        }
        for (copy_column_index = 0; copy_column_index < dimension; copy_column_index++)
        {
            matrix_copy[copy_row_index][copy_column_index] = matrix[copy_row_index][copy_column_index];
        }
    }

    for (row_iteration = 0; row_iteration < dimension; row_iteration++)
    {
        pivot_row = row_iteration;
        // Find pivot row with non-zero element
        for (row = row_iteration + 1; row < dimension; row++)
        {
            if (matrix_copy[row][row_iteration] != 0)
            {
                pivot_row = row;
                break;
            }
        }

        if (0 == matrix_copy[pivot_row][row_iteration])
        {
            determinant = 0;
            break;
        }

        // Swap rows if needed
        if (pivot_row != row_iteration) {
            temp_row = matrix_copy[row_iteration];
            matrix_copy[row_iteration] = matrix_copy[pivot_row];
            matrix_copy[pivot_row] = temp_row;
            determinant = negate_over_galois_field(determinant, prime_field);
        }

    	// Multiply the diagonal element into the determinant
        determinant = multiply_over_galois_field(determinant, matrix_copy[row_iteration][row_iteration], prime_field);

        inverse_pivot = raise_power_over_galois_field(matrix_copy[row_iteration][row_iteration], prime_field - 2, prime_field);

        // Normalize pivot row
        for (column = row_iteration; column < dimension; column++)
        {
            matrix_copy[row_iteration][column] = multiply_over_galois_field(matrix_copy[row_iteration][column], inverse_pivot, prime_field);
        }
        // Eliminate other rows
        for (row = 0; row < dimension; row++) {
            if (row != row_iteration && matrix_copy[row][row_iteration] != 0) {
                factor = matrix_copy[row][row_iteration];
                for (column = row_iteration; column < dimension; column++) {
                    product = multiply_over_galois_field(factor, matrix_copy[row_iteration][column], prime_field);
                    matrix_copy[row][column] = add_over_galois_field(matrix_copy[row][column], negate_over_galois_field(product, prime_field), prime_field);
                }
            }
        }
    }

    *out_determinant = align_to_galois_field(determinant, prime_field);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (matrix_copy)
	{
		for (free_index = 0; free_index < dimension; free_index++)
		{
			free(matrix_copy[free_index]);
		}
		free(matrix_copy);
	}

	return return_code;
}
