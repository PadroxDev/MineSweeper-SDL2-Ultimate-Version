typedef struct Array {
	int capacity;
	int size;
	int* content;
} Array;

void initArray(Array* array, int size);

void insertAt(Array* array, int element, int index);

void insert(Array* array, int element);

void removeAt(Array* array, int index);

int removeEl(Array* array, int element);

int pop(Array* array);

void cleanArray(Array* array);

void displayArray(Array* array);