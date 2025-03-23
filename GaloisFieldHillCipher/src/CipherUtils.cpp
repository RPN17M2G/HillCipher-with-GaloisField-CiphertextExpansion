#include "CipherUtils.h"

STATUS_CODE add_random_bits_between_bytes(double* out_value, double value)
{
	STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
	
	char double_as_byte_array[sizeof(value)];

	uint32_t value_index = 0;

	// Copy value to uint for doing arithmetic operations
	uint64_t value_as_bits = 0;
	if (memcpy_s(&value_as_bits, sizeof(value_as_bits), &value, sizeof(value)))
	{
		return_code = STATUS_CODE_ERROR_COPY_FAILED;
		goto cleanup;
	}

	for (uint32_t i = 0; i < sizeof(value); ++i)
	{
		if (i % BYTE_SIZE == 0)
		{
			double_as_byte_array[i] = 0; // TODO: add random bit
		}
		else
		{
			double_as_byte_array[i] = (value_as_bits >> value_index) & 1;
			++value_index;
		}
	}

	if (memcpy_s(out_value, sizeof(out_value), double_as_byte_array, sizeof(value)))
	{
		return_code = STATUS_CODE_ERROR_COPY_FAILED;
		goto cleanup;
	}

	return_code = STATUS_CODE_SUCCESS;
cleanup:
	return return_code;
}
