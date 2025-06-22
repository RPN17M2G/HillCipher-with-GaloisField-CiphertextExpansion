#include "Math/MatrixInverse.h"

STATUS_CODE inverse_square_matrix_adjugate_method(int64_t*** out_inverse_matrix, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t determinant = 0;
	bool is_invertible = false;
	int64_t** adjugate_matrix = NULL;
	int64_t inverse_determinant = 0;
	int64_t** minor_matrix = NULL;
	int64_t minor_matrix_determinant = 0;
	size_t row = 0, column = 0;
	int64_t** inverse_matrix_buffer = NULL;

	return_code = is_matrix_invertible(&is_invertible, matrix, dimension, prime_field);
	if (STATUS_FAILED(return_code))
	{
		log_error("[!] Failed to check if matrix is invertible in inverse_square_matrix_adjugate_method.");
		goto cleanup;
	}

	if (!is_invertible)
	{
		log_error("[!] Matrix is not invertible in inverse_square_matrix_adjugate_method.");
		return_code = STATUS_CODE_MATRIX_NOT_INVERTIBLE;
		goto cleanup;
	}


	return_code = matrix_determinant_over_galois_field_laplace_expansion(&determinant, matrix, dimension, prime_field);
	if (STATUS_FAILED(return_code))
	{
		log_error("[!] Failed to compute determinant in inverse_square_matrix_adjugate_method.");
		goto cleanup;
	}
	inverse_determinant = raise_power_over_galois_field(determinant, prime_field - 2, prime_field);

	// Calculate the adjugate matrix
	adjugate_matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
	if (NULL == adjugate_matrix)
	{
		log_error("[!] Memory allocation failed for adjugate_matrix in inverse_square_matrix_adjugate_method.");
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (row = 0; row < dimension; ++row)
	{
		adjugate_matrix[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if (NULL == adjugate_matrix[row])
		{
			log_error("[!] Memory allocation failed for adjugate_matrix row in inverse_square_matrix_adjugate_method.");
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	for (row = 0; row < dimension; ++row)
	{
		for (column = 0; column < dimension; ++column)
		{
			minor_matrix = NULL;
			return_code = build_minor_matrix(&minor_matrix, matrix, dimension, row, column);
			if (STATUS_FAILED(return_code))
			{
				log_error("[!] Failed to build minor matrix in inverse_square_matrix_adjugate_method.");
				goto cleanup;
			}
			minor_matrix_determinant = 0;
			return_code = matrix_determinant_over_galois_field_laplace_expansion(&minor_matrix_determinant, minor_matrix, dimension - 1, prime_field);
			if (STATUS_FAILED(return_code))
			{
				log_error("[!] Failed to compute minor matrix determinant in inverse_square_matrix_adjugate_method.");
				goto cleanup;
			}

			if (IS_ODD(row + column)) // If the sum of the row and column is odd, the cofactor is negative
			{
				minor_matrix_determinant = negate_over_galois_field(minor_matrix_determinant, prime_field);
			}

			adjugate_matrix[column][row] = align_to_galois_field(minor_matrix_determinant, prime_field);

			(void)free_int64_matrix(minor_matrix, dimension - 1);
			minor_matrix = NULL;
		}
	}

	// Calculate the inverse matrix
	inverse_matrix_buffer = (int64_t**)malloc(dimension * sizeof(int64_t*));
	if (NULL == inverse_matrix_buffer)
	{
		log_error("[!] Memory allocation failed for inverse_matrix_buffer in inverse_square_matrix_adjugate_method.");
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (row = 0; row < dimension; ++row)
	{
		inverse_matrix_buffer[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if (NULL == inverse_matrix_buffer[row])
		{
			log_error("[!] Memory allocation failed for inverse_matrix_buffer row in inverse_square_matrix_adjugate_method.");
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
	}

	for (row = 0; row < dimension; ++row)
	{
		for (column = 0; column < dimension; ++column)
		{

			inverse_matrix_buffer[row][column] = multiply_over_galois_field(adjugate_matrix[row][column], inverse_determinant, prime_field);
		}
	}

	*out_inverse_matrix = inverse_matrix_buffer;
	inverse_matrix_buffer = NULL;

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	(void)free_int64_matrix(adjugate_matrix, dimension);
	(void)free_int64_matrix(inverse_matrix_buffer, dimension);
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
	size_t allocation_index = 0, row_iteration = 0, row = 0, column = 0, free_index = 0;
	int64_t** out_inverse_matrix_buffer = NULL;

    if ((NULL == matrix) || (NULL == out_inverse_matrix))
    {
		log_error("[!] Invalid argument: matrix or out_inverse_matrix is NULL in inverse_square_matrix_gauss_jordan.");
		return_code = STATUS_CODE_INVALID_ARGUMENT;
    	goto cleanup;
    }

    augmented_matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!augmented_matrix)
    {
		log_error("[!] Memory allocation failed for augmented_matrix in inverse_square_matrix_gauss_jordan.");
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
    }

    for (allocation_index = 0; allocation_index < dimension; allocation_index++)
    {
        augmented_matrix[allocation_index] = (int64_t*)malloc(2 * dimension * sizeof(int64_t));
        if (!augmented_matrix[allocation_index])
        {
            log_error("[!] Memory allocation failed for augmented_matrix row in inverse_square_matrix_gauss_jordan.");
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
        }
    }

    // Initialize augmented matrix: [matrix | identity]
    for (row = 0; row < dimension; row++)
    {
        for (column = 0; column < dimension; column++)
        {
            augmented_matrix[row][column] = matrix[row][column];
            augmented_matrix[row][column + dimension] = (row == column) ? 1 : 0;
        }
    }

    // Perform Gauss-Jordan elimination
    for (row_iteration = 0; row_iteration < dimension; row_iteration++)
    {
        // Find pivot
        pivot_row = row_iteration;
        for (row = row_iteration + 1; row < dimension; row++)
        {
            if (augmented_matrix[row][row_iteration] != 0)
            {
                pivot_row = row;
                break;
            }
        }

        if (0 == augmented_matrix[pivot_row][row_iteration])
        {
            log_error("[!] Matrix is not invertible (zero pivot) in inverse_square_matrix_gauss_jordan.");
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

        for (column = 0; column < 2 * dimension; column++)
    {
            augmented_matrix[row_iteration][column] = multiply_over_galois_field(augmented_matrix[row_iteration][column], pivot_inverse, prime_field);
        }
        // Eliminate other rows
        for (row = 0; row < dimension; row++)
        {
            if (row != row_iteration && augmented_matrix[row][row_iteration] != 0)
            {
                factor = augmented_matrix[row][row_iteration];
                for (column = 0; column < 2 * dimension; column++)
                {
                    product = multiply_over_galois_field(factor, augmented_matrix[row_iteration][column], prime_field);
                    augmented_matrix[row][column] = add_over_galois_field(augmented_matrix[row][column], negate_over_galois_field(product, prime_field), prime_field);
                }
            }
        }
    }

    out_inverse_matrix_buffer = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!out_inverse_matrix_buffer)
    {
        log_error("[!] Memory allocation failed for out_inverse_matrix_buffer in inverse_square_matrix_gauss_jordan.");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
    	goto cleanup;
    }
    for (row = 0; row < dimension; row++)
    {
        out_inverse_matrix_buffer[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
		if (!out_inverse_matrix_buffer[row])
		{
            log_error("[!] Memory allocation failed for out_inverse_matrix_buffer row in inverse_square_matrix_gauss_jordan.");
    		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
    }

    // Copy right half of augmented matrix as inverse
    for (row = 0; row < dimension; row++)
    {
        for (column = 0; column < dimension; column++)
        {
            out_inverse_matrix_buffer[row][column] = align_to_galois_field(augmented_matrix[row][column + dimension], prime_field);
        }
    }

	*out_inverse_matrix = out_inverse_matrix_buffer;
	out_inverse_matrix_buffer = NULL;

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	(void)free_int64_matrix(augmented_matrix, dimension);
	(void)free_int64_matrix(out_inverse_matrix_buffer, dimension);
    return return_code;
}
