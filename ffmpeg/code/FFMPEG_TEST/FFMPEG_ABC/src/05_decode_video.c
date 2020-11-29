/*
* 首先参考09_ffmpeg_pure_decoder，这同样是一个纯净的解码器，仅仅使用了libavcodec（而没有使用libavformat）
*/
#if 0
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
#ifdef __cplusplus
};
#endif

int got_picture;

AVCodec* codec = NULL;
AVCodecContext* codec_ctx = NULL;
AVCodecParserContext* codec_parse_ctx = NULL;
AVFrame* frame = NULL;
AVPacket packet;

void open_decoder()
{
	avcodec_register_all();
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	codec_ctx = avcodec_alloc_context3(codec);
	codec_parse_ctx = av_parser_init(AV_CODEC_ID_H264);
	avcodec_open2(codec_ctx, codec, NULL);
	av_init_packet(&packet);
	frame = av_frame_alloc();
}

void close_decoder()
{
	avcodec_close(codec_ctx);
	av_free(codec_ctx);
	av_frame_free(&frame);
}

void write_out_yuv_frame(AVFrame* frame, FILE* out_fp)
{
	uint8_t **pbuf = frame->data;
	int* pStride = frame->linesize;

	for (int color_idx = 0; color_idx < 3; color_idx++)
	{
		int width = color_idx == 0 ? frame->width : frame->width / 2;
		int height = color_idx == 0 ? frame->height : frame->height / 2;
		for (int idx = 0; idx < height; idx++)
		{
			fwrite(pbuf[color_idx], 1, width, out_fp);
			pbuf[color_idx] += pStride[color_idx];
		}
		fflush(out_fp);
	}
}

int main()
{
	FILE *in_fp, *out_fp;
	in_fp = fopen("../data/in.h264", "rb");
	out_fp = fopen("../data/out.yuv", "wb");

	//FF_INPUT_BUFFER_PADDING_SIZE -> AV_INPUT_BUFFER_PADDING_SIZE 新版本中名称修改了
	uint8_t inbuf[4096 + FF_INPUT_BUFFER_PADDING_SIZE];
	memset(inbuf + 4096, 0, FF_INPUT_BUFFER_PADDING_SIZE);

	open_decoder();

	while (1)
	{
		int uDataSize = fread(inbuf, 1, 4096, in_fp);
		if (uDataSize == 0)
		{
			break;
		}
		uint8_t* pDataPtr = inbuf;
		while (uDataSize > 0)
		{
			//read packet from compress video file
			int len = av_parser_parse2(codec_parse_ctx, codec_ctx,
				&packet.data, &packet.size,
				pDataPtr, uDataSize,
				AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

			pDataPtr += len;
			uDataSize -= len;

			if (packet.size == 0)
			{
				continue;
			}

			//decode frame from packet
			avcodec_decode_video2(codec_ctx, frame, &got_picture, &packet);
			if (got_picture)
			{
				//write frame to yuv file
				write_out_yuv_frame(frame, out_fp);
			}
		}
	}

	packet.data = NULL;
	packet.size = 0;
	while (1)
	{
		int ret = avcodec_decode_video2(codec_ctx, frame, &got_picture, &packet);
		if (got_picture)
		{
			write_out_yuv_frame(frame, out_fp);
		}
		else
		{
			break;
		}
	}

	fclose(in_fp);
	fclose(out_fp);
	close_decoder();
}
#endif