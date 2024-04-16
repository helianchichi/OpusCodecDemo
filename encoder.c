#include <stdio.h>
#include <opus.h>
#include <stdlib.h>

#define FRAME_SIZE 960 // 每次读取的 PCM 数据帧大小

int main()
{
    int error;
    OpusEncoder *encoder;
    FILE *pcmFile = fopen("48k_mono.pcm", "rb");
    FILE *dataFile = fopen("48k_mono.opus", "a+");
    FILE *lenFile = fopen("48k_mono.txt", "wa");

    if (pcmFile == NULL || dataFile == NULL)
    {
        fprintf(stderr, "Error opening PCM file\n");
        return 1;
    }

    encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_AUDIO, &error);
    if (error < 0)
    {
        fprintf(stderr, "Error creating Opus encoder: %s\n", opus_strerror(error));
        return 1;
    }
    opus_encoder_ctl(encoder, OPUS_SET_INBAND_FEC(1));
    // 读取 PCM 数据并编码
    short pcm_buffer[FRAME_SIZE];
    unsigned char opus_buffer[FRAME_SIZE*2];
    while (fread(pcm_buffer, sizeof(short), FRAME_SIZE, pcmFile) == FRAME_SIZE)
    {
        int opus_size = opus_encode(encoder, pcm_buffer, FRAME_SIZE, opus_buffer, sizeof(opus_buffer));
        if (opus_size < 0)
        {
            fprintf(stderr, "Opus encoding error\n");
            break;
        }
        else
        {
            fwrite(opus_buffer, opus_size, 1, dataFile);
            fprintf(lenFile, "%d\n", opus_size);
        }
    }

    fclose(pcmFile);
    fclose(dataFile);
    fclose(lenFile);
    opus_encoder_destroy(encoder);
}