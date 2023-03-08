typedef struct Slot {
	int x;
	int y;
	int revealed;
	int flag;
	int bomb;
	int surroundingBombs;
} Slot;

void initSlot(Slot* slot, int x, int y);

typedef struct Board {
	Slot* grid;
	int width;
	int height;
} Board;

int getArrayPosition(Board* board, int x, int y);

Slot* getSlot(Board* board, int x, int y);

void initBoard(Board* board, int width, int height);