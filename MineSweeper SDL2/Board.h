#pragma once
//#include <SDL.h>

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
} Slot;

void initSlot(Slot* slot, int x, int y, int offsetX, int offsetY);

typedef struct Board {
	Slot* grid;
	int width;
	int height;
	int firstClick;
} Board;

int getArrayPosition(Board* board, int x, int y);

void getTwoDimensionalPosition(Board* oBoard, int pos, int* outCoords);

Slot* getSlot(Board* board, int x, int y);

void initBoard(Board* board, int width, int height);

int getBombCountFromRatio(Board* board);

int isInBorder(Board* board, int x, int y);

void elaborateSafeZone(Board* board, Array* positions, int x, int y);

void plantBombs(Board* board, int bombsCount, int startingX, int startingY);

void calculateSurroundingBombs(Board* board);

void digAt(Board* board, int x, int y);

void recursiveReveal(Board* board, Slot* slot);

int countNearbyFlags(Board* board, int x, int y);

void clearFlags(Board* board);

void revealGrid(Board* board);

void loose(Board* board);

int isVictory(Board* board);

void win(Board* board);