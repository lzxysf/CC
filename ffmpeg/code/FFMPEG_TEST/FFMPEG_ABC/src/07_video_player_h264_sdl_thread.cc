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

#pragma comment(lib,"legacy_stdio_definitions.lib")
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

using namespace std;

#define SFM_REFRESH_EVENT (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT (SDL_USEREVENT + 2)
int thread_exit = 0;
int thread_pause = 0;

//SDL_Delay(40)表示40ms解码一帧，那么fps应该为25
//但是解码的帧可能是音频帧，这样就会导致视频帧卡顿，后续解决吧
//解决思路是在此函数中就读取一帧并判断帧类型，只有是视频类型才SDL_PushEvent
int sfp_refresh_thread(void *opaque)
{
	thread_exit = 0;
	thread_pause = 0;
	while (!thread_exit)
	{
		if (!thread_pause)
		{
			SDL_Event event;
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);
		}
		SDL_Delay(40);
	}
	thread_exit = 0;
	thread_pause = 0;

	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}

int main(int argc, char* argv[])
{
	//char* filepath = "../data/bigbuckbunny_480x272.h265";
	char* filepath = "../data/Titanic.mkv";
	//char* filepath = "../data/test.mp4";
	if (argc > 1)
	{
		filepath = argv[1];
	}

	av_register_all();
	avformat_network_init();
	
	AVFormatContext* pFormatCtx = avformat_alloc_context();

	int ret = avformat_open_input(&pFormatCtx, filepath, NULL, NULL);
	if (ret != 0)
	{
		printf("avformat_open_input failed!");
		return -1;
	}
	av_dump_format(pFormatCtx, 0, filepath, 0);//print file information

	ret = avformat_find_stream_info(pFormatCtx, NULL);
	if (ret < 0)
	{
		printf("avformat_find_stream_info failed!");
		return -1;
	}

	int videoindex = -1;
	for (uint8_t i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}
	if (videoindex == -1)
	{
		printf("Didn't find a video stream");
		return -1;
	}
	AVCodecContext *pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL)
	{
		printf("codec not found");
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("Could not open codec.\n");
		return -1;
	}

	AVFrame *pFrame = av_frame_alloc();//存储解码后的AVFrame
	AVFrame *pFrameYUV = av_frame_alloc();//存储转换后的AVFrame
	uint8_t *out_buffer = new uint8_t[avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height)];
	avpicture_fill((AVPicture*)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	AVPacket* packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	struct SwsContext *img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	if (ret != 0)
	{
		printf("SDL_Init failed - %s", SDL_GetError());
		return -1;
	}
	SDL_Window *screen = SDL_CreateWindow("My Camera",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		pCodecCtx->width,
		pCodecCtx->height,
		SDL_WINDOW_OPENGL);
	if (!screen)
	{
		printf("SDL: could not create window - exiting %s\r\n", SDL_GetError());
		return -1;
	}
	SDL_Renderer *sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
	SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);
	SDL_Rect sdlRect;
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = pCodecCtx->width;
	sdlRect.h = pCodecCtx->height;

	int got_picture = 0;
	SDL_Thread* video_tid = SDL_CreateThread(sfp_refresh_thread, NULL, NULL);
	SDL_Event event;
	while (true)
	{
		SDL_WaitEvent(&event);
		if (event.type == SFM_REFRESH_EVENT)
		{
			if (av_read_frame(pFormatCtx, packet) >= 0)
			{
				if (packet->stream_index == videoindex)
				{
					ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
					if (ret < 0)
					{
						printf("decode error\r\n");
						return -1;
					}
					if (got_picture)
					{
						sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
							pFrameYUV->data, pFrameYUV->linesize);

						SDL_UpdateYUVTexture(sdlTexture, &sdlRect,
							pFrameYUV->data[0], pFrameYUV->linesize[0],
							pFrameYUV->data[1], pFrameYUV->linesize[1],
							pFrameYUV->data[2], pFrameYUV->linesize[2]);

						SDL_RenderClear(sdlRenderer);
						SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
						SDL_RenderPresent(sdlRenderer);
						SDL_Delay(50);
					}
				}
				av_free_packet(packet);
			}
			else
			{
				thread_exit = 1;
			}
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				thread_pause = !thread_pause;
			}
		}
		else if (event.type == SDL_QUIT)
		{
			thread_exit = 1;
		}
		else if (event.type == SFM_BREAK_EVENT)
		{
			break;
		}
	}

	while (1)
	{
		int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
		if (ret < 0)
		{
			break;
		}
		if (!got_picture)
		{
			break;
		}
		sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
			pFrameYUV->data, pFrameYUV->linesize);

		SDL_UpdateYUVTexture(sdlTexture, &sdlRect,
			pFrameYUV->data[0], pFrameYUV->linesize[0],
			pFrameYUV->data[1], pFrameYUV->linesize[1],
			pFrameYUV->data[2], pFrameYUV->linesize[2]);
		SDL_RenderClear(sdlRenderer);
		SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
		SDL_RenderPresent(sdlRenderer);
		SDL_Delay(50);
	}

	sws_freeContext(img_convert_ctx);
	SDL_Quit();

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	getchar();
	return 0;
}
#endif