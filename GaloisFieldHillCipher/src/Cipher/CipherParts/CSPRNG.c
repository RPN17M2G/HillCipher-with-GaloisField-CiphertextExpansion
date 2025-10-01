#include "Cipher/CipherParts/CSPRNG.h"

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

STATUS_CODE secure_fisher_yates_shuffle(uint8_t *array, size_t length)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	uint32_t secure_random_index = 0;
	uint8_t temporary_value = 0;

	if (!array || (0 == length))
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	for (size_t current_index = length - 1; current_index > 0; --current_index)
	{
		return_code = generate_secure_random_number(&secure_random_index, 0, current_index);
		if (STATUS_FAILED(return_code))
		{
			log_error("Failed to generate secure random number for Fisher-Yates");
			goto cleanup;
		}
		temporary_value = array[current_index];
		array[current_index] = array[secure_random_index];
		array[secure_random_index] = temporary_value;
	}

	return_code = STATUS_CODE_SUCCESS;

cleanup:
	return return_code;
}
