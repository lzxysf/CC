#if 0
#include <iostream>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#include <libavutil/audio_fifo.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <SDL.h>
#ifdef __cplusplus
};
#endif

#define USE_SDL 1
#define OUTPUT_PCM 1			//输出的pcm通过 ffplay out.pcm -ac 2 -ar 44100 -f s16le 进行播放

#define MAX_AUDIO_FRAME_SIZE 192000 //48k * 2 * 2

#pragma comment(lib,"legacy_stdio_definitions.lib")
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

using namespace std;

/*
len
|-----------------------|-----------------------|
audio_chunk				 audio_pos

*/
static uint8_t *audio_chunk;//当前音频内存块起始地址
static uint8_t *audio_pos;//当前音频内存块待处理起始地址，即待播放位置
static uint32_t audio_len;//当前音频内存块待处理长度，长度为0时说明当前音频块播放完毕
void fill_audio(void *udata, uint8_t *stream, int len)
{
	//SDL2.0中stream并没有被完全初始化，需要在回调中初始化
	SDL_memset(stream, 0, len);
	if (audio_len == 0)
		return;
	len = (len > audio_len ? audio_len : len);
	//混音到stream流中，因为stream为0，因此是将音频内存块传输给stream，stream流最终将输出给硬件播放
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("SDL_Init failed.\r\n");
		return -1;
	}
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = 44100;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = 2;
	wanted_spec.silence = 0;
	wanted_spec.samples = 1024;
	wanted_spec.callback = fill_audio;

	if (SDL_OpenAudio(&wanted_spec, NULL) < 0)
	{
		printf("can not open audio.\r\n");
		return -1;
	}

	FILE *fp = fopen("../Debug/test/out.pcm", "rb");
	
	int pcm_buffer_size = 4096;
	char* pcm_buffer = (char*)malloc(pcm_buffer_size);

	while (1)
	{
		if (fread(pcm_buffer, 1, pcm_buffer_size, fp) > 0)
		{
			while (audio_len>0)
			{
				SDL_Delay(1);
			}
			audio_chunk = (uint8_t*)pcm_buffer;
			audio_pos = audio_chunk;
			audio_len = pcm_buffer_size;

			SDL_PauseAudio(0);
		}
		else
		{
			break;
		}
	}

	free(pcm_buffer);
	SDL_Quit();
	return 0;
}

#endif
