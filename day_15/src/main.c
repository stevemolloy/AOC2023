#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

int main(void) {
  char *file_path = "./test_input.txt";
  // char *file_path = "./real_input.txt";
  char **lines;
  char *buffer;

  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  free(lines);
  free(buffer);

  return 0;
}
