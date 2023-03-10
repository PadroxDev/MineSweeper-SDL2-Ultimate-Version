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
	clock->deltaTime = (tick - clock->lastTimeTick) * 0.001; // In seconds
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

	if (Mix_Init(MIX_INIT_MP3) < 0) {
		printf("\nSDL_Mixer could not initialize! MIX_Error: %n", Mix_GetError());
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
		printf("\nSDL_Mixer could not open the audio! MIX_Error: %n", Mix_GetError());
	}

	Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP;
	app->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);

	if (!app->window) {
		printf("\nWindow could not be created! SDL_Error: %n", SDL_GetError());
		return -1;
	}

	SDL_Surface* iconSurf = IMG_Load("res/icon.png");
	SDL_SetWindowIcon(app->window, iconSurf);

	Uint32 renderFlags = VSYNC ? SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED;
	app->renderer = SDL_CreateRenderer(app->window, -1, renderFlags);

	if (!app->renderer) {
		printf("\nRenderer could not be created! SDL_Error: %n", SDL_GetError());
		return -1;
	}

	app->surface = SDL_GetWindowSurface(app->window);
	app->clock.lastTimeTick = SDL_GetTicks();
	app->running = 1;
	app->menu = 1;
	return 1;
}

void setCursor(App* app) {
	SDL_Surface* cursorSurf = IMG_Load(CURSOR_PATH);
	SDL_Cursor* cursor = SDL_CreateColorCursor(cursorSurf, 0, 0);
	SDL_SetCursor(cursor);
	app->cursor = cursor;
}

void displayFPS(App* app) {
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 12);
	SDL_Color color = { 0, 0, 0 };
	
	char* buffer[100];
	sprintf_s(buffer, 100, "Current FPS count: %.2f", app->fpsCount);

	SDL_Surface* fpsSurf = TTF_RenderText_Solid(font, buffer, color);
	SDL_Texture* fpsTex = SDL_CreateTextureFromSurface(app->renderer, fpsSurf);

	SDL_Rect rect = { fpsSurf->w, fpsSurf->h};

	SDL_RenderCopy(app->renderer, fpsTex, NULL, &rect);
}

void displayMenu(App* app, Board* board, SDL_Texture* resources[], SpriteRenderer* spriteRenderer, SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect) {
	updateSprite(spriteRenderer, app);

	SDL_Rect btnClip;
	btnClip.x = 0;
	btnClip.y = 32;
	btnClip.w = 64;
	btnClip.h = 16;

	int x;
	int y;
	int clicked = SDL_GetMouseState(&x, &y);
	SDL_Point mousePoint = { x, y };

	if (SDL_PointInRect(&mousePoint, easyBtnRect)) {
		if (SDL_BUTTON(clicked) == 1) {
			btnClip.y = 16 * 12;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, easyBtnRect);
		}
		else {
			btnClip.y = 16 * 7;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, easyBtnRect);
		}
	}
	else {
		btnClip.y = 16*2;
		SDL_RenderCopy(app->renderer, resources[8], &btnClip, easyBtnRect);
	}

	if (SDL_PointInRect(&mousePoint, medBtnRect)) {
		if (SDL_BUTTON(clicked) == 1) {
			btnClip.y = 16 * 13;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, medBtnRect);
		}
		else {
			btnClip.y = 16 * 8;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, medBtnRect);
		}
	}
	else {
		btnClip.y = 16 * 3;
		SDL_RenderCopy(app->renderer, resources[8], &btnClip, medBtnRect);
	}

	if (SDL_PointInRect(&mousePoint, hardBtnRect)) {
		if (SDL_BUTTON(clicked) == 1) {
			btnClip.y = 16 * 14;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, hardBtnRect);
		}
		else {
			btnClip.y = 16 * 9;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, hardBtnRect);
		}
	}
	else {
		btnClip.y = 16 * 4;
		SDL_RenderCopy(app->renderer, resources[8], &btnClip, hardBtnRect);
	}

	if (SDL_PointInRect(&mousePoint, exitBtnRect)) {
		if (SDL_BUTTON(clicked) == 1) {
			btnClip.y = 16 * 11;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, exitBtnRect);
		}
		else {
			btnClip.y = 16 * 6;
			SDL_RenderCopy(app->renderer, resources[8], &btnClip, exitBtnRect);
		}
	}
	else {
		btnClip.y = 16 * 1;
		SDL_RenderCopy(app->renderer, resources[8], &btnClip, exitBtnRect);
	}
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
						clip.y = TILE_SIZE;
						SDL_RenderCopy(app->renderer, resources[1], &clip, &slot->transform);
					}
				}
			}
			else {
				updateSprite(&slot->spriteRenderer, app);
				if (slot->flag) {
					SDL_RenderCopy(app->renderer, resources[4], &clip, &slot->transform);
				}
			}
			int x;
			int y;
			SDL_GetMouseState(&x, &y);
			SDL_Point mousePoint = { x, y };
			SDL_bool hovering = SDL_PointInRect(&mousePoint, &slot->transform);
			if (hovering) {
				clip.x = 0;
				clip.y = 0;
				SDL_RenderCopy(app->renderer, resources[6], &clip, &slot->transform);
			}
		}
	}
}

void onSlotClicked(App* app, Board* board, Slot* slot, Mix_Chunk* soundResources[], int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]) {
	if (board->firstClick) {
		board->firstClick = 0;
		clearFlags(board);
		int bombCount = getBombCountFromRatio(board);
		plantBombs(board, bombCount, slot->x, slot->y);
		calculateSurroundingBombs(board);
		digAt(board, slot->x, slot->y, displayResult, result, resources);
		Mix_PlayChannel(1, soundResources[0], 0);
	}
	else {
	}
	if (!slot->revealed) {
		digAt(board, slot->x, slot->y, displayResult, result, resources);
		Mix_PlayChannel(1, soundResources[0], 0);
	}
	else if (slot->surroundingBombs > 0 && countNearbyFlags(board, slot->x, slot->y) == slot->surroundingBombs) {
		for (int i = slot->x - 1; i < slot->x + 2; i++) {
			for (int j = slot->y - 1; j < slot->y + 2; j++) {
				if (!isInBorder(board, i, j))
					continue;
				if (!slot->flag && !slot->revealed) {
					digAt(board, i, j, displayResult, result, resources);
					Mix_PlayChannel(1, soundResources[0], 0);
				}
			}
		}
	}
	if (isVictory(board)) {
		win(board, displayResult, result, resources);
	}
}

void quitApp(App* app, SDL_Texture* resources[], int resourcesSize, Mix_Music* ambientMusic, Mix_Chunk* soundResources[], int soundResourcesSize) {
	printf("\nQuitting the application...");
	for (int i = 0; i < resourcesSize; i++)
	{
		SDL_DestroyTexture(resources[i]);
	}
	for (int i = 0; i < soundResourcesSize; i++) {
		Mix_FreeChunk(soundResources[i]);
	}
	SDL_FreeCursor(app->cursor);
	Mix_FreeMusic(ambientMusic);
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	Mix_CloseAudio();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}