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

void handleEvents(App* app, Board* board);
void handleRender(App* app, Board* board, SDL_Texture* resources[]);

int main() {

	Array availablePositions = { 0,0, NULL }; 
	
	Array tezeea;
	initArray(&availablePositions, 10);
	
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

void handleEvents(App* app, Board* board) {
	SDL_Event events;
	while (SDL_PollEvent(&events) != 0) {
		switch (events.type) {
		case SDL_QUIT:
			app->running = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (events.button.button == SDL_BUTTON_LEFT) {
				for (int i = 0; i < board->width; i++) {
					for (int j = 0; j < board->height; j++) {
						SDL_Point clickPoint = { events.button.x, events.button.y };
						Slot* slot = getSlot(board, i, j);
						SDL_bool result = SDL_PointInRect(&clickPoint, &slot->transform);
						if (result) {
							onSlotClicked(app, board, slot);
							break;
						}
					}
				}
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