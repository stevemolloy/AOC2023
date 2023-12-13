#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "aoc_lib.h"

bool compare_two_lines(char *l1, char *l2) {
  assert(strlen(l1) == strlen(l2) && "Line lengths do not match");
  for (size_t i=0; i<strlen(l1); i++) {
    if (l1[i] != l2[i]) return false;
  }
  return true;
}

bool compare_two_columns(char **lines, size_t start_row, size_t end_row, size_t col1, size_t col2) {
  for (size_t i=start_row; i<end_row; i++) {
    if (lines[i][col1] != lines[i][col2]) return false;
  }
  return true;
}

int horizonal_reflection_error_count(char **lines, int start, int end, int mirror_loc) {
  int reflection_start = start + mirror_loc;
  int line_length = strlen(lines[reflection_start]);

  int error_count = 0;

  int s1 = reflection_start - 1;
  int s2 = reflection_start;

  while (s1>=start && s2<end) {
    for (int i=0; i<line_length; i++) {
      if (lines[s1][i] != lines[s2][i]) error_count++;
    }
    s1--;
    s2++;
  }

  return error_count;
}

int vertical_reflection_error_count(char **lines, int start, int end, int mirror_loc) {
  int reflection_start = mirror_loc - 1;
  int line_length = strlen(lines[start]);

  int error_count = 0;

  int s1 = reflection_start;
  int s2 = reflection_start + 1;

  while (s1>=0 && s2<line_length) {
    for (int i=start; i<end; i++) {
      if (lines[i][s1] != lines[i][s2]) error_count++;
    }
    s1--;
    s2++;
  }

  return error_count;
}

int main(void) {
  // char *file_path = "./my_test_input.txt";
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;

  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  size_t num_grids = 1;
  for (size_t i=0; i<num_lines; i++) {
    if (strlen(lines[i]) == 0) num_grids++;
  }
  size_t *grid_starts = calloc(num_grids, sizeof(size_t));
  size_t *grid_ends   = calloc(num_grids, sizeof(size_t));
  size_t ind = 0;
  grid_starts[ind] = 0;
  for (size_t i=0; i<num_lines; i++) {
    if (strlen(lines[i]) == 0) {
      grid_ends[ind] = i;
      ind++;
      grid_starts[ind] = i+1;
    }
  }
  grid_ends[ind] = num_lines;

  printf("I found %zu grids\n", num_grids);

  size_t ans_1 = 0;
  size_t ans_2 = 0;
  for (size_t gridN=0; gridN<num_grids; gridN++) {
    bool found_a_mirror = false;
    bool found_a_mirror2 = false;
    int start = grid_starts[gridN];
    int end   = grid_ends[gridN];

    int line_length = strlen(lines[start]);

    for (int h_loc=1; h_loc<=end-start-1; h_loc++) {
      int err_count = horizonal_reflection_error_count(lines, start, end, h_loc);
      printf("err_count = %d, h_loc = %d\n", err_count, h_loc);
      if (err_count == 0) {
        found_a_mirror = true;
        ans_1 += 100 * h_loc;
      }
      if (err_count == 1) {
        found_a_mirror2 = true;
        ans_2 += 100 * h_loc;
      }
    }
    for (int v_loc=1; v_loc<=line_length-1; v_loc++) {
      int err_count = vertical_reflection_error_count(lines, start, end, v_loc);
      printf("err_count = %d, v_loc = %d\n", err_count, v_loc);
      if (err_count == 0) {
        if (found_a_mirror) printf("Found a second reflection for grid %zu!\n", gridN);
        found_a_mirror = true;
        ans_1 += v_loc;
      }
      if (err_count == 1) {
        if (found_a_mirror2) printf("Found a second reflection for grid %zu!\n", gridN);
        found_a_mirror2 = true;
        ans_2 += v_loc;
      }
    }
    if (!found_a_mirror) {
      printf("Found no mirror for grid %zu\n", gridN);
      for (int i=start; i<end; i++) {
        printf("%s\n", lines[i]);
      }
    }
    printf("\n");
  }

  printf("Answer to part 1 = %zu (Should be 29846)\n", ans_1);
  printf("Answer to part 2 = %zu (24615 is wrong)\n", ans_2);

  return 0;
}
