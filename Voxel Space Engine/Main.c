#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <SDL.h>
#include "Gifdec/gifdec.h"

#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

#define WIDTH 320
#define HEIGHT 200

SDL_Window* window;
SDL_Renderer* renderer;

uint32_t* frameBuffer = NULL;
SDL_Texture* frameBufferTexture = NULL;

uint8_t* heightMap = NULL;
uint8_t* colorMap = NULL;

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

void Setup()
{
	frameBuffer = (uint32_t*)malloc(WIDTH * HEIGHT * sizeof(uint32_t));
	frameBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

	int mapWidth, mapHeight, mapCount;

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

	ClearFameBuffer(0xFF00FF00);

	//Draw Stuff
	DrawPixel(100, 100, 0xFF0000FF);

	RenderFrameBuffer();
}

void Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	running = InitializeWindow();

	Setup();

	printf("Test");

	while (running)
	{
		Input();
		Update();
		Render();
	} 

	Destroy();

	return 0;
}