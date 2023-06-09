/*
Copyright (c) 2023 Daniel Fedai Larsen

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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