#if 0
#include <stdio.h>
#include <libavutil/log.h>

#pragma comment(lib,"avutil.lib")

int main(int argc, char* argv[])
{
	int err_num = 100;

	av_log_set_level(AV_LOG_DEBUG);
	av_log(NULL, AV_LOG_WARNING, "ffmpeg codec error %d! \r\n", 100);

	return 0;
}
#endif