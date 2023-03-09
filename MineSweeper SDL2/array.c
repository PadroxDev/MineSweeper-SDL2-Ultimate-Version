#include <stdio.h>
#include <stdlib.h>
#include "array.h"

int* reallocateToCapacity(Array* array) {
	return (int*)realloc(array->content, sizeof(int) * array->capacity);
}

void initArray(Array* array, int size) {
	array->capacity = size > 0 ? size * 2 : 1;
	array->size = size;
	array->content = (int*)malloc(sizeof(int) * array->capacity);
}

void insertAt(Array* array, int element, int index) {
	if (array->size++ > array->capacity) {
		array->capacity *= 2;
		int* reallocated = reallocateToCapacity(array);
		if (reallocated) {
			array->content = reallocated;
		}
	}

	for (int i = array->size; i >= index + 1; i--) {
		array->content[i] = array->content[i - 1];
	}
	array->content[index] = element;
}

void insert(Array* array, int element) {
	if (array->size++ > array->capacity) {
		array->capacity *= 2;
		int* reallocated = reallocateToCapacity(array);
		if (reallocated) {
			array->content = reallocated;
		}
	}
	array->content[array->size - 1] = element;
}

void removeAt(Array* array, int index) {
	if (array->size-- <= array->capacity / 2) {
		array->capacity /= 2;
		int* reallocated = reallocateToCapacity(array);
		if (reallocated) {
			array->content = reallocated;
		}
	}

	for (int i = index + 1; i < array->size+1; i++) {
		array->content[i - 1] = array->content[i];
	}
}

int removeEl(Array* array, int element) {
	for (int i = 0; i < array->size; i++) {
		if (array->content[i] == element) {
			removeAt(array, i);
			return 1;
		}
	}
	return 0;
}

int pop(Array* array) {
	if (array->size-- <= array->capacity / 2) {
		array->capacity /= 2;
		int* reallocated = reallocateToCapacity(array);
		if (reallocated) {
			array->content = reallocated;
		}
	}

	int result = array->content[array->size];
	array->content[array->size] = NULL;
	return result;
}

void cleanArray(Array* array) {
	free(array->content);
}

void displayArray(Array* array) {
	printf("\n");
	for (int i = 0; i < array->size; i++) {
		printf("%d, ", array->content[i]);
	}
}