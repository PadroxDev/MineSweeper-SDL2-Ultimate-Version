#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "App.h"
#include "Board.h"
#include "Settings.h"

void tick(Clock* clock) {
	uint32_t tick = SDL_GetTicks();
	clock->deltaTime = (tick - clock->lastTimeTick) * 0.0001; // In seconds
	clock->lastTimeTick = tick;
}

int initApp(App* app) {
	printf("\nApplication initializing...");

	// Initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("\nSDL could not initialize! SDL_Error: %n", SDL_GetError());
	}

	printf("\n%d IMG Flags initialized", IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG));
	
	if (TTF_Init() < 0) {
		printf("\nSDL_TTF could not initialize! TTF_Error: %n", TTF_GetError());
	}

	app->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	if (!app->window) {
		printf("\nWindow could not be created! SDL_Error: %n", SDL_GetError());
		return -1;
	}

	Uint32 renderFlags = VSYNC ? SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED;
	app->renderer = SDL_CreateRenderer(app->window, -1, renderFlags);

	if (!app->renderer) {
		printf("\nRenderer could not be created! SDL_Error: %n", SDL_GetError());
		return -1;
	}

	app->surface = SDL_GetWindowSurface(app->window);
	app->clock.lastTimeTick = SDL_GetTicks();
	app->running = 1;
	return 1;
}

void displayFPS(App* app) {
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 12);
	SDL_Color color = { 0, 0, 0 };
	
	char* buffer[100];
	sprintf_s(buffer, 100, "Current FPS count: %.2f", app->fpsCount);

	SDL_Surface* fpsSurf = TTF_RenderText_Solid(font, buffer, color);
	SDL_Texture* fpsTex = SDL_CreateTextureFromSurface(app->renderer, fpsSurf);

	SDL_Rect rect = { 5, 5, fpsSurf->w, fpsSurf->h };

	SDL_RenderCopy(app->renderer, fpsTex, NULL, &rect);
}

void displayBoard(App* app, Board* board, SDL_Texture** resources) {
	SDL_Rect clip;
	clip.x = 0;
	clip.y = 0;
	clip.w = 64;
	clip.h = 64;

	for (int i = 0; i < board->width; i++)
	{
		for (int j = 0; j < board->height; j++)
		{
			// More perf ?
			//Slot* slot = getSlot(board, i, j);
			//SDL_RenderCopy(app->renderer, resources[0], &clip, &slot.transform); // Put the transform in the slot and initialize it at the same moment

			SDL_Rect transform;
			transform.x = i * TILE_SIZE;
			transform.y = j * TILE_SIZE;
			transform.w = TILE_SIZE;
			transform.h = TILE_SIZE;

			SDL_RenderCopy(app->renderer, resources[0], &clip, &transform);
		}
	}
}

void quitApp(App* app, SDL_Texture** resources) {
	printf("\nQuitting the application...");
	int resourcesLength = sizeof(resources) / sizeof(resources[0]); // Warning ? 
	printf("\nResources length: %d", resourcesLength);
	for (int i = 0; i < resourcesLength; i++)
	{
		SDL_DestroyTexture(resources[i]);
	}
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	SDL_Quit();
}