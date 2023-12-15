#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

size_t hash_next_char(char c, size_t curr_hash) {
  curr_hash += c;
  curr_hash = (curr_hash%256) * 17;
  curr_hash %= 256;
  return curr_hash;
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";

  char *contents = read_entire_file(file_path);
  // char *contents = "rn=1";
  printf("%s\n", contents);

  char *cursor = contents;
  size_t hash = 0;
  size_t total = 0;
  while (*cursor) {
    if (*cursor == '\n') {
    } else if (*cursor == ',') {
      printf("%zu\n", hash);
      total += hash;
      hash = 0;
    } else {
      hash = hash_next_char(*cursor, hash);
    }
    cursor++;
  }
  total += hash;
  printf("Answer to part 1 = %zu\n", total);

  return 0;
}
