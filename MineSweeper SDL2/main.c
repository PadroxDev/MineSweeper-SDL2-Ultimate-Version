#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "App.h"
#include "Board.h"
#include "Resources.h"
#include "Settings.h"

void handleEvents(App* app);
void handleRender(App* app);

int main() {
	// Initializing app
	App app;
	if (!initApp(&app)) {
		quitApp(&app, NULL);
	}

	// Loading resources
	SDL_Texture** resources; // Tableau d'adresses
	initResources(&app, resources); // adresse ou pointeur ?

	Board board;
	initBoard(&board, GRID_WIDTH, GRID_HEIGHT);

	// Gameloop
	while (app.running) {
		Uint64 start = SDL_GetPerformanceCounter();
		tick(&app.clock);

		// Events
		handleEvents(&app);

		// Render
		handleRender(&app, &board, &resources);

		Uint64 end = SDL_GetPerformanceCounter();
		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		app.fpsCount = 1.0 / elapsed;
	}

	free(board.grid);

	// Quitting app
	quitApp(&app, resources);

	return 0;
}

void handleEvents(App* app) {
	SDL_Event events;
	while (SDL_PollEvent(&events) != 0) {
		switch (events.type) {
		case SDL_QUIT:
			app->running = 0;
			break;
		default:
			break;
		}
	}
}

void handleRender(App* app, Board* board, SDL_Texture** resources) {
	SDL_SetRenderDrawColor(app->renderer, 190, 190, 190, 255);
	SDL_RenderClear(app->renderer); // Clear the window

	displayFPS(app);

	displayBoard(app, board, resources);

	SDL_RenderPresent(app->renderer);
}