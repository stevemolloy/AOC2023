#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "aoc_lib.h"

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

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide a single argument -- the file to be analysed.\n");
    return 1;
  }
  char *buffer;
  char **lines;

  size_t num_lines = read_entire_file_to_lines(argv[1], &buffer, &lines);

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

  size_t ans_1 = 0;
  size_t ans_2 = 0;
  for (size_t gridN=0; gridN<num_grids; gridN++) {
    int start = grid_starts[gridN];
    int end   = grid_ends[gridN];

    int line_length = strlen(lines[start]);

    for (int h_loc=1; h_loc<=end-start-1; h_loc++) {
      int err_count = horizonal_reflection_error_count(lines, start, end, h_loc);
      if (err_count == 0) ans_1 += 100 * h_loc;
      if (err_count == 1) ans_2 += 100 * h_loc;
    }
    for (int v_loc=1; v_loc<=line_length-1; v_loc++) {
      int err_count = vertical_reflection_error_count(lines, start, end, v_loc);
      if (err_count == 0) ans_1 += v_loc;
      if (err_count == 1) ans_2 += v_loc;
    }
  }

  printf("Answer to part 1 = %zu\n", ans_1);
  printf("Answer to part 2 = %zu\n", ans_2);

  free(grid_starts);
  free(grid_ends);
  free(buffer);
  free(lines);

  return 0;
}
