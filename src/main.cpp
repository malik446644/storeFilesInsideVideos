///////////////////////////////////////
// this program will encode a video  //
///////////////////////////////////////

extern "C" {
    #include <stdio.h>
    #include <stdlib.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
    #include <time.h>
    #include <string.h>
}

#include "functions.h"

int main(int argc, char **argv) {

    // taking information from a file and put it in a memory buffer
    FILE* f = fopen("test.txt", "rb");
    size_t fsize = 1280;
    uint8_t* dstbuffer = (uint8_t*)malloc(fsize);
    for (size_t i = 0; i < 1280; i++){
        dstbuffer[i] = 0x00;
    }
    fread(dstbuffer, 1, fsize, f);
    fclose(f);


    if(argc < 2){
        printf("program usage: main <verb>\n");
        return 0;
    }

    if(strcmp(argv[1], "encode") == 0){
        //converting a buffer of bytes into a video
        file_to_video("test.mp4", dstbuffer, fsize);
    }else if(strcmp(argv[1], "decode") == 0){
        // converting a video to a file
        video_to_file("test.mp4");
    }

    free(dstbuffer);


    return 0;
}