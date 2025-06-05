#include "Math/FieldBasicOperations.h"

int64_t multiply_over_galois_field(int64_t first_element, int64_t second_element, uint32_t prime_field)
{
    return (prime_field + (first_element * second_element)) % prime_field;
}

int64_t add_over_galois_field(int64_t first_element, int64_t second_element, uint32_t prime_field)
{
	return (prime_field + first_element + second_element) % prime_field;
}

int64_t negate_over_galois_field(int64_t element, uint32_t prime_field)
{
	return (prime_field - (element % prime_field)) % prime_field;
}

int64_t align_to_galois_field(int64_t element, uint32_t prime_field)
{
	return (prime_field + element) % prime_field;
}

int64_t raise_power_over_galois_field(int64_t base, int64_t exponent, int64_t field)
{
	int64_t result = 0;
	if (base < 0 || exponent < 0 || field <= 0)
	{
		return -1;
	}

	result = 1;
	base = base % field;

	while (exponent > 0)
	{
		if (exponent & 1)
		{
			result = (result * base) % field;
		}
		base = (base * base) % field;
		exponent >>= 1;
	}

cleanup:
	return result;
}
