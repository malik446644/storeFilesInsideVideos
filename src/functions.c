#include <stdio.h>
#include <string.h>
#include <stdint.h>

void byte_to_bits(uint8_t* byte_bits, int the_byte) {
  int c, k;

  int i = 0;
  for (c = 7; c >= 0; c--){
    k = the_byte >> c;
    if (k & 1)
      byte_bits[i] = 1;
    else
      byte_bits[i] = 0;
      i++;
  }
}

void bits_to_byte(uint8_t* byte_bits, uint8_t* the_byte) {
  *the_byte = 0 ;
  for(uint8_t i=0; i < 8; i++ ){
    *the_byte<<=1; 
    *the_byte |=byte_bits[i];
  }
}

char *basename(char const *path) {
  char *s = strrchr(path, '/');
  if (!s)
    return strdup(path);
  else
    return strdup(s + 1);
}

float get_percentage(int value, int total) {
  float percentage;
  percentage = (float)value / total * 100.0;
  return percentage;
}