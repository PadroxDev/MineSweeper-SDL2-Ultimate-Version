#pragma once
#include "SpriteRenderer.h"

typedef struct Array Array;

typedef struct SDL_Rect2 {
	int x, y;
	int w, h;
} SDL_Rect2;

typedef struct Slot {
	int x;
	int y;
	int revealed;
	int flag;
	int bomb;
	int surroundingBombs;
	SDL_Rect2 transform;
	SpriteRenderer spriteRenderer;
} Slot;

void initSlot(Slot* slot, int x, int y, int offsetX, int offsetY);

typedef struct Board {
	Slot* grid;
	int width;
	int height;
	int firstClick;
	int MED_GRID_SIZE;
} Board;

int getArrayPosition(Board* board, int x, int y);

void getTwoDimensionalPosition(Board* oBoard, int pos, int* outCoords);

Slot* getSlot(Board* board, int x, int y);

void initBoard(Board* board, int width, int height, SDL_Texture* resources[]);

int getBombCountFromRatio(Board* board);

int isInBorder(Board* board, int x, int y);

void elaborateSafeZone(Board* board, Array* positions, int x, int y);

void plantBombs(Board* board, int bombsCount, int startingX, int startingY);

void calculateSurroundingBombs(Board* board);

void digAt(Board* board, int x, int y, int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]);

void recursiveReveal(Board* board, Slot* slot);

int countNearbyFlags(Board* board, int x, int y);

void clearFlags(Board* board);

void revealGrid(Board* board);

void loose(Board* board, int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]);

int isVictory(Board* board);

void win(Board* board, int* displayResult, SpriteRenderer* result, SDL_Texture* resources[]);