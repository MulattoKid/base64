#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * BASE64
 * 
 * - Base2: each digit can take on 2 values [0, 1], so each digit represents 1 bit
 * - Base16: each digit can take on 16 values [0, F], so each digit represents 4 bits
 * - Base64: each digit can take on 64 values [A, Z], [a, z], [0, 9], '+' and '/' and
 *           '=' for padding (RFC 4648), so each digit represents 6 bits
 * 
 * Since usually working with bytes, groups of 3 bytes are worked on. This is because
 * - 3 bytes * 8 bits = 24 bits
 * - 24 bits / 6 bits = 4 Base64
 * i.e. it's the smallest common denomitaor for Base2 and Base64. Base64 therefore
 * requires 1/3 more space than Base2.
 * 
 * If a sequence of bytes that's to be Base64 encoded isn't a multiple of 3 the Base64
 * output is usually just padded with the padding digit for the selected version, e.g.
 * '=', to make the length a multiple of 4.
 * 
 * If the Base64 output is to also be used as a regular C-string, then it must of course
 * also contain a null-terminating character.
*/

static const uint8_t base64_rfc_4648_lut[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

bool base64_encode(const uint8_t* const input, const uint32_t input_length, uint8_t* const output, const uint32_t output_length, bool null_terminate)
{
    uint32_t required_output_size = input_length;
    /**
     * The output length must be a multiple of 4, so the input length must be
     * increased to be a multiple of 3. 
    */
    const uint32_t remainder = input_length % 3;
    if (remainder != 0)
    {
        required_output_size += 3 - remainder;
    }
    // The output length requires 1/3 more space than the input
    required_output_size = required_output_size + ((required_output_size + 3 - 1) / 3);
    //Check if to null-terminate
    if (null_terminate == true)
    {
        required_output_size += 1;
    }

    if (required_output_size > output_length)
    {
        printf("[ERROR] output_length must be at least %u, but is only %u\n", required_output_size, output_length);
        return false;
    }

    // Loop over 3 bytes at a time
    const uint32_t iterations = input_length / 3;
    uint32_t input_index = 0;
    uint32_t output_index = 0;
    for (uint32_t i = 0; i < iterations; i++)
    {
        // 6 MSb of 1st byte
        uint8_t first_6 = (input[input_index] & 0b11111100) >> 2;

        // 2 LSb of 1st byte
        uint8_t second_6 = ((input[input_index] & 0b11) << 4);
        input_index += 1;
        // 4 MSb of 2nd byte
        second_6 |= (input[input_index] & 0b11110000) >> 4;

        // 4 LSb of 2nd byte
        uint8_t third_6 = (input[input_index] & 0b1111) << 2;
        input_index += 1;
        // 2 MSB of 3rd byte
        third_6 |= (input[input_index] & 0b11000000) >> 6;

        // 6 LSb of 3rd byte
        uint8_t fourth_6 = input[input_index] & 0b111111;
        input_index += 1;

        // Fill output
        output[output_index++] = base64_rfc_4648_lut[first_6];
        output[output_index++] = base64_rfc_4648_lut[second_6];
        output[output_index++] = base64_rfc_4648_lut[third_6];
        output[output_index++] = base64_rfc_4648_lut[fourth_6];
    }

    // Handle left-over input bytes (will always be < 3)
    const uint32_t remaining_input_bytes = input_length - (iterations * 3);
    if (remaining_input_bytes == 1)
    {
        // 6 MSb of 1st byte
        uint8_t first_6 = (input[input_index] & 0b11111100) >> 2;

        // 2 LSb of 1st byte
        uint8_t second_6 = (input[input_index] & 0b11) << 4;
        input_index += 1;

        // Fill output
        output[output_index++] = base64_rfc_4648_lut[first_6];
        output[output_index++] = base64_rfc_4648_lut[second_6];
        output[output_index++] = '=';
        output[output_index++] = '=';
    }
    else if (remaining_input_bytes == 2)
    {
        // 6 MSb of 1st byte
        uint8_t first_6 = (input[input_index] & 0b11111100) >> 2;

        // 2 LSb of 1st byte
        uint8_t second_6 = (input[input_index] & 0b11) << 4;
        input_index += 1;
        // 4 MSb of 2nd byte
        second_6 |= (input[input_index] & 0b11110000) >> 4;

        // 4 LSb of 3nd byte
        uint8_t third_6 = (input[input_index] & 0b1111) << 2;

        // Fill output
        output[output_index++] = base64_rfc_4648_lut[first_6];
        output[output_index++] = base64_rfc_4648_lut[second_6];
        output[output_index++] = base64_rfc_4648_lut[third_6];
        output[output_index++] = '=';
    }

    if (null_terminate == true)
    {
        output[output_index++] = 0x00;
    }

    return true;
}

int main(int argc, char** argv)
{
    const uint32_t input_length = 2;
    const uint8_t input[input_length + 1] = { 'a', 'b', '\0' };
    printf("Input: %s\n", input);

    const uint32_t output_length = 5;
    uint8_t output[5];
    base64_encode(input, input_length, output, output_length, true);
    printf("Base64 output: %s\n", output);

    return 0;
}