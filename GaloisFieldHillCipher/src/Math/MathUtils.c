#include "../../include/Math/MathUtils.h"

STATUS_CODE matrix_determinant_laplace_expansion(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t** minor_matrix = NULL;
	uint32_t row = 0;
	int64_t minor_matrix_determinant = 0;
	int64_t matrix_element = 0;
	int64_t cofactor = 0;

	if (matrix == NULL || out_determinant == NULL)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	// Stopping Condition: The determinant of a 1x1 matrix is the value of the only element in the matrix.
	if (1 == dimension)
	{
		*out_determinant = matrix[0][0];
		return_code = STATUS_CODE_SUCCESS;
		goto cleanup;
	}

	*out_determinant = 0;

	// Expand along the first row (row 0)
	row = 0;
	for (size_t column = 0; column < dimension; ++column)
	{
		if (matrix[row][column] == 0)
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
		*out_determinant = add_over_galois_field(*out_determinant, cofactor, prime_field);

		(void)free_matrix(minor_matrix, dimension - 1);
		minor_matrix = NULL;
	}

cleanup:
	if (minor_matrix != NULL)
	{
		(void)free_matrix(minor_matrix, dimension - 1);
	}
	return return_code;
}

STATUS_CODE matrix_determinant_over_galois_field_laplace_expansion(int64_t* out_determinant, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;

	if ((matrix == NULL) || (out_determinant == NULL))
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

STATUS_CODE inverse_square_matrix_adjugate_method(int64_t*** out_inverse_matrix, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;
	bool is_invertible = false;
	int64_t** adjugate_matrix = NULL;
	int64_t inverse_determinant = 0;
	int64_t** minor_matrix = NULL;
	int64_t minor_matrix_determinant = 0;

	return_code = is_matrix_invertible(&is_invertible, matrix, dimension, prime_field);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	if (!is_invertible)
	{
		return_code = STATUS_CODE_MATRIX_NOT_INVERTIBLE;
		goto cleanup;
	}


	return_code = matrix_determinant_over_galois_field_laplace_expansion(&determinant, matrix, dimension, prime_field);
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}
	inverse_determinant = raise_power_over_galois_field(determinant, prime_field - 2, prime_field);

	// Calculate the adjugate matrix
	adjugate_matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
	if (adjugate_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (size_t row = 0; row < dimension; ++row)
	{
		adjugate_matrix[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if (adjugate_matrix[row] == NULL)
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	for (size_t row = 0; row < dimension; ++row)
	{
		for (size_t column = 0; column < dimension; ++column)
		{
			minor_matrix = NULL;
			return_code = build_minor_matrix(&minor_matrix, matrix, dimension, row, column);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}
			minor_matrix_determinant = 0;
			return_code = matrix_determinant_over_galois_field_laplace_expansion(&minor_matrix_determinant, minor_matrix, dimension - 1, prime_field);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}

			if (IS_ODD(row + column)) // If the sum of the row and column is odd, the cofactor is negative
			{
				minor_matrix_determinant = negate_over_galois_field(minor_matrix_determinant, prime_field);
			}

			adjugate_matrix[column][row] = align_to_galois_field(minor_matrix_determinant, prime_field);

			(void)free_matrix(minor_matrix, dimension - 1);
			minor_matrix = NULL;
		}
	}

	// Calculate the inverse matrix
	*out_inverse_matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
	if (*out_inverse_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (size_t row = 0; row < dimension; ++row)
	{
		(*out_inverse_matrix)[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if ((*out_inverse_matrix)[row] == NULL)
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	for (size_t row = 0; row < dimension; ++row)
	{
		for (size_t column = 0; column < dimension; ++column)
		{

			(*out_inverse_matrix)[row][column] = multiply_over_galois_field(adjugate_matrix[row][column], inverse_determinant, prime_field);
		}
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (adjugate_matrix != NULL)
	{
		for (size_t row = 0; row < dimension; ++row)
		{
			free(adjugate_matrix[row]);
		}
		free(adjugate_matrix);
	}
	if (STATUS_FAILED(return_code) && (*out_inverse_matrix != NULL))
	{
		for (size_t row = 0; row < dimension; ++row)
		{
			free((*out_inverse_matrix)[row]);
		}
		free(*out_inverse_matrix);
		*out_inverse_matrix = NULL;
	}
	return return_code;
}

STATUS_CODE multiply_matrix_with_uint8_t_vector(int64_t** out_vector, int64_t** matrix, uint8_t* vector, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t product = 0;
	int64_t temp_result = 0;

	*out_vector = (int64_t*)malloc(dimension * sizeof(int64_t));
	if (*out_vector == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (size_t row = 0; row < dimension; ++row)
	{
		temp_result = 0;
		for (size_t column = 0; column < dimension; ++column)
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

STATUS_CODE gcd(int64_t* out_gcd, int64_t first_element, int64_t second_element)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t temp = 0;

	if ((first_element == 0) && (second_element == 0))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	second_element = (second_element > 0) ? second_element : (second_element * -1);
	first_element = (first_element > 0) ? first_element : (first_element * -1);

	// Euclidean algorithm
	while (second_element != 0) {
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

	if (out_matrix == NULL)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	*out_matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
	if (*out_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	// Generate random numbers mod prime_field to fill the matrix
	for (size_t row = 0; row < dimension; ++row)
	{
		(*out_matrix)[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if ((*out_matrix)[row] == NULL)
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
		for (size_t column = 0; column < dimension; ++column)
		{
			secure_random_number = 0;
			return_code = generate_secure_random_number(&secure_random_number, (uint32_t)0, prime_field - 1);
			if (STATUS_FAILED(return_code))
			{
				goto cleanup;
			}
			(*out_matrix)[row][column] = (int64_t)(secure_random_number);
		}
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (STATUS_FAILED(return_code) && (out_matrix != NULL) && (*out_matrix != NULL))
	{
		for (size_t row = 0; row < dimension; ++row)
		{
			free((*out_matrix)[row]);
			(*out_matrix)[row] = NULL;
		}
		free(*out_matrix);
		*out_matrix = NULL;
	}
	return return_code;
}
 
STATUS_CODE multiply_matrix_with_int64_t_vector(uint8_t** out_vector, int64_t** matrix, int64_t* vector, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint64_t temp_result = 0;
	int64_t product = 0;
	
	if ((NULL == out_vector) || (NULL == matrix) || (NULL == vector))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}
	
	*out_vector = (uint8_t*)malloc(dimension + MEMORY_BUFFER_FOR_PLAINTEXT_BLOCK);
	if (*out_vector == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (size_t row = 0; row < dimension; ++row)
	{
		temp_result = 0;
		for (size_t column = 0; column < dimension; ++column)
		{
			product = multiply_over_galois_field(matrix[row][column], vector[column], prime_field);
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

STATUS_CODE build_minor_matrix(int64_t*** out_matrix, int64_t** matrix, uint32_t dimension, uint32_t row, uint32_t column)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint32_t minor_matrix_row = 0;
	uint32_t minor_matrix_column = 0;

	int64_t** minor_matrix = (int64_t**)malloc((dimension - 1) * sizeof(int64_t*));
	if (minor_matrix == NULL)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (size_t sub_row = 0; sub_row < dimension - 1; ++sub_row)
	{
		minor_matrix[sub_row] = (int64_t*)malloc((dimension - 1) * sizeof(int64_t));
		if (minor_matrix[sub_row] == NULL)
		{
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	minor_matrix_row = 0;
	minor_matrix_column = 0;
	for (size_t sub_row = 0; sub_row < dimension; ++sub_row)
	{
		if (sub_row == row) // Insert all rows except the current row
		{
			continue;
		}
		for (size_t sub_column = 0; sub_column < dimension; ++sub_column)
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
		(void)free_matrix(minor_matrix, dimension - 1);
	}
	else
	{
		*out_matrix = minor_matrix;
	}
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

	if (matrix == NULL || out_determinant == NULL)
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

    for (uint32_t copy_row_index = 0; copy_row_index < dimension; copy_row_index++)
    {
        matrix_copy[copy_row_index] = (int64_t*)malloc(dimension * sizeof(int64_t));
        if (!matrix_copy[copy_row_index])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        	goto cleanup;
        }
        for (uint32_t copy_column_index = 0; copy_column_index < dimension; copy_column_index++)
        {
            matrix_copy[copy_row_index][copy_column_index] = matrix[copy_row_index][copy_column_index];
        }
    }

    for (uint32_t row_iteration = 0; row_iteration < dimension; row_iteration++)
    {
        pivot_row = row_iteration;
        // Find pivot row with non-zero element
        for (uint32_t row = row_iteration + 1; row < dimension; row++)
        {
            if (matrix_copy[row][row_iteration] != 0)
            {
                pivot_row = row;
                break;
            }
        }

        if (matrix_copy[pivot_row][row_iteration] == 0)
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
        for (uint32_t column = row_iteration; column < dimension; column++)
        {
            matrix_copy[row_iteration][column] = multiply_over_galois_field(matrix_copy[row_iteration][column], inverse_pivot, prime_field);
        }
        // Eliminate other rows
        for (uint32_t row = 0; row < dimension; row++) {
            if (row != row_iteration && matrix_copy[row][row_iteration] != 0) {
                factor = matrix_copy[row][row_iteration];
                for (uint32_t column = row_iteration; column < dimension; column++) {
                    product = multiply_over_galois_field(factor, matrix_copy[row_iteration][column], prime_field);
                    matrix_copy[row][column] = add_over_galois_field(matrix_copy[row][column], negate_over_galois_field(product, prime_field), prime_field);
                }
            }
        }
    }

    *out_determinant = align_to_galois_field(determinant, prime_field);

    return_code = STATUS_CODE_SUCCESS;
cleanup:
	for (uint32_t i = 0; i < dimension; i++)
	{
		free(matrix_copy[i]);
	}
	free(matrix_copy);

	return return_code;
}

STATUS_CODE inverse_square_matrix_gauss_jordan(int64_t*** out_inverse_matrix, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t** augmented_matrix = NULL;
	int64_t* temp_row = NULL;
	uint32_t pivot_row = 0;
	int64_t pivot_inverse = 0;
	int64_t pivot_element = 0;
	int64_t factor = 0;
	int64_t product = 0;

    if (matrix == NULL || out_inverse_matrix == NULL)
    {
		return_code = STATUS_CODE_INVALID_ARGUMENT;
    	goto cleanup;
    }

    augmented_matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!augmented_matrix)
    {
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
    }

    for (uint32_t i = 0; i < dimension; i++)
    {
        augmented_matrix[i] = (int64_t*)malloc(2 * dimension * sizeof(int64_t));
        if (!augmented_matrix[i])
        {
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
        }
    }

    // Initialize augmented matrix: [matrix | identity]
    for (uint32_t row = 0; row < dimension; row++)
    {
        for (uint32_t column = 0; column < dimension; column++)
        {
            augmented_matrix[row][column] = matrix[row][column];
            augmented_matrix[row][column + dimension] = (row == column) ? 1 : 0;
        }
    }

    // Perform Gauss-Jordan elimination
    for (uint32_t row_iteration = 0; row_iteration < dimension; row_iteration++) {
        // Find pivot
        pivot_row = row_iteration;
        for (uint32_t row = row_iteration + 1; row < dimension; row++)
        {
            if (augmented_matrix[row][row_iteration] != 0)
            {
                pivot_row = row;
                break;
            }
        }

        if (augmented_matrix[pivot_row][row_iteration] == 0)
        {
            return_code = STATUS_CODE_MATRIX_NOT_INVERTIBLE;
			goto cleanup;
        }

        // Swap rows if needed
        if (pivot_row != row_iteration)
        {
            temp_row = augmented_matrix[row_iteration];
            augmented_matrix[row_iteration] = augmented_matrix[pivot_row];
            augmented_matrix[pivot_row] = temp_row;
        }

    	// Normalize pivot row
        pivot_element = augmented_matrix[row_iteration][row_iteration];
        pivot_inverse = raise_power_over_galois_field(pivot_element, prime_field - 2, prime_field);

        for (uint32_t column = 0; column < 2 * dimension; column++) {
            augmented_matrix[row_iteration][column] = multiply_over_galois_field(augmented_matrix[row_iteration][column], pivot_inverse, prime_field);
        }
        // Eliminate other rows
        for (uint32_t row = 0; row < dimension; row++)
        {
            if (row != row_iteration && augmented_matrix[row][row_iteration] != 0)
            {
                factor = augmented_matrix[row][row_iteration];
                for (uint32_t column = 0; column < 2 * dimension; column++)
                {
                    product = multiply_over_galois_field(factor, augmented_matrix[row_iteration][column], prime_field);
                    augmented_matrix[row][column] = add_over_galois_field(augmented_matrix[row][column], negate_over_galois_field(product, prime_field), prime_field);
                }
            }
        }
    }

    *out_inverse_matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!*out_inverse_matrix)
    {
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
    	goto cleanup;
    }
    for (uint32_t row = 0; row < dimension; row++)
    {
        (*out_inverse_matrix)[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if (!(*out_inverse_matrix)[row])
		{
    		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
    }

    // Copy right half of augmented matrix as inverse
    for (uint32_t row = 0; row < dimension; row++)
    {
        for (uint32_t column = 0; column < dimension; column++)
        {
            (*out_inverse_matrix)[row][column] = align_to_galois_field(augmented_matrix[row][column + dimension], prime_field);
        }
    }

	return_code = STATUS_CODE_SUCCESS;
cleanup:
    for (uint32_t i = 0; i < dimension; i++)
    {
	    free(augmented_matrix[i]);
    }
    free(augmented_matrix);

	if (STATUS_FAILED(return_code) && (out_inverse_matrix != NULL))
	{
		for (uint32_t i = 0; i < dimension; i++)
		{
			free((*out_inverse_matrix)[i]);
		}
		free((*out_inverse_matrix));
	}

    return return_code;
}
