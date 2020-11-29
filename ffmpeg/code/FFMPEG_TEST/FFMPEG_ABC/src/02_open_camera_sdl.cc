#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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

using namespace std;

#pragma comment(lib,"legacy_stdio_definitions.lib")
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

#define OUTPUT_ORIGIN_YUV 1 //输出从摄像头采集的原始YUV数据，HD Pro Webcam C920支持的YUV数据为YUYV422
#define OUTPUT_YUV420P 0

int main(int argc, char* argv[])
{
	//init
	av_register_all();
	avdevice_register_all();
	avformat_network_init();

	//show dshow device
	//查看电脑上都有哪些设备 ffmpeg -list_devices true -f dshow -i dummy
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVInputFormat *pInputFormat = av_find_input_format("dshow");
	AVDictionary* options = NULL;
	av_dict_set(&options, "list_devices", "true", 0);
	avformat_open_input(&pFormatCtx, "video=dummy", pInputFormat, &options);
	av_dict_free(&options);
	
	//open input stream
	//查看某个摄像头的支持的参数 ffmpeg -list_options true -f dshow -i video="HD Pro Webcam C920"
	char* in_url = "video=HD Pro Webcam C920";
	av_dict_set(&options, "video_size", "640x360", 0);
	av_dict_set(&options, "framerate", "30", 0);
	int ret = avformat_open_input(&pFormatCtx, in_url, pInputFormat, &options);
	av_dict_free(&options);
	if (ret != 0)
	{
		printf("avformat_open_input failed!");
		return -1;
	}

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
	AVFrame *pFrameYUV = av_frame_alloc();//存储转换后的AVFrame，因为解码后的AVFrame对应图像右边会有一个黑边,通过swscale转换去除黑边
	uint8_t *out_buffer = new uint8_t[avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height)];
	avpicture_fill((AVPicture*)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	AVPacket* packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	struct SwsContext *img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

#if OUTPUT_ORIGIN_YUV
	FILE* fp_o_yuv = fopen("../data/origin.yuv", "wb+");
#endif

#if OUTPUT_YUV420P
	FILE* fp_yuv = fopen("../data/output.yuv", "wb+");
#endif

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

	//av_read_frame 可以从摄像头中读取的数据放到packet中
	//av_read_frame 从摄像头中读取的数据是yuv数据，存储在 packet->data 中，数据的大小为packet->size
	//当 av_read_frame 从h264/mp4等文件读取数据时，那么读取到的 packet->data 就是压缩数据
	//所以说 packet->data 中的数据可以是压缩后的数据，也可以是未压缩的yuv数据
	//av_read_frame 读取视频，读取到的 packet 存放的是完整的一帧图像
	//所以下面使用 avcodec_decode_video2 解码的时候每次 got_picture 都为1，即每次都能得到完整的一帧图像
	int got_picture = 0;
	while (av_read_frame(pFormatCtx, packet)>=0)
	{
		static int count = 0;
		count++;
		if (count==300)
		{
			break;
		}
		if (packet->stream_index == videoindex)
		{
			printf("capture a packet [data:%p size:%d]", packet->data, packet->size);
#if OUTPUT_ORIGIN_YUV
			fwrite(packet->data, 1, packet->size, fp_o_yuv);
#endif
			//1.注意此处解码出来的AVFrame无所谓YUV的格式（I420 YUYV422等）
			//YUV数据分别存放在AVFrame->data[0]、AVFrame->data[1]、AVFrame->data[2]中
			//按照YYYYYYYYUUVV排列存储在文件中就是I420格式，按照YUYVYUYV排列存储在文件中就是YUYV422格式。
			//2.另外，解码后的AVFrame对应图像右边会有一个黑边, 通过swscale转换去除黑边
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

#if OUTPUT_YUV420P
				int size = pCodecCtx->width * pCodecCtx->height;
				fwrite(pFrameYUV->data[0], 1, size, fp_yuv);
				fwrite(pFrameYUV->data[1], 1, size/4, fp_yuv);
				fwrite(pFrameYUV->data[2], 1, size/4, fp_yuv);
#endif
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

	sws_freeContext(img_convert_ctx);
#if OUTPUT_YUV420P
	fclose(fp_yuv);
#endif
	SDL_Quit();

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	getchar();
	return 0;
}
#endif