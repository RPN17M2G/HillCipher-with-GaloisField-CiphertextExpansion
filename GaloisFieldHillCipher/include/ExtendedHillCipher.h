#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "StatusCodes.h"
#include "MathUtils.h"
#include "CipherUtils.h"

STATUS_CODE encrypt(int64_t** out_ciphertext, uint32_t* out_ciphertext_size, int64_t** encryption_matrix, uint32_t dimentation, uint32_t prime_field, uint8_t* plaintext_vector, uint32_t vector_size);
STATUS_CODE decrypt(uint8_t** out_plaintext, uint32_t* out_plaintext_size, int64_t** decryption_matrix, uint32_t dimentation, uint32_t prime_field, int64_t* ciphertext_vector, uint32_t vector_size);
