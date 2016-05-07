#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	int *array;
	size_t size;
	size_t used;
} Array;

void initialize_array(Array *a, size_t initial_size);

void insert_array(Array *a, int element);

void free_array(Array *a);