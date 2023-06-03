#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	unsigned char r, g, b, a;
}color;

typedef struct{
    unsigned int x, y;
}pointer;

typedef struct{
    float x, y;
}vec2;

typedef struct{
    pointer position;
    pointer scale;
}markdown;

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char fileMarker1;                                                                                                                                                                                              
    unsigned char fileMarker2;                                                                                                                                                                                               
    unsigned int   bfSize;                                                                                                                                                                                                                   
    unsigned short unused1;                                                                                                                                                                                                                        
    unsigned short unused2;                                                                                                                                                                                                                        
    unsigned int   imageDataOffset;                                                                                                                                                            
} FILEHEADER;                                                                                                                                                                                                                                

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned int   biSize;                                                                                                                                                                                                                   
    int            width;                                                                                                                                                                
    int            height;                                                                                                                                                                     
    unsigned short planes;                                                                                                                                                                                                                         
    unsigned short bitPix;                                                                                                                                                                                                                         
    unsigned int   biCompression;                                                                                                                                                                                                            
    unsigned int   biSizeImage;                                                                                                                                                                                                              
    int            biXPelsPerMeter;                                                                                                                                                                                                          
    int            biYPelsPerMeter;                                                                                                                                                                                                          
    unsigned int   biClrUsed;                                                                                                                                                                                                                
    unsigned int   biClrImportant;                                                                                                                                                                                                           
} INFOHEADER;                                                                                                                                                                                                                                

typedef struct{
	FILEHEADER fh;
	INFOHEADER ih;
    pointer bias;
	color *data;
}BITMAP;

/*typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char  b;                                                                                                                                                                                                                        
    unsigned char  g;                                                                                                                                                                                                                        
    unsigned char  r;                                                                                                                                                                                                                        
} IMAGE;*/

#endif
