#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;
  size_t num_rows = read_entire_file_to_lines(file_path, &buffer, &lines);
  size_t num_cols = strlen(lines[0]);

  size_t num_galaxies = 0;
  for (size_t j=0; j<num_rows; j++) {
    for (size_t i=0; i<num_cols; i++) {
      if (lines[j][i] == '#') num_galaxies++;
    }
  }

  printf("Galaxies found: %zu\n", num_galaxies);

  Pos *galaxy_pos_list = calloc(num_galaxies, sizeof(Pos));

  size_t x=0, y=0;
  size_t galaxy_index = 0;
  for (size_t j=0; j<num_rows; j++) {
    y = 0;
    for (size_t i=0; i<num_cols; i++) {
      if (lines[j][i] == '#') {
        galaxy_pos_list[galaxy_index++] = (Pos) { x, y };
      }
      y += 1;
      if (!is_symbol_in_col(lines, num_rows, i, '#')) y++;
    }
    x += 1;
    if (!is_symbol_in_row(lines, j, '#')) x++;
  }

  size_t *distances = calloc(triangular_number(num_galaxies), sizeof(size_t));

  get_all_distances(galaxy_pos_list, num_galaxies, distances);

  size_t sum;
  sum = sum_vector(distances, triangular_number(num_galaxies));
  printf("Answer to part 1 = %zu\n", sum);

  x=0, y=0;
  galaxy_index = 0;
  for (size_t j=0; j<num_rows; j++) {
    y = 0;
    for (size_t i=0; i<num_cols; i++) {
      if (lines[j][i] == '#') {
        galaxy_pos_list[galaxy_index++] = (Pos) { x, y };
      }
      y += 1;
      if (!is_symbol_in_col(lines, num_rows, i, '#')) y += 1000*1000 - 1;
    }
    x += 1;
    if (!is_symbol_in_row(lines, j, '#')) x += 1000*1000 - 1;
  }

  get_all_distances(galaxy_pos_list, num_galaxies, distances);

  sum = sum_vector(distances, triangular_number(num_galaxies));
  printf("Answer to part 1 = %zu\n", sum);

  free(galaxy_pos_list);
  free(distances);
  free(buffer);
  free(lines);

  return 0;
}

