#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "App.h"
#include "Board.h"
#include "Resources.h"
#include "Settings.h"
#include "array.h"
#include "SpriteRenderer.h"

void onMouseLeftButtonDown(SDL_Event* events, App* app, Board* board, Mix_Chunk* soundResources[], Mix_Music* ambientMusic, SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect, int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]);
void onMouseRightButtonDown(SDL_Event* events, App* app, Board* board);
void onWindowResized(SDL_Event* events, App* app, Board* board);
void handleEvents(App* app, Board* boar, Mix_Chunk* soundResources[], Mix_Music* ambientMusic, SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect, int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]);
void handleRender(App* app, Board* board, SDL_Texture* resources[], Mix_Chunk* soundResources[], SpriteRenderer* lobbyBg, SpriteRenderer* icon, SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect, int* enableGameResult, SpriteRenderer* gameResult);
void switchGameState(App* app, Board* board, Mix_Music* ambientMusic);

int main() {
	// Initializing app
	App app;
	if (!initApp(&app)) {
		quitApp(&app, NULL, 0, NULL, NULL, 0);
	}

	// Loading resources
	SDL_Texture* resources[15];
	initResources(&app, resources);

	Mix_Chunk* soundResources[5];
	initSoundResources(&app, soundResources);

	setCursor(&app);

	Mix_Music* ambientMusic;
	ambientMusic = Mix_LoadMUS("res/LobbyMusic.mp3");

	Mix_AllocateChannels(5);
	Mix_Volume(1, MIX_MAX_VOLUME);

	Mix_PlayMusic(ambientMusic, 1);
	Mix_VolumeMusic(MIX_MAX_VOLUME/16);

	Board board;
	initBoard(&board, MED_GRID_SIZE, MED_GRID_SIZE, resources);

	SpriteRenderer lobbyBg;
	initSprite(&lobbyBg, resources[7], 2, LOBBY_BG_ANIMATION_SPEED, NULL, SDL_FALSE, 1152, 1296/2);

	int width, height;
	SDL_GetWindowSize(app.window, &width, &height);

	SDL_Rect iconRect;
	iconRect.x = width / 2 - 128 * 5 / 2;
	iconRect.y = height * 0.1;
	iconRect.w = 128 * 5;
	iconRect.h = 256 * 5 / 4;

	SpriteRenderer icon;
	initSprite(&icon, resources[9], 4, LOBBY_TITLE_ANIMATION_SPEED, &iconRect, 0, 128, 256/4);

	int btnScale = 7;
	int centerX = width / 2 - 32 * btnScale;
	int centerY = height / 2 - 8 * btnScale + height * 0.05;

	SDL_Rect easyBtnRect;
	easyBtnRect.x = centerX;
	easyBtnRect.y = centerY - 24 * btnScale + 5;
	easyBtnRect.w = 64 * btnScale;
	easyBtnRect.h = 16 * btnScale;

	SDL_Rect medBtnRect;
	medBtnRect.x = centerX;
	medBtnRect.y = centerY - 8 * btnScale + 10;
	medBtnRect.w = 64 * btnScale;
	medBtnRect.h = 16 * btnScale;

	SDL_Rect hardBtnRect;
	hardBtnRect.x = centerX;
	hardBtnRect.y = centerY + 8 * btnScale + 15;
	hardBtnRect.w = 64 * btnScale;
	hardBtnRect.h = 16 * btnScale;

	SDL_Rect exitBtnRect;
	exitBtnRect.x = centerX;
	exitBtnRect.y = centerY + 24 * btnScale + 20;
	exitBtnRect.w = 64 * btnScale;
	exitBtnRect.h = 16 * btnScale;

	int max = width * 0.6;
	if (height * 0.8 < max)
		max = height * 0.8;

	int displayGameResult = 0;

	SDL_Rect gameResultRect;
	gameResultRect.x = width / 2 - max / 2;
	gameResultRect.y = height / 2 - max / 2;
	gameResultRect.w = max;
	gameResultRect.h = max;

	SpriteRenderer gameResultRenderer;
	initSprite(&gameResultRenderer, resources[10], 4, GAME_RESULT_ANIMATION_SPEED, &gameResultRect, 0, 150, 150);

	// Gameloop
	while (app.running) {
		Uint64 start = SDL_GetPerformanceCounter();
		tick(&app.clock);

		// Events
		handleEvents(&app, &board, soundResources, ambientMusic, &easyBtnRect, &medBtnRect, &hardBtnRect, &exitBtnRect, &displayGameResult, &displayGameResult, &gameResultRenderer);

		// Render
		handleRender(&app, &board, resources, soundResources, &lobbyBg, &icon, &easyBtnRect, &medBtnRect, &hardBtnRect, &exitBtnRect, &displayGameResult, &gameResultRenderer);

		Uint64 end = SDL_GetPerformanceCounter();
		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		app.fpsCount = 1.0 / elapsed;
	}

	free(board.grid);

	// Quitting app
	int resourcesSize = sizeof(resources) / sizeof(resources[0]);
	int soundResourcesSize = sizeof(soundResources) / sizeof(resources[0]);
	quitApp(&app, resources, resourcesSize, ambientMusic, soundResources, soundResourcesSize);

	return 0;
}

Slot* getClickedSlot(SDL_Event* events, Board* board) {
	for (int i = 0; i < board->width; i++) {
		for (int j = 0; j < board->height; j++) {
			SDL_Point clickPoint = { events->button.x, events->button.y };
			Slot* slot = getSlot(board, i, j);
			SDL_bool result = SDL_PointInRect(&clickPoint, &slot->transform);
			if (result) {
				return slot;
				break;
			}
		}
	}
	return NULL;
}

