#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stbImage/stb_image.h"

#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#define WIDTH 320
#define HEIGHT 200

const float SCALE_FACTOR = 100.0;

typedef struct
{
	float x;
	float y;
	float zFar;
	float height;
} camera_t;

camera_t camera = { .x = 512, .y = 512, .zFar = 400, .height = 150};

SDL_Window* window;
SDL_Renderer* renderer;

uint32_t* frameBuffer = NULL;
SDL_Texture* frameBufferTexture = NULL;

uint8_t* heightMap = NULL;
uint8_t* colorMap = NULL;
uint32_t* colorMapRGBA = NULL;

int running = FALSE;

int lastFrameTime = 0;

int InitializeWindow(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error inititalizing SDL");
		return FALSE;
	}

	window = SDL_CreateWindow("Voxel Space Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, NULL);

	if (!window)
	{
		printf("Error creating SDL Window");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(window, -1, NULL);

	if (!renderer)
	{
		printf("Error creating SDL Renderer");
		return FALSE;
	}

	return TRUE;

}

void DrawPixel(int x, int y, uint32_t color)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return;
	}
	frameBuffer[(WIDTH * y) + x] = color;
}

void RenderFrameBuffer()
{
	SDL_UpdateTexture(frameBufferTexture, NULL, frameBuffer, (int)(WIDTH * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, frameBufferTexture, NULL, NULL);

	SDL_RenderPresent(renderer);
}

void ClearFameBuffer(uint32_t color)
{
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		frameBuffer[i] = color;
	}
}

void ConvertRGBtoRGBA(const uint8_t* rgbBuffer, uint32_t* rgbaBuffer, size_t numPixels)
{
	for (size_t i = 0; i < numPixels; ++i)
	{
		uint8_t r = rgbBuffer[i * 3 + 0];
		uint8_t g = rgbBuffer[i * 3 + 1];
		uint8_t b = rgbBuffer[i * 3 + 2];

		rgbaBuffer[i] = (b << 24) | (g << 16) | (r << 8) | 0xFF;
	}
}

void Setup()
{
	frameBuffer = (uint32_t*)malloc(WIDTH * HEIGHT * sizeof(uint32_t));
	frameBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

	int width, height, bitDepth1, bitDepth2;

	colorMap = stbi_load("Maps/png/map0.color.png", &width, &height, &bitDepth1, 0);
	heightMap = stbi_load("Maps/png/map0.height.png", &width, &height, &bitDepth2, 0);

	colorMapRGBA = (uint32_t*)malloc(width * height * sizeof(uint32_t));

	ConvertRGBtoRGBA(colorMap, colorMapRGBA, width * height);
}

void Input()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		running = FALSE;
		break;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			running = FALSE;
		}
		break;
	}
}

void Update()
{
	float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0;

	lastFrameTime = SDL_GetTicks();
}

void Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	ClearFameBuffer(0xFF660022);

	//Draw Stuff

	float plx = -camera.zFar;
	float ply = +camera.zFar;

	float prx = +camera.zFar;
	float pry = +camera.zFar;

	for (int i = 0; i < WIDTH; i++)
	{
		float deltaX = (plx + (prx - plx) / WIDTH * i) / camera.zFar;
		float deltaY = (ply + (pry - ply) / WIDTH * i) / camera.zFar;

		float rx = camera.x;
		float ry = camera.y;

		float maxHeight = HEIGHT;

		for (int z = 1; z < camera.zFar; z++)
		{
			rx += deltaX;
			ry -= deltaY;

			int mapOffset = ((1024 * (int)ry) + (int)rx);

			int heightOnScreen = (int)((camera.height - heightMap[mapOffset]) / z * SCALE_FACTOR);

			if (heightOnScreen < 0)
			{
				heightOnScreen = 0;
			}

			if (heightOnScreen > HEIGHT)
			{
				heightOnScreen = HEIGHT - 1;
			}

			if (heightOnScreen < maxHeight)
			{
				for (int y = heightOnScreen; y < maxHeight; y++)
				{
					DrawPixel(i, y, colorMapRGBA[mapOffset]);
				}

				maxHeight = heightOnScreen;
			}
		}
	}

	RenderFrameBuffer();
}

void Destroy()
{
	free(frameBuffer);
	free(colorMapRGBA);

	stbi_image_free(colorMap);
	stbi_image_free(heightMap);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	running = InitializeWindow();

	Setup();

	while (running)
	{
		Input();
		Update();
		Render();
	} 

	Destroy();

	return 0;
}