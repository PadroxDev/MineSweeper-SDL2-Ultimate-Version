#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "Board.h"
#include "App.h"
#include "Settings.h"
#include "Array.h"

void initSlot(Slot* slot, int x, int y, int offsetX, int offsetY) {
	slot->x = x;
	slot->y = y;
	slot->transform.x = TILE_SIZE * x + offsetX;
	slot->transform.y = TILE_SIZE * y + offsetY;
	slot->transform.w = TILE_SIZE;
	slot->transform.h = TILE_SIZE;
	slot->revealed = 0;
	slot->flag = 0;
	slot->bomb = 0;
	slot->surroundingBombs = 0;
}

int getArrayPosition(Board* board, int x, int y) {
	return (x * board->width + y);
}

void getTwoDimensionalPosition(Board* board, int pos, int* outCoords) {
	outCoords[1] = pos % board->width;
	outCoords[0] = (pos - outCoords[1]) / board->width;
}

Slot* getSlot(Board* board, int x, int y) {
	int pos = getArrayPosition(board, x, y);
	return &board->grid[pos];
}

void initBoard(Board* board, int width, int height) {
	printf("\nBoard's initializing...");

	board->width = width;
	board->height = height;
	board->firstClick = 1;

	board->grid = (Slot*)malloc(sizeof(Slot) * (board->width * board->height));
	if (board->grid) // Successfully allocated?
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				int centerX = WINDOW_WIDTH / 2 - (board->width * TILE_SIZE) / 2;
				int centerY = WINDOW_HEIGHT / 2 - (board->height * TILE_SIZE) / 2;

				// Initialize every slots
				Slot* slot = getSlot(board, i, j);
				initSlot(slot, i, j, centerX, centerY);
			}
		}
	}
}

int getBombCountFromRatio(Board* board) {
	return (int)ceil((board->width * board->height) / BOMBS_RATIO);
}

int isInBorder(Board* board, int x, int y) {
	return (x >= 0 && x < board->width && y >= 0 && y < board->height);
}

void elaborateSafeZone(Board* board, Array* positions, int x, int y) {
	for (int i = x - 1; i < x + 2; i++) {
		for (int j = y - 1; j < y + 2; j++) {
			if (isInBorder(board, i, j)) {
				removeEl(positions, getArrayPosition(board, i, j));
			}
		}
	}
}

void plantBombs(Board* board, int quantity, int startingX, int startingY) {
	srand(time(NULL));

	Array availablePositions = {0,0,NULL};
	initArray(&availablePositions, board->width * board->height);

	for (int i = 0; i < board->width; i++) {
		for (int j = 0; j < board->height; j++) {
			int pos = getArrayPosition(board, i, j);
			availablePositions.content[pos] = pos;
		}
	}

	elaborateSafeZone(board, &availablePositions, startingX, startingY);

	for (int i = 0; i < quantity; i++) {
		int iRandPos = rand() % availablePositions.size;
		int coords[2];
		getTwoDimensionalPosition(board, availablePositions.content[iRandPos], &coords); // Why???
		Slot* slot = getSlot(board, coords[0], coords[1]);
		slot->bomb = 1;
		removeAt(&availablePositions, iRandPos);
	}

	cleanArray(&availablePositions);
}

void calculateSurroundingBombs(Board* board) {
	for (int i = 0; i < board->width; i++) {
		for (int j = 0; j < board->height; j++) {
			Slot* slot = getSlot(board, i, j);
			if (!slot->bomb) {
				for (int a = i - 1; a < i + 2; a++) {
					for (int b = j - 1; b < j + 2; b++) {
						Slot surroundingSlot = *getSlot(board, a, b);
						if (isInBorder(board, a, b) && surroundingSlot.bomb) {
							slot->surroundingBombs++;
						}
					}
				}
			}
		}
	}
}

void digAt(Board* board, int x, int y) {
	Slot* slot = getSlot(board, x, y);
	if (slot->flag) {
		return;
	}
	if (slot->bomb) {
		loose(board);
	}
	else if (slot->surroundingBombs == 0) {
		recursiveReveal(board, slot);
	}
	else {
		slot->revealed = 1;
	}
}

void recursiveReveal(Board* board, Slot* slot) {
	if (slot->revealed) return;
	slot->revealed = 1;
	if (slot->surroundingBombs > 0) {
		return;
	}
	else {
		for (int a = slot->x - 1; a < slot->x + 2; a++) {
			for (int b = slot->y - 1; b < slot->y + 2; b++) {
				if (isInBorder(board, a, b)) {
					Slot* toSpread = getSlot(board, a, b);
					recursiveReveal(board, toSpread);
				}
			}
		}
	}
}

int countNearbyFlags(Board* board, int x, int y) {
	int counter = 0;
	for (int i = x - 1; i < x + 2; i++) {
		for (int j = y - 1; j < y + 2; j++) {
			if (isInBorder(board, i, j)) {
				Slot* slot = getSlot(board, i, j);
				if (slot->flag) {
					counter++;
				}
			}
		}
	}
	return counter;
}

void clearFlags(Board* board) {
	for (int i = 0; i < board->width; i++)
	{
		for (int j = 0; j < board->height; j++)
		{
			Slot* slot = getSlot(board, i, j);
			slot->flag = 0;
		}
	}
}

void revealGrid(Board* board) {
	for (int i = 0; i < board->width; i++) {
		for (int j = 0; j < board->height; j++) {
			Slot* slot = getSlot(board, i, j);
			slot->revealed = 1;
		}
	}
}

void loose(Board* board) {
	revealGrid(board);
	printf("\nYou lost!");
}

int isVictory(Board* board) {
	for (int i = 0; i < board->width; i++)
	{
		for (int j = 0; j < board->height; j++)
		{
			Slot* slot = getSlot(board, i, j);
			if (slot->revealed == 0 && slot->bomb != 1) {
				return 0;
			}
		}
	}
	return 1;
}

void win(Board* board) {
	revealGrid(board);
	printf("\nYou won! Well played!");
}