#include "encode.h"

unsigned int encode(char *from, char *to, unsigned int rsize){
    FILE *fl1 = fopen(from, "rb");
    FILE *fl2 = fopen(to, "wb");
    if(fl1 == NULL) return 1;
    unsigned int qr = rsize*rsize;
    unsigned char *userdata = (unsigned char*)malloc(qr*4);
    fread(userdata, qr*4, 1, fl1);
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
    fclose(fl1);
    fclose(fl2);
    free(userdata);
    return fsize;
}
