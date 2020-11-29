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

int main(int argc, char* argv[])
{
	char* filepath = "../data/test.mp4";
	if (argc > 1) {
		filepath = argv[1];
	}

	av_register_all();
	avformat_network_init();

	AVFormatContext* pFormatCtx = avformat_alloc_context();

	int ret = avformat_open_input(&pFormatCtx, filepath, NULL, NULL);
	if (ret != 0) {
		printf("avformat_open_input failed!");
		return -1;
	}

	ret = avformat_find_stream_info(pFormatCtx, NULL);
	if (ret < 0) {
		printf("avformat_find_stream_info failed!");
		return -1;
	}
	//av_dump_format(ic,index,url,is_output);打印文件信息
	//index表示第index个流，is_output表示是否是输出（0表示输入，1表示输出）
	av_dump_format(pFormatCtx, 0, filepath, 0);//print file information

	printf("file duration:%d\n", pFormatCtx->duration); //文件时长，时间单位为微秒
	printf("file format:%s\n", pFormatCtx->iformat->long_name);  //文件格式
	printf("video width=%d,height=%d\n", pFormatCtx->streams[1]->codec->width, pFormatCtx->streams[1]->codec->height);
	printf("video pix fmt = %d\n", pFormatCtx->streams[1]->codec->pix_fmt);
	printf("video bit rate = %d\n", pFormatCtx->streams[1]->codec->bit_rate); 
	printf("audio bit rate = %d\n", pFormatCtx->streams[0]->codec->bit_rate);

	getchar();
	return 0;
}
#endif