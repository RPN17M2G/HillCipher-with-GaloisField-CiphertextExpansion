#ifndef AFFINETRANSFORMATION_HPP
#define AFFINETRANSFORMATION_HPP

#include "StatusCodes.h"
#include "Math/MathUtils.h"

/**
 * @brief Adds multiple error vectors to a vector over a finite field (affine transformation).
 *
 * @param out_transformed_vector Pointer to the output vector that will contain the result.
 * @param error_vectors Array of error vectors to add.
 * @param number_of_error_vectors Number of error vectors.
 * @param vector_to_transform The input vector to which error vectors are added.
 * @param dimension The length of each vector.
 * @param prime_field The modulus for finite field arithmetic.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE add_affine_transformation(int64_t** out_transformed_vector, int64_t** error_vectors, uint32_t number_of_error_vectors, int64_t* vector_to_transform, uint32_t dimension, uint32_t prime_field);

/**
 * @brief Subtracts multiple error vectors from a vector over a finite field (affine transformation).
 *
 * @param out_transformed_vector Pointer to the output vector that will contain the result.
 * @param error_vectors Array of error vectors to subtract.
 * @param number_of_error_vectors Number of error vectors.
 * @param vector_to_transform The input vector from which error vectors are subtracted.
 * @param dimension The length of each vector.
 * @param prime_field The modulus for finite field arithmetic.
 * @return STATUS_CODE - Status of the operation.
 */
STATUS_CODE substruct_affine_transformation(int64_t** out_transformed_vector, int64_t** error_vectors, uint32_t number_of_error_vectors, int64_t* vector_to_transform, uint32_t dimension, uint32_t prime_field);

#endif
