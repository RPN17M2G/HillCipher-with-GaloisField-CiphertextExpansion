#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "MathUtils.h"
#include "CipherUtils.h"

STATUS_CODE encrypt(long double** out_ciphertext, uint32_t* out_ciphertext_size, long double** encryption_matrix, uint32_t dimentation, uint32_t prime_field, uint8_t* plaintext_vector, uint32_t vector_size);
STATUS_CODE decrypt(uint8_t** out_plaintext, uint32_t* out_plaintext_size, long double** decryption_matrix, uint32_t dimentation, uint32_t prime_field, long double* ciphertext_vector, uint32_t vector_size);
