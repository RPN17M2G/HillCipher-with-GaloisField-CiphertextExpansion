#include "Math/MathUtils.h"

STATUS_CODE gcd(int64_t* out_gcd, int64_t first_element, int64_t second_element)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    int64_t temp = 0;

    if ((0 == first_element) && (0 == second_element))
    {
        log_error("[!] Invalid argument: both elements are zero in GCD computation");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Computing GCD of %ld and %ld", first_element, second_element);

    second_element = (second_element > 0) ? second_element : (second_element * -1);
    first_element = (first_element > 0) ? first_element : (first_element * -1);
    log_debug("Using absolute values: |a|=%ld, |b|=%ld", first_element, second_element);

    // Euclidean algorithm
    while (second_element != 0)
    {
        temp = second_element;
        second_element = first_element % second_element;
        first_element = temp;
        log_debug("Euclidean step: a=%ld, b=%ld", first_element, second_element);
    }

    *out_gcd = first_element;
    log_debug("GCD result: %ld", first_element);
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    return return_code;
}

STATUS_CODE add_two_vectors_over_gf(int64_t** out_vector, int64_t* first_vector, int64_t* second_vector, uint32_t length, uint32_t prime_field)
{
    STATUS_CODE return_code = STATUS_CODE_UNINITIALIZED;
    size_t index = 0;
    int64_t* vector_buffer = NULL;

    if (!out_vector || !first_vector || !second_vector)
    {
        log_error("[!] Invalid arguments in vector addition: %s",
            !out_vector ? "out_vector is NULL" :
            !first_vector ? "first_vector is NULL" :
            "second_vector is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Adding vectors over GF(%u), length=%u", prime_field, length);

    vector_buffer = (int64_t*)malloc(length * sizeof(int64_t));
    if (NULL == vector_buffer)
    {
        log_error("[!] Memory allocation failed for vector addition (size: %u)", length);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (index = 0; index < length; ++index)
    {
        vector_buffer[index] = add_over_galois_field(first_vector[index], second_vector[index], prime_field);
    }

    log_debug("Vector addition completed successfully");
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

    if (!out_vector || !first_vector || !second_vector)
    {
        log_error("[!] Invalid arguments in vector subtraction: %s",
            !out_vector ? "out_vector is NULL" :
            !first_vector ? "first_vector is NULL" :
            "second_vector is NULL");
        return_code = STATUS_CODE_INVALID_ARGUMENT;
        goto cleanup;
    }

    log_debug("Subtracting vectors over GF(%u), length=%u", prime_field, length);

    vector_buffer = (int64_t*)malloc(length * sizeof(int64_t));
    if (NULL == vector_buffer)
    {
        log_error("[!] Memory allocation failed for vector subtraction (size: %u)", length);
        return_code = STATUS_CODE_ERROR_MEMORY_ALLOCATION;
        goto cleanup;
    }

    for (index = 0; index < length; ++index)
    {
        int64_t negated = negate_over_galois_field(second_vector[index], prime_field);
        vector_buffer[index] = add_over_galois_field(first_vector[index], negated, prime_field);
    }

    log_debug("Vector subtraction completed successfully");
    *out_vector = vector_buffer;
    vector_buffer = NULL;
    return_code = STATUS_CODE_SUCCESS;

cleanup:
    free(vector_buffer);
    return return_code;
}

bool is_prime(int64_t number)
{
    log_debug("Testing primality of %ld", number);

    if (number <= 1)
    {
        log_debug("Number %ld is not prime (≤ 1)", number);
        return false;
    }
    if (number <= 3)
    {
        log_debug("Number %ld is prime (2 or 3)", number);
        return true;
    }
    if (IS_EVEN(number) || (number % 3 == 0))
    {
        log_debug("Number %ld is not prime (divisible by 2 or 3)", number);
        return false;
    }

    // Check divisors from 5 to sqrt(number)
    for (int64_t divisor = 5; divisor * divisor <= number; divisor += 6)
    {
        if ((number % divisor == 0) || (number % (divisor + 2) == 0))
        {
            log_debug("Number %ld is not prime (divisible by %ld or %ld)",
                     number, divisor, divisor + 2);
            return false;
        }
    }

    log_debug("Number %ld is prime", number);
    return true;
}