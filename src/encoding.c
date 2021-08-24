///////////////////////////////////////
// this program will encode a video  //
///////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#include "functions.h"


static void encode_frame(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile){
    int ret;

    /* send the frame to the encoder */
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

int file_to_video(const char *infilepath, const char *outfilepath) {
    const AVCodec *codec;
    AVCodecContext *c= NULL;
    int i, ret;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder_by_name("libx264");
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* put sample parameters */
    c->bit_rate = 1000000 * 4;
    /* resolution must be a multiple of two */
    c->width = 1280;
    c->height = 40;
    /* frames per second */
    c->time_base = (AVRational){1, 24};
    c->framerate = (AVRational){24, 1};

    /* emit one intra frame every ten frames
    * check frame pict_type before passing frame
    * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
    * then gop_size is ignored and the output of encoder
    * will always be I frame irrespective to gop_size
    */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_GRAY8;

    // adding option in code context to make the encoding slower and better
    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);

    /* open it */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    f = fopen(outfilepath, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", outfilepath);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }

    // calculating bytes number per frame
    unsigned long long bytes_size_per_frame = (frame->width * frame->height) / 8;
    
    // open the file to take the data from
    char* infile_name = basename(infilepath);            // extracting the name of the file from the path
    printf("the path is %s\nthe base name is: %s\n", infilepath, infile_name);
    FILE* infile = fopen(infilepath, "rb");
    fseek(infile, 0, SEEK_END);
    size_t infile_size =  ftell(infile);
    if(infile_size < 12800 * 6) {
        fclose(f);
        fclose(infile);
        remove(outfilepath);
        return 1;
    }
    rewind(infile);
    unsigned long long frames_count = infile_size / bytes_size_per_frame;
    printf("size of the file %u\nnumber of frames is: %u\nbytes_size_per_frame: %u\n", infile_size, frames_count, bytes_size_per_frame);

    bool is_header_written = false;

    /* encode video frames */
    int number = 20;
    int print_every = number;
    for (i = 0; i < frames_count + 2; i++) {
        fflush(stdout);

        uint8_t* bytes = (uint8_t*)malloc(bytes_size_per_frame);
        if(is_header_written){
            memset(bytes, 0x00, bytes_size_per_frame);                      // clearing the memory from any old data
            fread(bytes, 1, bytes_size_per_frame, infile);
        }else{
            memset(bytes, 0x00, bytes_size_per_frame);                                  // clearing the memory from any old data
            sprintf((char *)bytes, "%s\n%u\0", infile_name, infile_size);       // adding the header information in the first frame
        }

        /* make sure the frame data is writable */
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);

        // fill realData with gray colors data
        int z = 0;
        for (int x = 0; x < frame->height; x++) {
            for (int y = 0; y < frame->width; y = y + 8) {
                uint8_t arr[8];
                byte_to_bits(arr, bytes[z]);
                frame->data[0][(x * frame->width + y) + 0] = arr[0] == 1 ? 255 : 0;
                frame->data[0][(x * frame->width + y) + 1] = arr[1] == 1 ? 255 : 0;
                frame->data[0][(x * frame->width + y) + 2] = arr[2] == 1 ? 255 : 0;
                frame->data[0][(x * frame->width + y) + 3] = arr[3] == 1 ? 255 : 0;
                frame->data[0][(x * frame->width + y) + 4] = arr[4] == 1 ? 255 : 0;
                frame->data[0][(x * frame->width + y) + 5] = arr[5] == 1 ? 255 : 0;
                frame->data[0][(x * frame->width + y) + 6] = arr[6] == 1 ? 255 : 0;
                frame->data[0][(x * frame->width + y) + 7] = arr[7] == 1 ? 255 : 0;
                z++;
            }
        }

        frame->pts = i;

        /* encode the image */
        encode_frame(c, frame, pkt, f);
        
        // make the program write one header file
        is_header_written = true;

        // freeing the memory allocation
        free(bytes);

        // printing the progress
        if(print_every == 0) {
            printf("progress: %f\n", get_percentage(i, frames_count + 2));
            print_every = number;
        }
        print_every--;
    }

    /* flush the encoder */
    encode_frame(c, NULL, pkt, f);

    /* add sequence end code to have a real MPEG file */
    fwrite(endcode, 1, sizeof(endcode), f);

    // closing all opened files
    fclose(f);
    fclose(infile);

    // sws_freeContext(swsCtx);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);

    printf("\n\ndone converting the file into a video\n\n");

    return 0;
}