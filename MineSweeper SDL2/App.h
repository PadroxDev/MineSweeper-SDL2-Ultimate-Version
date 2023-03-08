#pragma once

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Surface SDL_Surface;
typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct Board Board;
typedef struct Slot Slot;
typedef unsigned int uint32_t;

typedef struct Clock {
	uint32_t lastTimeTick;
	double deltaTime;
} Clock;

typedef struct App {
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
	Clock clock;
	int running;
	float fpsCount;
} App;

void tick(Clock* clock);

int initApp(App* app);

void displayFPS(App* app);

void displayBoard(App* app, Board* board, SDL_Texture* resources[]);

void onSlotClicked(App* app, Board* board, Slot* slot);

void quitApp(App* app, SDL_Texture* resources[], int resourcesSize);