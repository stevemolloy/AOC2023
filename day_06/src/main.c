#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "aoc_lib.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide a single argument -- the file to be parsed.\n");
    return 1;
  }
  char *buffer;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(argv[1], &buffer, &lines);
  assert(num_lines == 2);

  char *time_cursor = lines[0];
  char *dist_cursor = lines[1];

  advance_to_char(&time_cursor, ':');
  advance_past_chars(&time_cursor, ": ");
  advance_to_char(&dist_cursor, ':');
  advance_past_chars(&dist_cursor, ": ");

  size_t product = 1;
  size_t corrected_time = 0;
  size_t corrected_dist = 0;
  while (*time_cursor) {
    size_t time = get_next_val_from_string(&time_cursor);
    size_t dist = get_next_val_from_string(&dist_cursor);
    advance_past_chars(&time_cursor, " ");
    advance_past_chars(&dist_cursor, " ");

    size_t num_victories = 0;
    for (size_t test_time=0; test_time<time; test_time++) {
      if ((time - test_time) * test_time > dist) num_victories++;
    }

    product *= num_victories;

    corrected_time = corrected_time * ten_to_the_power_of(count_digits(time)) + time;
    corrected_dist = corrected_dist * ten_to_the_power_of(count_digits(dist)) + dist;
  }

  printf("Answer to part 1 = %zu\n", product);

  size_t num_victories = 0;
  for (size_t test_time=0; test_time<corrected_time; test_time++) {
    if ((corrected_time - test_time) * test_time > corrected_dist) num_victories++;
  }
  printf("Answer to part 2 = %zu\n", num_victories);

  free(buffer);
  free(lines);

  return 0;
}

