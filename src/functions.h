#pragma once

// functions for converting bytes into bits
void byte_to_bits(uint8_t* byte_bits, int the_byte);

// functions for converting bits into bytes
void bits_to_byte(uint8_t* byte_bits, uint8_t* the_byte);

// funcions for extracting the basename from a path
char *basename(char const *path);

// calculates the percentage from a value and a maximum value
float get_percentage(int value, int total);