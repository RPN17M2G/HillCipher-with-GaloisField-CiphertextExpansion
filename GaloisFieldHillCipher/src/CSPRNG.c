#include "CSPRNG.h"

STATUS_CODE initialize_sodium_library()
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	if (sodium_init() < 0)
	{
		return_code = STATUS_CODE_ERROR_SODIUM_INITIALIZATION;
		goto cleanup;
	}
	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}

STATUS_CODE generate_secure_random_number(uint32_t* out_number, uint32_t minimum_value, uint32_t maximum_value)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	return_code = initialize_sodium_library();
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}
	
	if ((NULL == out_number) || (maximum_value <= minimum_value))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	*out_number = randombytes_uniform(maximum_value - minimum_value) + minimum_value;
	return_code = STATUS_CODE_SUCCESS;

cleanup:
	if (STATUS_FAILED(return_code) && (out_number != NULL))
	{
		*out_number = 0;
	}
	return return_code;
}

STATUS_CODE generate_secure_random_double(double* out_number, double minimum_value, double maximum_value)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	return_code = initialize_sodium_library();
	if (STATUS_FAILED(return_code))
	{
		goto cleanup;
	}

	if ((NULL == out_number) || (maximum_value <= minimum_value))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}
	
	uint64_t random_integer = 0; // Larger number to later cast to double
	randombytes_buf(&random_integer, sizeof(random_integer));

	// Normalize to a value between 0.0 and 1.0
	double normalized = (double)random_integer / (double)UINT64_MAX;

	// Apply the normalized value to the range of minimum_value and maximum_value
	*out_number = minimum_value + normalized * (maximum_value - minimum_value);

	return_code = STATUS_CODE_SUCCESS;

cleanup:
	if (STATUS_FAILED(return_code) && (out_number != NULL))
	{
		*out_number = 0.0;
	}
	return return_code;
}
