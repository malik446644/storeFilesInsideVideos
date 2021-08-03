#include <iostream>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

int main(){ 
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if(!pFormatContext){
        std::cout << "cant allocate space for context" << std::endl;
    }
    if(avformat_open_input(&pFormatContext, "media/test.mp4", NULL, NULL) != 0){
        std::cout << "cant open the file" << std::endl;
        return 1;
    }
    std::cout << "successfully opened the file" << std::endl;
    return 0;
}