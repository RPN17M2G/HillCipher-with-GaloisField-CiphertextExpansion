#ifndef FIELD_BASIC_OPERATIONS_H
#define FIELD_BASIC_OPERATIONS_H

#include <stdint.h>

/**
 * @brief Multiplies two elements over a finite field.
 *
 * @param first_element - The first element to multiply.
 * @param second_element - The second element to multiply.
 * @param prime_field - The finite field to use for calculations.
 * @return The result of the operation.
 */
int64_t multiply_over_galois_field(int64_t first_element, int64_t second_element, uint32_t prime_field);

/**
 * @brief Adds two elements over a finite field.
 *
 * @param first_element - The first element to add.
 * @param second_element - The second element to add.
 * @param prime_field - The finite field to use for calculations.
 * @return The result of the operation.
 */
int64_t add_over_galois_field(int64_t first_element, int64_t second_element, uint32_t prime_field);

/**
 * @brief Negates an element over a finite field.
 *
 * @param element - The element to negate.
 * @param prime_field - The finite field to use for calculations.
 * @return The result of the operation.
 */
int64_t negate_over_galois_field(int64_t element, uint32_t prime_field);

/**
 * @brief Aligns a non field element to a finite field.
 *
 * @param element - The element to align.
 * @param prime_field - The finite field to use for calculations.
 * @return The result of the operation.
 */
int64_t align_to_galois_field(int64_t element, uint32_t prime_field);

/**
 * @brief Raises an element to a power over a finite field. Returns -1 if invalid arguments
 *
 * @param base - The base element.
 * @param exponent - The exponent.
 * @param field - The finite field to use for calculations.
 * @return The result of the operation.
 */
int64_t raise_power_over_galois_field(int64_t base, int64_t exponent, int64_t field);

#endif
