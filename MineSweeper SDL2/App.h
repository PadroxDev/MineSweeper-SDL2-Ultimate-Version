#pragma once
#include <SDL_mixer.h>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Surface SDL_Surface;
typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct Board Board;
typedef struct Slot Slot;
typedef struct SpriteRenderer SpriteRenderer;
typedef struct SDL_Rect SDL_Rect;
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
	SDL_Cursor* cursor;
	int running;
	int menu;
	float fpsCount;
} App;

void tick(Clock* clock);

int initApp(App* app);

void setCursor(App* app);

void displayFPS(App* app);

void displayMenu(App* app, Board* board, SDL_Texture* resources[], SpriteRenderer* spriteRenderer, SDL_Rect* easyBtnRect, SDL_Rect* medBtnRect, SDL_Rect* hardBtnRect, SDL_Rect* exitBtnRect);

void displayBoard(App* app, Board* board, SDL_Texture* resources[]);

void onSlotClicked(App* app, Board* board, Slot* slot, Mix_Chunk* soundResources[], int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]);

void quitApp(App* app, SDL_Texture* resources[], int resourcesSize, Mix_Music* ambientMusic, Mix_Chunk* soundResources[], int soundResourcesSize);