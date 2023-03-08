#include <stdio.h>
#include <stdlib.h>
#include "Board.h"

void initSlot(Slot* slot, int x, int y) {
	slot->x = x;
	slot->y = y;
	slot->revealed = 0;
	slot->flag = 0;
	slot->bomb = 0;
	slot->surroundingBombs = 0;
}

int getArrayPosition(Board* board, int x, int y) {
	return (x * board->width + y);
}

Slot* getSlot(Board* board, int x, int y) {
	int pos = getArrayPosition(board, x, y);
	return &board->grid[pos];
}

void initBoard(Board* board, int width, int height) {
	printf("\nBoard's initializing...");

	board->width = width;
	board->height = height;

	board->grid = (Slot*)malloc(sizeof(Slot) * (board->width * board->height));
	if (board->grid) // Successfully allocated?
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				// Initialize every slots
				Slot* slot = getSlot(board, i, j);
				initSlot(slot, i, j);
			}
		}
	}
}