# UtuakGameGraphics
Image format for games
## Add to project
```c
#include "encode.h" //encode function
#include "decode.h" //decode function
```
## Examples
```c
int main(void){
    unsigned int size = encode("bytes_array", "tex.ugg", 2048);
    decode("tex.ugg", "bytes_array", 2048, size);
    return 0;
}
```
