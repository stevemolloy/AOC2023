#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "aoc_lib.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Provide a single argument -- the file to be parsed.\n");
    return 1;
  }
  char * file_path = argv[1];
  char *contents = read_entire_file(file_path);
  char *cursor = contents;

  size_t num_lines = 0;
  while (*cursor) {
    if (*cursor == '\n') num_lines++;
    cursor++;
  }

  size_t num_wn = 0;
  size_t num_my_nums = 0;
  cursor = contents;
  bool cnting_wn = true;
  bool is_num = false;
  while (*cursor != ':') cursor++;
  cursor += 2;
  while (*cursor != '\n') {
    if (cnting_wn && isdigit(*cursor) && !is_num) {
      is_num = true;
      num_wn++;
    } else if (!cnting_wn && isdigit(*cursor) && !is_num) {
      is_num = true;
      num_my_nums++;
    } else if (*cursor == '|') {
      cnting_wn = false;
    } else if (*cursor == ' ') {
      is_num = false;
    }
    cursor++;
  }

  size_t *wns     = calloc(num_wn, sizeof(size_t));
  size_t *my_nums = calloc(num_my_nums, sizeof(size_t));
  size_t *plays   = calloc(num_lines, sizeof(size_t));

  for (size_t i=0; i<num_lines; i++) {
    plays[i] = 1;
  }

  size_t total = 0;
  cursor = contents;
  for (size_t line=0; line<num_lines; line++) {
    while (*cursor != ':') cursor++;
    cursor += 1;
    while (*cursor == ' ') cursor++;
    for (size_t i=0; i<num_wn; i++) {
      size_t val = 0;
      while (isdigit(*cursor)) {
        val = val*10 + *cursor - '0';
        cursor++;
      }
      while (*cursor == ' ') cursor++;
      wns[i] = val;
    }

    while (*cursor != '|') cursor++;
    cursor++;
    while (*cursor == ' ') cursor++;

    for (size_t i=0; i<num_my_nums; i++) {
      size_t val = 0;
      while (isdigit(*cursor)) {
        val = val*10 + *cursor - '0';
        cursor++;
      }
      while (*cursor == ' ') cursor++;
      my_nums[i] = val;
    }

    while (*cursor != '\n') cursor++;

    size_t matches = 0;
    for (size_t i=0; i<num_my_nums; i++) {
      for (size_t j=0; j<num_wn; j++) {
        if (my_nums[i] == wns[j]) matches++;
      }
    }
    size_t score = (matches>0) ? 1 << (matches - 1) : 0;
    total += score;

    for (size_t offs=1; offs<=matches; offs++) {
      plays[line+offs] += plays[line];
    }
  }

  printf("Answer to part 1 = %zu\n", total);

  size_t cards = 0;
  for (size_t i=0; i<num_lines; i++) cards += plays[i];

  printf("Answer to part 2 = %zu\n", cards);

  free(wns);
  free(my_nums);
  free(plays);
  free(contents);

  return 0;
}

