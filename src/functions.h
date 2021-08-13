// functions for converting bytes into bits
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

// functions for converting bits into bytes
void bits_to_byte(uint8_t* byte_bits, uint8_t* the_byte) {
  *the_byte = 0 ;
  for(uint8_t i=0; i < 8; i++ ){
    *the_byte<<=1; 
    *the_byte |=byte_bits[i];
  }
}

// funcions for extracting the basename from a path
char *basename(char const *path) {
    char *s = strrchr(path, '/');
    if (!s)
        return strdup(path);
    else
        return strdup(s + 1);
}