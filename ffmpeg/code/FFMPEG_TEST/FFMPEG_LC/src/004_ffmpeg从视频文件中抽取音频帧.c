#if 1
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/log.h>

void adts_header(char *szAdtsHeader, int dataLen);

int main()
{
	av_register_all();

	AVFormatContext* pFormatCtx = NULL;
	char* url = "../Debug/test/test.mp4";
	int ret = avformat_open_input(&pFormatCtx, url, NULL, NULL);
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "open file test.mp4 error.\r\n");
		return -1;
	}

	av_dump_format(pFormatCtx, 0, url, 0);

	ret = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "can not find best stream");
		avformat_close_input(&pFormatCtx);
		return -1;
	}
	int audio_index = ret;

	char* out_url = "../Debug/test/out.aac";
	FILE* out_pf = fopen(out_url, "wb");
	if (out_pf == NULL)
	{
		av_log(NULL, AV_LOG_ERROR, "can not open out file!\r\n");
		avformat_close_input(&pFormatCtx);
		return -1;
	}

	AVPacket pakcet;
	av_init_packet(&pakcet);
	while (av_read_frame(pFormatCtx, &pakcet) >= 0)
	{
		if (pakcet.stream_index == audio_index)
		{
			char adts_header_buf[7];
			adts_header(adts_header_buf, pakcet.size);
			fwrite(adts_header_buf, 1, 7, out_pf);
			int len = fwrite(pakcet.data, 1, pakcet.size, out_pf);
			if (len != pakcet.size)
			{
				av_log(NULL, AV_LOG_WARNING, "warning, length of data is not equal size of packet\r\n");
			}
		}

		av_packet_unref(&pakcet);
	}

	avformat_close_input(&pFormatCtx);
	if (out_pf)
	{
		fclose(out_pf);
	}

	return 0;
}

void adts_header(char *szAdtsHeader, int dataLen)
{
	int audio_object_type = 2;
	int sampling_frequency_index = 7;
	int channel_config = 2;

	int adtsLen = dataLen + 7;

	szAdtsHeader[0] = 0xff;         //syncword:0xfff                          高8bits
	szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
	szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
	szAdtsHeader[1] |= (0 << 1);    //Layer:0                                 2bits
	szAdtsHeader[1] |= 1;           //protection absent:1                     1bit

	szAdtsHeader[2] = (audio_object_type - 1) << 6;            //profile:audio_object_type - 1                      2bits
	szAdtsHeader[2] |= (sampling_frequency_index & 0x0f) << 2; //sampling frequency index:sampling_frequency_index  4bits
	szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
	szAdtsHeader[2] |= (channel_config & 0x04) >> 2;           //channel configuration:channel_config               高1bit

	szAdtsHeader[3] = (channel_config & 0x03) << 6;     //channel configuration:channel_config      低2bits
	szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
	szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit
	szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit
	szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
	szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   高2bits

	szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits
	szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低3bits
	szAdtsHeader[5] |= 0x1f;                                 //buffer fullness:0x7ff 高5bits
	szAdtsHeader[6] = 0xfc;
}
#endif
