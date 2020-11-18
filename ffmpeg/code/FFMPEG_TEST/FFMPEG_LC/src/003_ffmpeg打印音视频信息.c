#if 0
#include <libavformat/avformat.h>
#include <libavutil/log.h>

//获取视频Meta信息
int main(int argc, char* agrv[])
{
	//注册音视频全局解码器
	av_register_all();

	//定义格式上下文指针变量
	AVFormatContext *pFormatCtx = NULL;

	char* url = "../Debug/test/test.mp4";

	//打开多媒体文件
	//第一个参数为指针变量的地址，而不是指针变量
	//第二个参数为要打开的视频文件
	//函数会根据视频文件后缀自动识别视频格式
	int ret = avformat_open_input(&pFormatCtx, url, NULL, NULL);
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "can not open file!\r\n");
		return -1;
	}

	//打印输入或输出流的详细信息
	av_dump_format(pFormatCtx, 0, url, 0);

	avformat_close_input(&pFormatCtx);

	return 0;
}
#endif