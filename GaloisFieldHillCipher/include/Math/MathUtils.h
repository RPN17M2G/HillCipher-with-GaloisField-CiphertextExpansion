#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "MatrixUtils.h"
#include "FieldBasicOperations.h"
#include "Cipher/CSPRNG.h"
#include "Math/MatrixDeterminant.h"

/**
 * @brief Calculates the greatest common divisor (GCD) of two elements.
 *
 * @param out_gcd - Pointer to the output GCD value.
 * @param first_element - First element.
 * @param second_element - Second element.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE gcd(int64_t* out_gcd, int64_t first_element, int64_t second_element);

#endif
