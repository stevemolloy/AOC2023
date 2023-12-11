#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef struct {
  size_t x,y;
} Pos;

size_t dist_between_pos(Pos a, Pos b) {
  int x_dist = a.x - b.x;
  if (x_dist < 0) x_dist *= -1;
  int y_dist = a.y - b.y;
  if (y_dist < 0) y_dist *= -1;
  return x_dist + y_dist;
}

bool is_symbol_in_row(char **lines, size_t row_num, char c) {
  for (size_t i=0; i<strlen(lines[row_num]); i++) {
    if (lines[row_num][i] == c) return true;
  }
  return false;
}

bool is_symbol_in_col(char **lines, size_t num_lines, size_t col_num, char c) {
  for (size_t i=0; i<num_lines; i++) {
    if (lines[i][col_num] == c) return true;
  }
  return false;
}

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

  size_t *distances = calloc(num_galaxies*(num_galaxies+1)/2, sizeof(size_t));
  galaxy_index = 0;
  for (size_t i=0; i<num_galaxies; i++) {
    for (size_t j=i; j<num_galaxies; j++) {
      distances[galaxy_index++] = dist_between_pos(galaxy_pos_list[i], galaxy_pos_list[j]);
    }
  }

  size_t sum = 0;
  for (size_t i=0; i<num_galaxies*(num_galaxies+1)/2; i++) {
    sum += distances[i];
  }

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

  galaxy_index = 0;
  for (size_t i=0; i<num_galaxies; i++) {
    for (size_t j=i; j<num_galaxies; j++) {
      distances[galaxy_index++] = dist_between_pos(galaxy_pos_list[i], galaxy_pos_list[j]);
    }
  }

  sum = 0;
  for (size_t i=0; i<num_galaxies*(num_galaxies+1)/2; i++) {
    sum += distances[i];
  }

  printf("Answer to part 1 = %zu\n", sum);

  free(galaxy_pos_list);
  free(distances);
  free(buffer);
  free(lines);

  return 0;
}

