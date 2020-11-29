/**
* 最简单的基于FFmpeg的视频解码器（纯净版）
* Simplest FFmpeg Decoder Pure
*
* 实现了视频码流(支持HEVC，H.264，MPEG2等)解码为YUV数据。
* 仅仅使用了libavcodec（而没有使用libavformat）
* 
* 在非纯净版中媒体数据的编码参数是通过avformat_open_input直接读取到AVCodecContxt中的，
* 在纯净版中不使用libavformat，是通过av_parser_parse2直接解析媒体编码参数的。
* (知道压缩数据的编码参数才知道如何解码)
*/

#if 0
#define _CRT_SECURE_NO_WARNINGS
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

int main(int argc, char* argv[])
{
	avcodec_register_all();

	FILE* in_fp = fopen("../data/in.h264", "rb");
	FILE* out_fp = fopen("../data/out.yuv", "wb");

	AVCodecID codec_id = AV_CODEC_ID_H264;

	AVCodec* pCodec = avcodec_find_decoder(codec_id);
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(pCodec);
	AVCodecParserContext* pCodecParseCtx = av_parser_init(codec_id);

	avcodec_open2(pCodecCtx, pCodec, NULL);

	int ret;
	const int in_buffer_size = 5000;
	uint8_t in_buffer[in_buffer_size + FF_INPUT_BUFFER_PADDING_SIZE];
	int cur_size;
	uint8_t* cur_ptr = NULL;

	AVFrame* pFrame = av_frame_alloc();
	AVFrame* pFrameYUV = av_frame_alloc();
	AVPacket packet;
	av_init_packet(&packet);
	uint8_t *out_buffer = NULL;
	struct SwsContext *img_convert_ctx = NULL;

	int first_time = 1;
	int got_picture = 0;
	while (1)
	{
		cur_size = fread(in_buffer, 1, in_buffer_size, in_fp);
		if (cur_size == 0)
		{
			break;
		}
		cur_ptr = in_buffer;
		while (cur_size > 0)
		{
			int len = av_parser_parse2(pCodecParseCtx, pCodecCtx,
				&packet.data, &packet.size,
				cur_ptr, cur_size,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
			cur_ptr += len;
			cur_size -= len;
			
			if (packet.size == 0)
			{
				continue;
			}

			printf("[Packet]Size:%6d\t", packet.size);
			switch (pCodecParseCtx->pict_type)
			{
				case AV_PICTURE_TYPE_I: printf("Type:I\t"); break;
				case AV_PICTURE_TYPE_P: printf("Type:P\t"); break;
				case AV_PICTURE_TYPE_B: printf("Type:B\t"); break;
				default: printf("Type:Other\t"); break;
			}
			printf("Number:%4d\n", pCodecParseCtx->output_picture_number);

			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
			if (ret < 0)
			{
				printf("decode error\r\n");
				return ret;
			}
			if (got_picture)
			{
				//pFrameYUV要分配内存空间，需要图像的宽高数据
				//纯净版解码器中只有在avcodec_decode_video2解析出第一帧后才能获得宽高
				if (first_time)
				{
					first_time = 0;
					out_buffer = (uint8_t*)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
					avpicture_fill((AVPicture*)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
					img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
						pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
				}
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameYUV->data, pFrameYUV->linesize);
				fwrite(pFrameYUV->data[0], 1, pCodecCtx->width * pCodecCtx->height, out_fp);
				fwrite(pFrameYUV->data[1], 1, pCodecCtx->width * pCodecCtx->height / 4, out_fp);
				fwrite(pFrameYUV->data[2], 1, pCodecCtx->width * pCodecCtx->height / 4, out_fp);
				//printf("succeed to decode one 1 frame\r\n");
			}
			av_free_packet(&packet);
		}
	}

	while (1)
	{
		ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
		if (ret < 0)
		{
			printf("decode error\r\n");
			return ret;
		}
		if (!got_picture)
		{
			break;
		}

		sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
			pFrameYUV->data, pFrameYUV->linesize);
		fwrite(pFrameYUV->data[0], 1, pCodecCtx->width * pCodecCtx->height, out_fp);
		fwrite(pFrameYUV->data[1], 1, pCodecCtx->width * pCodecCtx->height / 4, out_fp);
		fwrite(pFrameYUV->data[2], 1, pCodecCtx->width * pCodecCtx->height / 4, out_fp);
		//printf("succeed to decode one 1 frame\r\n");
	}

	fclose(in_fp);
	fclose(out_fp);
	sws_freeContext(img_convert_ctx);
	av_frame_free(&pFrame);
	av_frame_free(&pFrameYUV);
	av_parser_close(pCodecParseCtx);
	avcodec_close(pCodecCtx);
	av_free(pCodecCtx);
	pCodecCtx = NULL;

	getchar();
	return 0;
}
#endif
