#include "CipherUtils.h"


STATUS_CODE add_random_bits_between_bytes(CIPHERTEXT_EXPANSION_VECTOR* out_value, double value)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;

	if (NULL == out_value)
	{
		return_code = STATUS_CODE_INVALID_ARGUMENT;
		goto cleanup;
	}

	(*out_value).data = (uint8_t*)malloc(((NUMBER_OF_RANDOM_BYTES_TO_ADD * sizeof(value)) + sizeof(value)) * BYTE_SIZE);
	if (NULL == (*out_value).data)
	{
		return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
		goto cleanup;
	}

	if (memcpy_s((*out_value).data, sizeof(value), &value, sizeof(value)))
	{
		return_code = STATUS_CODE_ERROR_COPY_FAILED;
		goto cleanup;
	}

	for (uint32_t i = 0; i < NUMBER_OF_RANDOM_BYTES_TO_ADD; ++i)
	{
		uint8_t random_byte = 0; // TODO: Add csprng random number
		(*out_value).data[sizeof(value) + i] = random_byte;
	}

    return_code = STATUS_CODE_SUCCESS;
cleanup:
	if (STATUS_FAILED(return_code))
	{
		free((*out_value).data);
		(*out_value).data = NULL;
	}
    return return_code;
}
