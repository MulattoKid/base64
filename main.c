#include "base64.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    const uint32_t binary_input_length = 3;
    const uint8_t binary_input[binary_input_length + 1] = { '-', 'c', '4', '\0' };
    printf("Input: %s\n", binary_input);

    const uint32_t base64_output_length = 5;
    uint8_t base64_output[5];
    base64_encode(binary_input, binary_input_length, base64_output, base64_output_length, true);
    printf("Base64 output: %s\n", base64_output);

    const uint32_t binary_output_length = binary_input_length;
    uint8_t binary_output[binary_output_length + 1] = { 0 };
    base64_decode(base64_output, base64_output_length, binary_output, binary_output_length, true);
    printf("Binary output: %s\n", binary_output);

    return 0;
}