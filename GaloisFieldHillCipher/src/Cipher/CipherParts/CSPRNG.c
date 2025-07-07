#include "../../../include/Cipher/CipherParts/CSPRNG.h"

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
	
	if ((NULL == out_number) || (maximum_value < minimum_value))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	*out_number = randombytes_uniform(maximum_value - minimum_value) + minimum_value;

	return_code = STATUS_CODE_SUCCESS;

cleanup:
	return return_code;
}
