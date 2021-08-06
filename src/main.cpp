#include <iostream>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

int main(){
    printf("initializing all the containers, codecs and protocols.\n");
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        printf("ERROR could not allocate memory for Format Context\n");
        return -1;
    }

    if (avformat_open_input(&pFormatContext, "media/test.mp4", NULL, NULL) != 0) {
        printf("ERROR could not open the file\n");
        return -1;
    }

    printf("\nformat %s, duration %lld us, bit_rate %lld\n", pFormatContext->iformat->name, pFormatContext->duration, pFormatContext->bit_rate);

    if (avformat_find_stream_info(pFormatContext,  NULL) < 0) {
        printf("ERROR could not get the stream info\n");
        return -1;
    }

    AVCodec *pCodec = NULL;
    AVCodecParameters *pCodecParameters =  NULL;
    int video_stream_index = -1;

    for (int i = 0; i < pFormatContext->nb_streams; i++){
        AVCodecParameters *pLocalCodecParameters =  NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        printf("stream index-> %d\n", i);
        printf("AVStream->start_time %d\n", pFormatContext->streams[i]->start_time);
        printf("AVStream->duration %d\n", pFormatContext->streams[i]->duration);
        printf("\nfinding the proper decoder (CODEC)\n\n");

        const AVCodec *pLocalCodec = NULL;
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);
        printf("codec id is: %d\n\n", pLocalCodecParameters->codec_id);

        if (pLocalCodec==NULL) {
            printf("ERROR unsupported codec!\n");
            continue;
        }
    }

    return 0;
}