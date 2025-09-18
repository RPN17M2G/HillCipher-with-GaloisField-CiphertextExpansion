#include "Math/MatrixUtils.h"

STATUS_CODE is_matrix_invertible(bool* out_is_invertible, int64_t** matrix, uint32_t dimension, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t determinant = 0;
    int64_t gcd_result = 0;

    if (!out_is_invertible || !matrix)
    {
        log_error("Invalid arguments in is_matrix_invertible");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Checking matrix invertibility: dimension=%u, prime_field=%u", dimension, prime_field);

    return_code = matrix_determinant_over_galois_field_gauss_jordan(&determinant, matrix, dimension, prime_field);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to compute determinant for invertibility check");
        goto cleanup;
    }
    log_debug("Matrix determinant: %ld", determinant);

    return_code = gcd(&gcd_result, (int64_t)prime_field, determinant);
    if (STATUS_FAILED(return_code))
    {
        log_error("Failed to compute GCD(prime_field, determinant)");
        goto cleanup;
    }
    log_debug("GCD(prime_field, determinant) = %ld", gcd_result);

    // No common factor between the determinant and the prime field means the matrix is invertible
    *out_is_invertible = (1 == gcd_result);
    log_debug("Matrix %s invertible", *out_is_invertible ? "is" : "is not");

    return_code = STATUS_CODE_SUCCESS;
cleanup:
    return return_code;
}

STATUS_CODE free_int64_matrix(int64_t** matrix, uint32_t rows)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t row = 0;

	if (NULL == matrix)
	{
		log_warn("[!] Matrix is NULL in free_int64_matrix.");
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	for (row = 0; row < rows; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE free_uint8_matrix(uint8_t** matrix, uint32_t rows)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t row = 0;

	if (NULL == matrix)
	{
		log_warn("[!] matrix is NULL in free_uint8_matrix.");
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	for (row = 0; row < rows; ++row)
	{
		free(matrix[row]);
	}
	free(matrix);

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE generate_matrix_over_field(int64_t*** out_matrix, uint32_t rows, uint32_t columns, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint32_t secure_random_number = 0;
	size_t row = 0, column = 0;
	int64_t** out_matrix_buffer = NULL;

	if (NULL == out_matrix)
	{
		log_error("[!] Invalid argument: out_matrix is NULL in generate_matrix_over_field.");
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	out_matrix_buffer = (int64_t**)malloc(rows * sizeof(int64_t*));
	if (NULL == out_matrix_buffer)
	{
		log_error("[!] Memory allocation failed for out_matrix_buffer in generate_matrix_over_field.");
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	// Generate random numbers mod prime_field to fill the matrix
	for (row = 0; row < rows; ++row)
	{
		out_matrix_buffer[row] = (int64_t*)malloc(columns * sizeof(int64_t));
		if (NULL == out_matrix_buffer[row])
		{
			log_error("[!] Memory allocation failed for out_matrix_buffer row in generate_matrix_over_field.");
			return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
			goto cleanup;
		}
		for (column = 0; column < columns; ++column)
		{
			secure_random_number = 0;
			return_code = generate_secure_random_number(&secure_random_number, (uint32_t)0, prime_field - 1);
			if (STATUS_FAILED(return_code))
			{
				log_error("[!] Failed to generate secure random number in generate_matrix_over_field.");
				goto cleanup;
			}
			out_matrix_buffer[row][column] = (int64_t)(secure_random_number);
		}
	}

	*out_matrix = out_matrix_buffer;
	out_matrix_buffer = NULL;

	return_code = STATUS_CODE_SUCCESS;
	cleanup:
		(void)free_int64_matrix(out_matrix_buffer, rows);
	return return_code;
}

STATUS_CODE generate_square_matrix_over_field(int64_t*** out_matrix, uint32_t dimension, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** matrix = NULL;
    uint32_t random_number = 0;
    size_t row = 0, column = 0;

    if (!out_matrix)
    {
        log_error("Invalid argument: out_matrix is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Generating %ux%u matrix over GF(%u)", dimension, dimension, prime_field);

    matrix = (int64_t**)malloc(dimension * sizeof(int64_t*));
    if (!matrix)
    {
        log_error("Memory allocation failed for matrix rows");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (row = 0; row < dimension; ++row)
    {
        matrix[row] = (int64_t*)malloc(dimension * sizeof(int64_t));
        if (!matrix[row])
        {
            log_error("Memory allocation failed for matrix row %zu", row);
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }

        for (column = 0; column < dimension; ++column)
        {
            return_code = generate_secure_random_number(&random_number, 0, prime_field);
            if (STATUS_FAILED(return_code))
            {
                log_error("Failed to generate random number for matrix[%zu][%zu]", row, column);
                goto cleanup;
            }
            matrix[row][column] = random_number;
        }
    }

    log_debug("Successfully generated random matrix");
    *out_matrix = matrix;
    matrix = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (matrix)
    {
        (void)free_int64_matrix(matrix, dimension);
    }
    return return_code;
}

STATUS_CODE build_minor_matrix(int64_t*** out_minor_matrix, int64_t** matrix, uint32_t dimension, uint32_t row_to_exclude, size_t column_to_exclude)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t** minor_matrix = NULL;
    size_t current_row = 0, current_column = 0;
    size_t minor_row = 0, minor_column = 0;

    if (!out_minor_matrix || !matrix)
    {
        log_error("Invalid arguments in build_minor_matrix");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Building minor matrix: dimension=%u, excluding row=%u, column=%zu",
              dimension, row_to_exclude, column_to_exclude);

    minor_matrix = (int64_t**)malloc((dimension - 1) * sizeof(int64_t*));
    if (!minor_matrix)
    {
        log_error("Memory allocation failed for minor matrix rows");
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (current_row = 0; current_row < dimension - 1; ++current_row)
    {
        minor_matrix[current_row] = (int64_t*)malloc((dimension - 1) * sizeof(int64_t));
        if (!minor_matrix[current_row])
        {
            log_error("Memory allocation failed for minor matrix row %zu", current_row);
            return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
            goto cleanup;
        }
    }

    minor_row = 0;
    for (current_row = 0; current_row < dimension; ++current_row)
    {
        if (current_row == row_to_exclude)
        {
            continue;
        }

        minor_column = 0;
        for (current_column = 0; current_column < dimension; ++current_column)
        {
            if (current_column == column_to_exclude)
            {
                continue;
            }
            minor_matrix[minor_row][minor_column] = matrix[current_row][current_column];
            minor_column++;
        }
        minor_row++;
    }

    log_debug("Successfully built minor matrix of dimension %u", dimension - 1);
    *out_minor_matrix = minor_matrix;
    minor_matrix = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    if (minor_matrix)
    {
        (void)free_int64_matrix(minor_matrix, dimension - 1);
    }
    return return_code;
}
