#include <stdio.h>
#include "Statement.c"

typedef struct {
	Statement* _statements;
	size_t _used;
	size_t _size;
} Statements;