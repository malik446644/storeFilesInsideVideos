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

    if(argc < 2){
        printf("program usage: main <verb>\n");
        return 0;
    }

    if(strcmp(argv[1], "encode") == 0){
        //converting a buffer of bytes into a video
        file_to_video("test.txt", "test.mp4");
    }else if(strcmp(argv[1], "decode") == 0){
        // converting a video to a file
        video_to_file("test.mp4");
    }

    return 0;
}