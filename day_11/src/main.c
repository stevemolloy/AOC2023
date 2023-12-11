#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef struct {
  size_t x,y;
} Pos;

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
  char *file_path = "./test_input.txt";
  char *buffer;
  char **lines;
  size_t num_rows = read_entire_file_to_lines(file_path, &buffer, &lines);
  size_t num_cols = strlen(lines[0]);

  for (size_t i=0; i<num_rows; i++) {
    printf("line %03zu: %s\n", i, lines[i]);
  }

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
      y++;
      if (!is_symbol_in_col(lines, num_rows, i, '#')) y++;
    }
    x++;
    if (!is_symbol_in_row(lines, j, '#')) x++;
  }

  for (size_t i=0; i<num_galaxies; i++) {
    printf("Galaxy %03zu: %zu,%zu\n", i, galaxy_pos_list[i].x, galaxy_pos_list[i].y);
  }

  free(buffer);
  free(lines);

  return 0;
}

