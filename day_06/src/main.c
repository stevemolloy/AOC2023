#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "aoc_lib.h"

size_t count_digits(size_t val) {
  size_t cnt_digits = 0;
  while (val > 0) {
    cnt_digits++;
    val = val / 10;
  }
  return cnt_digits;
}

int main(void) {
  char *file_path = "./real_input.txt";
  // char *file_path = "./test_input_1.txt";
  char *buffer;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);
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

    size_t cnt_digits_time = count_digits(time);
    size_t cnt_digits_dist = count_digits(dist);

    size_t factor;
    factor = 1;
    while (cnt_digits_time--) factor *= 10;
    corrected_time = corrected_time * factor + time;

    factor = 1;
    while (cnt_digits_dist--) factor *= 10;
    corrected_dist = corrected_dist * factor + dist;
  }

  printf("Answer to part 1 = %zu\n", product);

  size_t num_victories = 0;
  for (size_t test_time=0; test_time<corrected_time; test_time++) {
    if ((corrected_time - test_time) * test_time > corrected_dist) num_victories++;
  }
  printf("Answer to part 2 = %zu\n", num_victories);

  return 0;
}

