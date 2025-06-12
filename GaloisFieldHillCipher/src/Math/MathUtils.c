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

	

}
