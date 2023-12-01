#include <stdio.h>
#include <stdlib.h>

#include "aoc_lib.h"

int main(void) {
  char *file_path = "./real_input.txt";
  char *contents = read_entire_file(file_path);
  
  char *cursor = contents;
  int sum = 0;
  while (*cursor) sum += get_value_from_line(&cursor);

  printf("Sum = %d\n", sum);

  free(contents);

  return 0;
}

