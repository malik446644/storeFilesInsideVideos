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

#include "decoding.h"
#include "encoding.h"


int main(int argc, char **argv) {

    if(argc < 2){
        printf("program usage: main <verb> <file_path>\n");
        return 0;
    }

    if(strcmp(argv[1], "encode") == 0){
        if(argv[2] == NULL){
            printf("[ERROR]: you need to specify the path of the file you want to encode\n");
            return 0;
        }
        if(file_to_video(argv[2], argv[3] == NULL ? "a.mp4" : argv[3]) == 1){
            printf("\n[ERROR] ===> the minimum file size is 72 KB\n");
        };
    }else if(strcmp(argv[1], "decode") == 0){
        if(argv[2] == NULL){
            printf("[ERROR]: you need to specify the path of the video you want to decode\n");
            return 0;
        }
        video_to_file(argv[2]);
    }else{
        printf("[ERROR]: you only have two options ===> encode or decode for <verb>\n");
    }

    return 0;
}