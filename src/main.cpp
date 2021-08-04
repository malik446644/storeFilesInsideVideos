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
    avformat_find_stream_info(pFormatContext,  NULL);

    const AVCodec* pCodec = nullptr;
    AVCodecParameters* pCodecParameters = nullptr;
    AVCodecContext *pCodecContext = nullptr;
    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        pCodecParameters = pFormatContext->streams[i]->codecpar;
        // specific for video and audio
        if (pCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            std::cout << "found the video stream" << std::endl;
            pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
            pCodecContext = avcodec_alloc_context3(pCodec);
            break;
        }
    }
    avcodec_parameters_to_context(pCodecContext, pCodecParameters);
    avcodec_open2(pCodecContext, pCodec, NULL);
    AVPacket *pPacket = av_packet_alloc();
    AVFrame *pFrame = av_frame_alloc();
    int num = 0;
    while (av_read_frame(pFormatContext, pPacket) >= 0) {
        avcodec_send_packet(pCodecContext, pPacket);
        avcodec_receive_frame(pCodecContext, pFrame);
        std::cout << "\n\nprinting frame information" << std::endl;
        std::cout << pCodecContext->frame_number << std::endl;
        std::cout << std::hex << (void*)pFrame->data[0] << std::endl;
        std::cout << std::dec << pFrame->linesize[0] << std::endl;
        std::cout << std::dec << pFrame->width << std::endl;
        std::cout << std::dec << pFrame->height << std::endl;
        num++;
        if (num > 5) break;
    }
    return 0;
}