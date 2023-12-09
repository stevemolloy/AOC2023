#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "aoc_lib.h"

int get_next_val_in_sequence(int *val_array, size_t N) {
  int result = 0;
  int *diff_array = calloc(N-1, sizeof(int));
  bool all_zeros = true;
  for (size_t i=1; i<N; i++) {
    diff_array[i-1] = val_array[i] - val_array[i-1];
    if (diff_array[i-1] != 0) all_zeros = false;
  }

  if (all_zeros) result = 0;
  else result = get_next_val_in_sequence(diff_array, N-1);
  free(diff_array);
  return result + val_array[N-1];
}

int get_first_val_in_sequence(int *val_array, size_t N) {
  int result = 0;
  int *diff_array = calloc(N-1, sizeof(int));
  bool all_zeros = true;
  for (size_t i=1; i<N; i++) {
    diff_array[i-1] = val_array[i] - val_array[i-1];
    if (diff_array[i-1] != 0) all_zeros = false;
  }

  if (all_zeros) result = 0;
  else result = get_first_val_in_sequence(diff_array, N-1);
  free(diff_array);
  return val_array[0] - result;
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  int total_part1 = 0;
  int total_part2 = 0;
  for (size_t i=0; i<num_lines; i++) {
    char *cursor = lines[i];
    size_t num_numbers = 0;
    while (*cursor) {
      if (*cursor==' ') num_numbers++;
      cursor++;
    }
    num_numbers++;
    
    int *val_array = calloc(num_numbers, sizeof(int));

    cursor = lines[i];
    for (size_t num=0; num<num_numbers; num++) {
      advance_past_chars(&cursor, " ");
      val_array[num] = get_next_int_from_string(&cursor);
    }

    int next = get_next_val_in_sequence(val_array, num_numbers);
    int first = get_first_val_in_sequence(val_array, num_numbers);
    total_part1 += next;
    total_part2 += first;

    free(val_array);
  }
  
  printf("Answer to part 1 = %d\n", total_part1);
  printf("Answer to part 2 = %d\n", total_part2);

  free(buffer);
  free(lines);

  return 0;
}

