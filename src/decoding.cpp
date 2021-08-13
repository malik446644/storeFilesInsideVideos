//////////////////////////////////////////////////////////////////////////////
// this program takes every frame from a video an produces an image from it //
//////////////////////////////////////////////////////////////////////////////

extern "C" {
    #include <stdio.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
}

#include "functions.h"

FILE* outfile;
bool is_header_done = false;
char file_name[1024];
size_t file_size;

static void save_color_frame(uint8_t *buf, int wrap, int xsize, int ysize, char *filename){
    uint8_t bytes[wrap];
    uint8_t bits[8];
    uint8_t byte;
    int ysize_part = ysize / 8;
    for (int x = 0; x < wrap; x++) {
        bits[0] = buf[0 * wrap + x] > 127 ? 1 : 0;
        bits[1] = buf[(ysize_part * 1) * wrap + x] > 127 ? 1 : 0;
        bits[2] = buf[(ysize_part * 2) * wrap + x] > 127 ? 1 : 0;
        bits[3] = buf[(ysize_part * 3) * wrap + x] > 127 ? 1 : 0;
        bits[4] = buf[(ysize_part * 4) * wrap + x] > 127 ? 1 : 0;
        bits[5] = buf[(ysize_part * 5) * wrap + x] > 127 ? 1 : 0;
        bits[6] = buf[(ysize_part * 6) * wrap + x] > 127 ? 1 : 0;
        bits[7] = buf[(ysize_part * 7) * wrap + x] > 127 ? 1 : 0;
        bits_to_byte(bits, &byte);
        bytes[x] = byte;
    }
    if(is_header_done == false) {
        // extract the name and the size of the file
        size_t last_position;
        for (size_t i = 0; true; i++) {
            file_name[i] = bytes[i];
            last_position = i;
            if(bytes[i] == 0x0A) {
                file_name[i] = 0x00;
                break;
            }
        }
        // converting string number to unsigned long long type
        file_size = strtol((const char*)bytes + last_position + 1, NULL, 10);
        // opengin the file with the name extracted from the video header
        outfile = fopen(file_name, "wb");
        // making sure this code happens only for the first frame
        is_header_done = true;
    }else{
        // save the bytes extracted from the frames of the video to the outfile
        fwrite(bytes, 1, file_size < wrap ? file_size : wrap, outfile);
        file_size -= wrap;
    }
}

static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame){
    int response = avcodec_send_packet(pCodecContext, pPacket);
    if (response < 0) {
        printf("Error while sending a packet to the decoder\n");
        return response;
    }

    while (response >= 0) {
        response = avcodec_receive_frame(pCodecContext, pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            break;
        } else if (response < 0) {
            printf("Error while receiving a frame from the decoder");
            return response;
        }

        if (response >= 0) {
            char frame_filename[1024];
            snprintf(frame_filename, sizeof(frame_filename), "media/%s-%d.pgm", "frame", pCodecContext->frame_number);
            if (pFrame->format != AV_PIX_FMT_YUV420P) {
                printf("Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB");
            }
            // save a grayscale frame into a .pgm file
            save_color_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);
        }
    }
    return 0;
}


int video_to_file(char* media){
    
    printf("initializing all the containers, codecs and protocols.\n");
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        printf("ERROR could not allocate memory for Format Context\n");
        return -1;
    }

    if (avformat_open_input(&pFormatContext, media, NULL, NULL) != 0) {
        printf("ERROR could not open the file\n");
        return -1;
    }

    printf("\nformat %s, duration %lld us, bit_rate %lld\n\n", pFormatContext->iformat->name, pFormatContext->duration, pFormatContext->bit_rate);

    if (avformat_find_stream_info(pFormatContext,  NULL) < 0) {
        printf("ERROR could not get the stream info\n");
        return -1;
    }

    const AVCodec *pCodec = NULL;
    const AVCodecParameters *pCodecParameters =  NULL;
    int video_stream_index = -1;

    for (int i = 0; i < pFormatContext->nb_streams; i++){
        AVCodecParameters *pLocalCodecParameters =  NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;

        const AVCodec *pLocalCodec = NULL;
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodec == NULL) {
            printf("ERROR unsupported codec!\n");
            continue;
        }

        // when the stream is a video we store its index, codec parameters and codec
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            if (video_stream_index == -1) {
                video_stream_index = i;
                pCodec = pLocalCodec;
                pCodecParameters = pLocalCodecParameters;
            }
            printf("Video Codec: resolution %d x %d\n", pLocalCodecParameters->width, pLocalCodecParameters->height);
        } else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            printf("Audio Codec: %d channels, sample rate %d\n", pLocalCodecParameters->channels, pLocalCodecParameters->sample_rate);
        }
        printf("\tCodec %s ID %d bit_rate %lld\n\n", pLocalCodec->name, pLocalCodec->id, pLocalCodecParameters->bit_rate);
    }

    if (video_stream_index == -1) {
        printf("this file does not contain a video stream!");
        return -1;
    }

    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
    if (!pCodecContext)
    {
        printf("failed to allocated memory for AVCodecContext\n");
        return -1;
    }

    if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0)
    {
        printf("failed to copy codec params to codec context\n");
        return -1;
    }

    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0)
    {
        printf("failed to open codec through avcodec_open2\n");
        return -1;
    }

    AVFrame *pFrame = av_frame_alloc();
    if (!pFrame)
    {
        printf("failed to allocated memory for AVFrame\n");
        return -1;
    }

    AVPacket *pPacket = av_packet_alloc();
    if (!pPacket)
    {
        printf("failed to allocated memory for AVPacket\n");
        return -1;
    }

    int response = 0;
    while (av_read_frame(pFormatContext, pPacket) >= 0)
    {
        if (pPacket->stream_index == video_stream_index) {
            response = decode_packet(pPacket, pCodecContext, pFrame);
            if (response < 0) break;
        }
        av_packet_unref(pPacket);
    }

    fclose(outfile);

    avformat_close_input(&pFormatContext);
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);

    printf("\n\ndone converting the video back to a file\n\n");

    return 0;
}