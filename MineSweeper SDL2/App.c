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

	Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	app->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);

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

	SDL_Rect rect = { 50, 50, fpsSurf->w, fpsSurf->h };

	SDL_RenderCopy(app->renderer, fpsTex, NULL, &rect);
}

void displayBoard(App* app, Board* board, SDL_Texture* resources[]) {
	for (int i = 0; i < board->width; i++)
	{
		for (int j = 0; j < board->height; j++)
		{
			SDL_Rect clip;
			clip.x = 0;
			clip.y = 0;
			clip.w = 64;
			clip.h = 64;

			Slot* slot = getSlot(board, i, j);
			if (slot->revealed) {
				if (slot->bomb) {
					SDL_RenderCopy(app->renderer, resources[3], &clip, &slot->transform);
				}
				else {
					if (slot->surroundingBombs > 0) {
						clip.y = TILE_SIZE * (slot->surroundingBombs - 1);
						SDL_RenderCopy(app->renderer, resources[2], &clip, &slot->transform);
					}
					else {
						clip.y = TILE_SIZE * 2;
						SDL_RenderCopy(app->renderer, resources[1], &clip, &slot->transform);
					}
				}
			}
			else {
				SDL_RenderCopy(app->renderer, resources[1], &clip, &slot->transform);
				if (slot->flag) {
					SDL_RenderCopy(app->renderer, resources[4], &clip, &slot->transform);
				}
			}
		}
	}
}

void onSlotClicked(App* app, Board* board, Slot* slot) {
	if (board->firstClick) {
		board->firstClick = 0;
		clearFlags(board);
		int bombCount = getBombCountFromRatio(board);
		plantBombs(board, bombCount, slot->x, slot->y);
		calculateSurroundingBombs(board);
		digAt(board, slot->x, slot->y);
	}
	else {
	}
	if (!slot->revealed)
		digAt(board, slot->x, slot->y);
	else if (slot->surroundingBombs > 0 && countNearbyFlags(board, slot->x, slot->y) == slot->surroundingBombs) {
		for (int i = slot->x - 1; i < slot->x + 2; i++) {
			for (int j = slot->y - 1; j < slot->y + 2; j++) {
				if (!isInBorder(board, i, j))
					continue;
				if (!slot->flag && !slot->revealed)
					digAt(board, i, j);
			}




		}
	}
	if (isVictory(board)) {
		win(board);
	}
}

void quitApp(App* app, SDL_Texture* resources[], int resourcesSize) {
	printf("\nQuitting the application...");
	for (int i = 0; i < resourcesSize; i++)
	{
		SDL_DestroyTexture(resources[i]);
	}
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	SDL_Quit();
}