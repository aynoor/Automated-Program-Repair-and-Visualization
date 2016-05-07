#include <stdio.h>
#include "dynamic_array.c"

int main(int argc, const char** argv) {
  Array a;
  int i;

  initialize_array(&a, 5);  // initially 5 elements
  for (i = 0; i < 50; i++)
    insert_array(&a, i);  // automatically resizes as necessary
  for (i = 0; i < 50; i++)
    printf("%d\n", a.array[i]);  // print 10th element
  printf("%d\n", a.used);  // print number of elements
  free_array(&a);
  return 0;
}