void onMouseLeftButtonDown(SDL_Event* events, App* app, Board* board, Mix_Chunk* soundResources[], Mix_Music* ambientMusic, SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect, int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]) {
	if (app->menu) {
		SDL_Point clickPoint = { events->button.x, events->button.y };
		if (SDL_PointInRect(&clickPoint, easyBtnRect))
		{	
			switchGameState(app, board, ambientMusic);
			Mix_PlayChannel(2, soundResources[1], 0);
		}
		if (SDL_PointInRect(&clickPoint, medBtnRect)) {
			switchGameState(app, board, ambientMusic);
			Mix_PlayChannel(3, soundResources[1], 0);
		}
		if (SDL_PointInRect(&clickPoint, hardBtnRect)) {
			switchGameState(app, board, ambientMusic);
			Mix_PlayChannel(4, soundResources[1], 0);
		}
		if (SDL_PointInRect(&clickPoint, exitBtnRect)) {
			Mix_PlayChannel(5, soundResources[1], 0);
			app->running = 0;
		}
	}
	else {
		Slot* clickedSlot = getClickedSlot(events, board);
		if (clickedSlot != NULL) {
			onSlotClicked(app, board, clickedSlot, soundResources,  displayResult, result, resources);
		}
	}
}

void onMouseRightButtonDown(SDL_Event* events, App* app, Board* board) {
	Slot* clickedSlot = getClickedSlot(events, board);
	if (clickedSlot != NULL) {
		clickedSlot->flag = clickedSlot->flag == 1 ? 0 : 1;
	}
}

void onWindowResized(SDL_Event* events, App* app, Board* board) {
	int newWidth = events->window.data1;
	int newHeight = events->window.data2;

	float widthPercentage = 0.6;
	float heightPercentage = 0.8;
	float boardWidthSize = (newWidth * widthPercentage);
	float boardHeightSize = (newHeight * heightPercentage);
	float boardMaxSize = boardWidthSize;
	if (boardMaxSize > boardHeightSize) {
		boardMaxSize = boardHeightSize;
	}

	int tileSize = (int)floor(boardMaxSize / MED_GRID_SIZE);
	float marginX = ((boardWidthSize / MED_GRID_SIZE) - floor(tileSize)) * MED_GRID_SIZE / 2;
	float marginY = ((boardHeightSize / MED_GRID_SIZE) - floor(tileSize)) * MED_GRID_SIZE / 2;

	float defaultX = (newWidth - boardWidthSize) / 2;
	float defaultY = (newHeight - boardHeightSize) / 2;

	for (int i = 0; i < board->width; i++)
	{
		for (int j = 0; j < board->height; j++)
		{
			Slot* slot = getSlot(board, i, j);
			slot->transform.x = defaultX + tileSize * i + marginX;
			slot->transform.y = defaultY + tileSize * j + marginY;
			slot->transform.w = tileSize;
			slot->transform.h = tileSize;
		}
	}
}

void handleEvents(App* app, Board* board, Mix_Chunk* soundResources[], Mix_Music* ambientMusic ,SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect, int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]) {
	SDL_Event events;
	while (SDL_PollEvent(&events) != 0) {
		switch (events.type) {
		case SDL_QUIT:
			app->running = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (events.button.button == SDL_BUTTON_LEFT) {
				onMouseLeftButtonDown(&events, app, board, soundResources, ambientMusic, easyBtnRect, medBtnRect, hardBtnRect, exitBtnRect, displayResult, result, resources);
			}
			else if (events.button.button == SDL_BUTTON_RIGHT) {
				onMouseRightButtonDown(&events, app, board);
			}
		case SDL_WINDOWEVENT:
			if (events.window.event == SDL_WINDOWEVENT_RESIZED) {
				onWindowResized(&events, app, board);
			}
		default:
			break;
		}
	}
}

void switchGameState(App* app, Board* board, Mix_Music* ambientMusic) {
	app->menu = !app->menu;
	ambientMusic = app->menu ? Mix_LoadMUS("res/LobbyMusic.mp3") : Mix_LoadMUS("res/AmbiantMusic.mp3");
}

void handleRender(App* app, Board* board, SDL_Texture* resources[], Mix_Chunk* soundResources[], SpriteRenderer* lobbyBg, SpriteRenderer* icon, SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect, int* enableGameResult, SpriteRenderer* gameResult) {
	SDL_SetRenderDrawColor(app->renderer, 190, 190, 190, 255);
	SDL_RenderClear(app->renderer); // Clear the window
	if (app->menu) {
		updateSprite(lobbyBg, app);
	}
	else {
		SDL_RenderCopy(app->renderer, resources[0], NULL, NULL);
	}

	displayFPS(app);

	if (app->menu) {
		// Menu
		displayMenu(app, board, resources, icon, easyBtnRect, medBtnRect, hardBtnRect, exitBtnRect);
	}
	else {
		// In game
		displayBoard(app, board, resources);
	}

	if (*enableGameResult != 0) {
		gameResult->spriteSheet = *enableGameResult == 1 ? resources[10] : resources[12];
		updateSprite(gameResult, app);
		if (gameResult->spriteSheet == resources[10] || gameResult->spriteSheet == resources[12]) {
			if (gameResult->lifetime >= gameResult->animationSpeed * gameResult->spriteCount) {
				gameResult->spriteSheet = *enableGameResult == 1 ? resources[11] : resources[13];
				gameResult->lifetime = 0;
			}
		}
	}

	SDL_RenderPresent(app->renderer);
}