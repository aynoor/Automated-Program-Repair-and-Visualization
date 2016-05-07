#include <stdio.h>
#include "dynamic_array.h"

void initialize_array(Array *a, size_t initial_size) {
	a->array = (int *)malloc( sizeof(int) * initial_size);
	a->size = initial_size;
	a->used = 0;
}

void insert_array(Array *a, int element) {
	if (a->used == a->size) {
		a->size = a->size*2;
		a->array = (int *)realloc(a->array, sizeof(int) * a->size );
	}
	a->array[a->used++] = element;
}
 void free_array(Array *a) {
 	free(a->array);
 	a->used = 0;
 	a->size = 0;
 	a->array = NULL;
 }