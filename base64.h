#ifndef BASE64_H
#define BASE64_H

#include <stdbool.h>
#include <stdint.h>

bool base64_encode(const uint8_t* const input, const uint32_t input_length, uint8_t* const output, const uint32_t output_length, bool null_terminate);
bool base64_decode(const uint8_t* const input, const uint32_t input_length, uint8_t* const output, const uint32_t output_length, bool null_terminated);

#endif
