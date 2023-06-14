#include "lodepng.h"
#include "encode.h"

#include <stdio.h>
#include <stdlib.h>

unsigned char *decodeOneStep(const char* filename, unsigned int *hei) {
  unsigned error;
  unsigned char* image = 0;
  unsigned width, height;

  error = lodepng_decode32_file(&image, &width, &height, filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  *hei = width;
  return image;
}

int main(int argc, char **argv){
    char name1[16];
    char name2[16];
    sprintf(name1, "%s.png", argv[1]);
    sprintf(name2, "%s.ugg", argv[1]);
    unsigned int s;
    unsigned char *dat = decodeOneStep(name1, &s);
    if(dat == 0) return 1;
    encode(dat, name2, s);
    free(dat);
    return 0;
}
