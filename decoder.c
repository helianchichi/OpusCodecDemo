#include <stdio.h>
#include <opus.h>
#include <stdlib.h>

#define FRAME_SIZE 960 // 每次读取的 PCM 数据帧大小

int main()
{
    FILE *pcmFile = fopen("48k_mono_dec.pcm", "a+");
    FILE *dataFile = fopen("48k_mono.opus", "rb");
    FILE *lenFile = fopen("48k_mono.txt", "r");

    if (pcmFile == NULL || dataFile == NULL)
    {
        fprintf(stderr, "Error opening PCM file\n");
        return 1;
    }

    int error;
    OpusDecoder *dec;
    dec = opus_decoder_create(48000, 1, &error);

    // 读取 PCM 数据并编码
    short pcm_buffer[FRAME_SIZE*2];
    unsigned char opus_buffer[FRAME_SIZE*2];

    int opus_size = 0;
    while (fscanf(lenFile, "%d", &opus_size) == 1) {
        // 在这里可以对每个读取的整数进行操作
        int randomNumber = rand() % 10;
        fread(opus_buffer, 1, opus_size, dataFile);
        if(randomNumber<2)
        {
            printf("lost packet\n");
        }
        else
        {
            int frame_size = opus_decode(dec, opus_buffer, opus_size, pcm_buffer, FRAME_SIZE*2, 0);
            if (frame_size < 0)
            {
                fprintf(stderr, "Opus decoding error %d\n", frame_size);
                //break;
            }
            else
            {
                printf("frame_size = %d\n", frame_size);
                fwrite(pcm_buffer, frame_size, sizeof(short), pcmFile);
            }
        }
    }

    fclose(pcmFile);
    fclose(dataFile);
    fclose(lenFile);
    opus_decoder_destroy(dec);
}