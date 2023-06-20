#include "encode.h"

void encode(unsigned char* userdata, char *to, unsigned int rsize){
    FILE *fl2 = fopen(to, "wb");
    fseek(fl2, 4, SEEK_SET);
    unsigned int qr = rsize*rsize;
    unsigned char prev[4];
    unsigned char s = 0;
    unsigned int fsize = 0;
    prev[0] = userdata[0];
    prev[1] = userdata[1];
    prev[2] = userdata[2];
    prev[3] = userdata[3];
    unsigned int f = 1;
    for(int h = 0; h < qr; ++h){
        unsigned int is_repeat =
            prev[0] == userdata[h*4+0] &&
            prev[1] == userdata[h*4+1] &&
            prev[2] == userdata[h*4+2] &&
            prev[3] == userdata[h*4+3];
        if(!is_repeat || s == 255){
            if(f){
                --s;
                f = 0;
            }
            fwrite(&s, sizeof(unsigned char), 1, fl2);
            fwrite(prev, 4, 1, fl2);
            fsize += 5;
            prev[0] = userdata[h*4+0];
            prev[1] = userdata[h*4+1];
            prev[2] = userdata[h*4+2];
            prev[3] = userdata[h*4+3];
            s = 0;
        }else ++s;
    }
    fseek(fl2, 0, SEEK_SET);
    fwrite(&fsize, 4, 1, fl2);
    fclose(fl2);
}
