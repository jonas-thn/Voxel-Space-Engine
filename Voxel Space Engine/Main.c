#include <stdio.h>
#include <SDL.h>

#define FALSE 0
#define TRUE 1

#define WIDTH 800
#define HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;

int running = FALSE;

int InitializeWindow(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error inititalizing SDL");
		return FALSE;
	}

	window = SDL_CreateWindow("Voxel Space Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL);

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

void Setup()
{

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

}

void Render()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Rect rect = { 20, 20, 15, 15 };

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer);
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