#if 0
#include <libavformat/avformat.h>
#include <libavutil/log.h>

//��ȡ��ƵMeta��Ϣ
int main(int argc, char* agrv[])
{
	//ע������Ƶȫ�ֽ�����
	av_register_all();

	//�����ʽ������ָ�����
	AVFormatContext *pFormatCtx = NULL;

	char* url = "../Debug/test/test.mp4";

	//�򿪶�ý���ļ�
	//��һ������Ϊָ������ĵ�ַ��������ָ�����
	//�ڶ�������ΪҪ�򿪵���Ƶ�ļ�
	//�����������Ƶ�ļ���׺�Զ�ʶ����Ƶ��ʽ
	int ret = avformat_open_input(&pFormatCtx, url, NULL, NULL);
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "can not open file!\r\n");
		return -1;
	}

	//��ӡ��������������ϸ��Ϣ
	av_dump_format(pFormatCtx, 0, url, 0);

	avformat_close_input(&pFormatCtx);

	return 0;
}
#endif