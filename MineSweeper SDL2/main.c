#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "App.h"
#include "Board.h"
#include "Resources.h"
#include "Settings.h"
#include "array.h"

void onMouseLeftButtonDown(SDL_Event* events, App* app, Board* board);
void onMouseRightButtonDown(SDL_Event* events, App* app, Board* board);
void onWindowResized(SDL_Event* events, App* app, Board* board);
void handleEvents(App* app, Board* board);
void handleRender(App* app, Board* board, SDL_Texture* resources[]);

int main() {
	// Initializing app
	App app;
	if (!initApp(&app)) {
		quitApp(&app, NULL, 0);
	}

	// Loading resources
	SDL_Texture* resources[5];
	initResources(&app, resources);

	Board board;
	initBoard(&board, GRID_WIDTH, GRID_HEIGHT);

	// Gameloop
	while (app.running) {
		Uint64 start = SDL_GetPerformanceCounter();
		tick(&app.clock);

		// Events
		handleEvents(&app, &board);

		// Render
		handleRender(&app, &board, resources);

		Uint64 end = SDL_GetPerformanceCounter();
		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		app.fpsCount = 1.0 / elapsed;
	}

	free(board.grid);

	// Quitting app
	int resourcesSize = sizeof(resources) / sizeof(resources[0]);
	quitApp(&app, resources, resourcesSize);

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

void onMouseLeftButtonDown(SDL_Event* events, App* app, Board* board) {
	Slot* clickedSlot = getClickedSlot(events, board);
	if (clickedSlot != NULL) {
		onSlotClicked(app, board, clickedSlot);
	}
}

void onMouseRightButtonDown(SDL_Event* events, App* app, Board* board) {
	Slot* clickedSlot = getClickedSlot(events, board);
	if (clickedSlot != NULL) {
		clickedSlot->flag = clickedSlot->flag == 1 ? 0 : 1;
	}
}

void onWindowResized(SDL_Event* events, App* app, Board* board) {
	//int newWidth = events->window.data1;
	//int newHeight = events->window.data2;
	//float wScale = (float)WINDOW_WIDTH / (float)newWidth;
	//float hScale = (float)WINDOW_HEIGHT / (float)newHeight;

	//for (int i = 0; i < board->width; i++)
	//{
	//	for (int j = 0; j < board->height; j++)
	//	{
	//		int offsetX = newWidth / 2 - (board->width * TILE_SIZE * wScale) / 2;
	//		int offsetY = newHeight / 2 - (board->height * TILE_SIZE * hScale) / 2;

	//		Slot* slot = getSlot(board, i, j);
	//		slot->transform.x = (TILE_SIZE * wScale) * i + offsetX;
	//		slot->transform.y = (TILE_SIZE * hScale) * j + offsetY;
	//		slot->transform.w = (TILE_SIZE * wScale);
	//		slot->transform.h = (TILE_SIZE * hScale);
	//	}
	//}
}

void handleEvents(App* app, Board* board) {
	SDL_Event events;
	while (SDL_PollEvent(&events) != 0) {
		switch (events.type) {
		case SDL_QUIT:
			app->running = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (events.button.button == SDL_BUTTON_LEFT) {
				onMouseLeftButtonDown(&events, app, board);
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

void handleRender(App* app, Board* board, SDL_Texture* resources[]) {
	SDL_SetRenderDrawColor(app->renderer, 190, 190, 190, 255);
	SDL_RenderClear(app->renderer); // Clear the window
	SDL_RenderCopy(app->renderer, resources[0], NULL, NULL);

	displayFPS(app);

	displayBoard(app, board, resources);

	SDL_RenderPresent(app->renderer);
}