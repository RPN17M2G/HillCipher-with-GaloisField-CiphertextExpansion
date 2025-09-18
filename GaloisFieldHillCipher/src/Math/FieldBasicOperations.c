#include "Math/FieldBasicOperations.h"

int64_t multiply_over_galois_field(int64_t first_element, int64_t second_element, uint32_t prime_field)
{
    int64_t result = (prime_field + (first_element * second_element)) % prime_field;
    log_debug("GF(%u) multiplication: %ld * %ld = %ld", prime_field, first_element, second_element, result);
    return result;
}

int64_t add_over_galois_field(int64_t first_element, int64_t second_element, uint32_t prime_field)
{
    int64_t result = (prime_field + first_element + second_element) % prime_field;
    log_debug("GF(%u) addition: %ld + %ld = %ld", prime_field, first_element, second_element, result);
    return result;
}

int64_t negate_over_galois_field(int64_t element, uint32_t prime_field)
{
    int64_t result = (prime_field - (element % prime_field)) % prime_field;
    log_debug("GF(%u) negation: -%ld = %ld", prime_field, element, result);
    return result;
}

int64_t align_to_galois_field(int64_t element, uint32_t prime_field)
{
    int64_t result = (prime_field + element) % prime_field;
    log_debug("GF(%u) alignment: %ld -> %ld", prime_field, element, result);
    return result;
}

int64_t raise_power_over_galois_field(int64_t base, int64_t exponent, int64_t field)
{
    if (base < 0 || exponent < 0 || field <= 0)
    {
        log_error("Invalid arguments in raise_power_over_galois_field: base=%ld, exponent=%ld, field=%ld",
                 base, exponent, field);
        return -1;
    }

    log_debug("Computing GF(%ld) power: %ld^%ld", field, base, exponent);
    int64_t result = 1;
    base = base % field;

    while (exponent > 0)
    {
        if (exponent & 1)
        {
            result = (result * base) % field;
            log_debug("Power step (odd exp): intermediate result = %ld", result);
        }
        base = (base * base) % field;
        exponent >>= 1;
        log_debug("Power step: new base = %ld, remaining exp = %ld", base, exponent);
    }

    log_debug("Final power result: %ld", result);
    return result;
}
