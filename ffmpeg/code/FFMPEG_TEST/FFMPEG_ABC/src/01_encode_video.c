#if 1
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

AVCodec* codec = NULL;
AVCodecContext* codec_ctx = NULL;
AVFrame* frame = NULL;
AVPacket packet;

int n_Total_Frames = 300;

int open_encoder()
{
	avcodec_register_all();

	codec = avcodec_find_encoder(AV_CODEC_ID_H264);

	codec_ctx = avcodec_alloc_context3(codec);
	codec_ctx->bit_rate = 200000;
	codec_ctx->width = 352;
	codec_ctx->height = 288;
	codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
	codec_ctx->gop_size = 10;
	codec_ctx->max_b_frames = 0;
	AVRational rational = { 1,25 };
	codec_ctx->time_base = rational;

	avcodec_open2(codec_ctx, codec, NULL);

	frame = av_frame_alloc();
	frame->format = codec_ctx->pix_fmt;
	frame->width = codec_ctx->width;
	frame->height = codec_ctx->height;

	av_image_alloc(frame->data, frame->linesize, codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt, 32);

	return 0;
}

void close_encoder()
{
	avcodec_close(codec_ctx);
	av_free(codec_ctx);
	av_freep(&(frame->data[0]));
	av_frame_free(&frame);
}

int read_yuv_data(AVFrame* frame, FILE* in_fp, int color_plane)
{
	int frame_height;
	int frame_width;
	int frame_size;
	int frame_stride;
	if (color_plane == 0)
	{
		frame_height = frame->height;
		frame_width = frame->width;
	}
	else
	{
		frame_height = frame->height / 2;
		frame_width = frame->width / 2;
	}
	frame_size = frame_width * frame_height;
	frame_stride = frame->linesize[color_plane];

	if (frame_width == frame_stride)
	{
		fread(frame->data[color_plane], frame_size, 1, in_fp);
	}
	else
	{
		for (int row_idx = 0; row_idx < frame_height; row_idx++)
		{
			fread(frame->data[color_plane] + row_idx * frame_stride, frame_width, 1, in_fp);
		}
	}
	return frame_size;
}

int main()
{
	int got_output;
	FILE *in_fp, *out_fp;
	in_fp = fopen("../data/akiyo_cif(352_288).yuv", "rb");
	out_fp = fopen("../data/out.h264", "wb");
	open_encoder();

	for (int frame_index = 0; frame_index < n_Total_Frames; frame_index++)
	{
		av_init_packet(&packet);
		packet.data = NULL;
		packet.size = 0;

		read_yuv_data(frame, in_fp, 0);
		read_yuv_data(frame, in_fp, 1);
		read_yuv_data(frame, in_fp, 2);

		frame->pts = frame_index;

		avcodec_encode_video2(codec_ctx, &packet, frame, &got_output);
		if (got_output)
		{
			fwrite(packet.data, 1, packet.size, out_fp);
			av_packet_unref(&packet);
		}
	}
	//read the last frame if it exists
	while (got_output)
	{
		avcodec_encode_video2(codec_ctx, &packet, NULL, &got_output);
		if (got_output)
		{
			fwrite(packet.data, 1, packet.size, out_fp);
			av_packet_unref(&packet);
		}
	}

	fclose(in_fp);
	fclose(out_fp);
	close_encoder();
	getchar();
}
#endif