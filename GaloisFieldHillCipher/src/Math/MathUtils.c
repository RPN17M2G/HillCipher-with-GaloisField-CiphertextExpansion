#include "Math/MathUtils.h"

STATUS_CODE gcd(int64_t* out_gcd, int64_t first_element, int64_t second_element)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	int64_t temp = 0;

	if ((0 == first_element) && (0 == second_element))
	{
		log_error("[!] Invalid argument: both elements are zero in gcd.");
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

STATUS_CODE add_two_vectors_over_gf(int64_t** out_vector, int64_t* first_vector, int64_t* second_vector, uint32_t length, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t index = 0;
	int64_t* vector_buffer = NULL;

	vector_buffer = (int64_t*) malloc(length * sizeof(int64_t));
	if (NULL == vector_buffer)
	{
		log_error("[!] Memory allocation failed for vector_buffer in add_two_vectors_over_gf.");
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (index = 0; index < length; ++index)
	{
		vector_buffer[index] = add_over_galois_field(first_vector[index], second_vector[index], prime_field);
	}

	*out_vector = vector_buffer;
	vector_buffer = NULL;
	return_code = STATUS_CODE_SUCCESS;

cleanup:
	free(vector_buffer);
	return return_code;
}

STATUS_CODE substruct_two_vectors_over_gf(int64_t** out_vector, int64_t* first_vector, int64_t* second_vector, uint32_t length, uint32_t prime_field)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	size_t index = 0;
	int64_t* vector_buffer = NULL;

	vector_buffer = (int64_t*) malloc(length * sizeof(int64_t));
	if (NULL == vector_buffer)
	{
		log_error("[!] Memory allocation failed for vector_buffer in substruct_two_vectors_over_gf.");
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	for (index = 0; index < length; ++index)
	{
		vector_buffer[index] = add_over_galois_field(first_vector[index], negate_over_galois_field(second_vector[index], prime_field), prime_field);
	}

	*out_vector = vector_buffer;
	vector_buffer = NULL;
	return_code = STATUS_CODE_SUCCESS;

	cleanup:
		free(vector_buffer);
	return return_code;
}
