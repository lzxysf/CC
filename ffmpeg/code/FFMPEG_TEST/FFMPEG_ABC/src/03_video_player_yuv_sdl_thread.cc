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

#define SFM_REFRESH_EVENT (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT (SDL_USEREVENT + 2)
int thread_exit = 0;
int thread_pause = 0;
int sfp_refresh_thread(void *opaque)
{
	thread_exit = 0;
	thread_pause = 0;
	while (!thread_exit)
	{
		if (!thread_pause)
		{
			SDL_Event event;
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);
		}
		SDL_Delay(50);
	}
	thread_exit = 0;
	thread_pause = 0;

	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}

int main(int argc, char* argv[])
{
	int ret;
	int width = 640;
	int height = 360;
	
	char* filepath = "../Debug/test/test_640x360.yuv";
	FILE* in_fp = fopen(filepath, "rb+");
	uint8_t* buffer = new uint8_t[width * height * 3 / 2];
	
	ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	if (ret != 0)
	{
		printf("SDL_Init failed - %s", SDL_GetError());
		return -1;
	}
	SDL_Window *screen = SDL_CreateWindow("My Camera",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL);
	if (!screen)
	{
		printf("SDL: could not create window - exiting %s\r\n", SDL_GetError());
		return -1;
	}
	SDL_Renderer *sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
	SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
	SDL_Rect sdlRect;
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = width;
	sdlRect.h = height;

	SDL_Thread* video_tid = SDL_CreateThread(sfp_refresh_thread, NULL, NULL);
	SDL_Event event;
	while (true)
	{
		SDL_WaitEvent(&event);
		if (event.type == SFM_REFRESH_EVENT)
		{
			if (fread(buffer, 1, width * height * 3 / 2, in_fp) > 0)
			{
				SDL_UpdateTexture(sdlTexture, NULL, buffer, width);
				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
				SDL_RenderPresent(sdlRenderer);
				SDL_Delay(50);
			}
			else
			{
				thread_exit = 1;
			}
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				thread_pause = !thread_pause;
			}
		}
		else if (event.type == SDL_QUIT)
		{
			thread_exit = 1;
		}
		else if (event.type == SFM_BREAK_EVENT)
		{
			break;
		}
	}

	SDL_Quit();

	getchar();
	return 0;
}
#endif
