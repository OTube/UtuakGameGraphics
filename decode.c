#include "decode.h"

unsigned char *decode(char *from, unsigned int rsize, unsigned int fsize){
    FILE *fl1 = fopen(from, "rb");
    if(fl1 == NULL) return NULL;
    unsigned char *userdata = (unsigned char*)malloc(rsize*rsize*4);
    unsigned char s;
    unsigned char prev[4];
    unsigned int writed = 0;
    for(unsigned int readed = 0; readed < fsize;){
        fread(&s, 1, 1, fl1);
        fread(prev, 4, 1, fl1);
        for(int k = 0; k < (unsigned int)s+1; ++k){
            userdata[writed++] = prev[0];
            userdata[writed++] = prev[1];
            userdata[writed++] = prev[2];
            userdata[writed++] = prev[3];
        }
        readed += 5;
    }
    fclose(fl1);
    return userdata;
}
