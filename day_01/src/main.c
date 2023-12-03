#include <stdio.h>
#include <stdlib.h>

#include "aoc_lib.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide a single argument: the path to the file you want to parse\n");
    exit(1);
  }

  char *file_path = argv[1];
  char *contents = read_entire_file(file_path);
  
  char *cursor = contents;
  int sum = 0;
  while (*cursor) sum += get_value_from_line(&cursor);

  printf("Sum = %d\n", sum);

  free(contents);

  return 0;
}

