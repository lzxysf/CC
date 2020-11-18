#if 0
#include <stdio.h>
#include <libavformat/avformat.h>

#pragma comment(lib, "avformat.lib")

int main()
{
	int ret = avpriv_io_move("../Debug/test/test.txt", "../Debug/test/test2.txt");
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "failed to move file!\r\n");
		return -1;
	}

	ret = avpriv_io_delete("../Debug/test/test2.txt");
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "failed to delete file!\r\n");
		return -1;
	}
	return 0;
}
#endif
