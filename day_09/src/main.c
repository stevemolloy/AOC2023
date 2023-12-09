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

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  int total = 0;
  for (size_t i=0; i<num_lines; i++) {
    size_t num_numbers = 0;
    char *cursor = lines[i];
    while (*cursor) {
      if (*cursor==' ') num_numbers++;
      cursor++;
    }
    num_numbers++;
    
    int *val_array = calloc(num_numbers, sizeof(int));
    cursor = lines[i];
    for (size_t num=0; num<num_numbers; num++) {
      int sign = 1;
      advance_past_chars(&cursor, " ");
      if (*cursor == '-') {
        sign = -1;
        cursor++;
      }
      val_array[num] = sign * (int)get_next_val_from_string(&cursor);
    }

    int next = get_next_val_in_sequence(val_array, num_numbers);
    total += next;

    free(val_array);
  }
  
  printf("Answer to part 1 = %d\n", total);

  return 0;
}

