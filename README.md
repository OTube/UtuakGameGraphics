docs deprecated
# UtuakGameGraphics
Image format for games
## Add to project
```c
#include "encode.h" //encode function
#include "decode.h" //decode function
```
## Examples
### Test
```c
int main(void){
    unsigned int size = encode("bytes_array", "tex.ugg", 2048);
    char *arr = decode("tex.ugg", 2048, size);
    free(arr);
    return 0;
}
```
### OpenGL
```c
void renderer_enable(render_conf *rc){
    unsigned char *data = decode("tex.ugg", TEXTURE_SIZE_INT, filesize);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, rc->rectCoord);
    glTexCoordPointer(2, GL_FLOAT, 0, rc->rectTex);
    glGenTextures(1, &rc->texture);
    glBindTexture(GL_TEXTURE_2D, rc->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_SIZE_INT, TEXTURE_SIZE_INT, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
}
```
