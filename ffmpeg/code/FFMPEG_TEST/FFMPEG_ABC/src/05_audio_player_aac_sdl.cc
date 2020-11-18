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

#if USE_SDL
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
#endif

int main(int argc, char* argv[])
{
	int ret;
	char* url = "../Debug/test/WavinFlag.aac";
	int audio_stream = -1;

#if OUTPUT_PCM
	FILE* out_pf = fopen("../Debug/test/out.pcm", "wb");
#endif

	av_register_all();
	avformat_network_init();

	AVFormatContext *pFormatCtx = avformat_alloc_context();
	ret = avformat_open_input(&pFormatCtx, url, NULL, NULL);
	if (ret != 0)
	{
		printf("avformat_open_input error\r\n");
		return -1;
	}

	ret = avformat_find_stream_info(pFormatCtx, NULL);
	if (ret < 0)
	{
		printf("Could not find stream info\r\n");
		return -1;
	}

	av_dump_format(pFormatCtx, 0, url, false);

	for (uint8_t i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audio_stream = i;
			break;
		}
	}
	if (audio_stream == -1)
	{
		printf("Did not find a audio stream.\r\n");
		return -1;
	}

	AVCodecContext *pCodecCtx = pFormatCtx->streams[audio_stream]->codec;
	AVCodec* pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		printf("Codec  not found.\r\n");
		return -1;
	}
	ret = avcodec_open2(pCodecCtx, pCodec, NULL);
	if (ret < 0)
	{
		printf("Could not open codec.\r\n");
	}

	AVPacket* packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);
	AVFrame* pFrame = av_frame_alloc();

	uint64_t in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);

	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;//双声道格式
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);//声道数量
	int out_nb_samples = pCodecCtx->frame_size;	//1024，一个音频帧中包含1024个样本
	AVSampleFormat out_sample_format = AV_SAMPLE_FMT_S16;//音频采样格式（采样深度），占16位/2个字节
	int out_sample_rate = 44100;//采样频率，一秒钟的采样个数
	int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_format, 1);//音频帧大小，1024（帧中样本个数）* 2（声道数）* 2（采样深度）= 4096
	uint8_t* out_buffer = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

	struct SwrContext *audio_convert_ctx = swr_alloc();
	audio_convert_ctx = swr_alloc_set_opts(audio_convert_ctx, out_channel_layout, out_sample_format, out_sample_rate,
		in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
	swr_init(audio_convert_ctx);

#if USE_SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("SDL_Init failed.\r\n");
		return -1;
	}
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = out_sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = out_channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = out_nb_samples;
	wanted_spec.callback = fill_audio;
	wanted_spec.userdata = pCodecCtx;

	if (SDL_OpenAudio(&wanted_spec, NULL) < 0)
	{
		printf("can not open audio.\r\n");
		return -1;
	}
#endif

	int got_picture = 0;
	while (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index == audio_stream)
		{
			ret = avcodec_decode_audio4(pCodecCtx, pFrame, &got_picture, packet);
			if (ret < 0)
			{
				printf("Error in decoding audio frame.\r\n");
				return -1;
			}
			//得到了一个解码后的音频帧
			if (got_picture > 0)
			{
				swr_convert(audio_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pFrame->data, pFrame->nb_samples);
#if OUTPUT_PCM
				fwrite(out_buffer, 1, out_buffer_size, out_pf);
#endif
			}
#if USE_SDL
			//等待上一个音频帧处理（播放）完成
			while (audio_len > 0)
			{
				SDL_Delay(1);
			}
			audio_chunk = out_buffer;
			audio_len = out_buffer_size;
			audio_pos = audio_chunk;
			SDL_PauseAudio(0);
#endif
		}
		av_free_packet(packet);
	}
#if USE_SDL
	SDL_CloseAudio();
	SDL_Quit();
#endif
	av_free(out_buffer);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
#if OUTPUT_PCM
	fclose(out_pf);
#endif

	return 0;
}

#endif
