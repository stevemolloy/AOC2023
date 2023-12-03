#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#include "aoc_lib.h"

int is_symbol(char chr) {
  if (chr=='.' || isdigit(chr)) return 0;
  return 1;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Provide a single argument -- the path to the test file.\n");
    return 1;
  }

  char *file_path = argv[1];
  char *contents = read_entire_file(file_path);
  char *cursor = contents;

  size_t cols = 0, rows = 0;
  get_dims_of_char_matrix(cursor, &cols, &rows);

  char *grid = calloc(cols*rows, sizeof(char));
  if (grid == NULL) {
    fprintf(stderr, "Could not allocate memory. Buy more RAM I guess?\n");
    return 1;
  }
  cursor = contents;
  make_data_grid_from_string(cursor, cols, rows, grid);

  cursor = contents;
  size_t total = 0;
  size_t engine_part = 0;
  size_t val = 0;
  for (size_t x=0; x<cols; x++) {
    for (size_t y=0; y<rows; y++) {
      char chr = grid[x*cols + y];
      if (isdigit(chr)) {
        val = val*10 + chr - '0';
        if (x>0 && y>0 && x<cols-1 && y<rows-1) {
          engine_part = engine_part |
								is_symbol(grid[(x-1)*rows + y+1]) | is_symbol(grid[(x+0)*rows + y+1]) | is_symbol(grid[(x+1)*rows + y+1]) |
								is_symbol(grid[(x-1)*rows + y  ]) | is_symbol(grid[(x+1)*rows + y  ]) |
								is_symbol(grid[(x-1)*rows + y-1]) | is_symbol(grid[(x+0)*rows + y-1]) | is_symbol(grid[(x+1)*rows + y-1]);
        } else if (x==0 && y>0 && y<rows-1) {
          engine_part = engine_part |
								is_symbol(grid[(x+0)*rows + y+1]) | is_symbol(grid[(x+1)*rows + y+1]) |
								is_symbol(grid[(x+1)*rows + y  ]) |
								is_symbol(grid[(x+0)*rows + y-1]) | is_symbol(grid[(x+1)*rows + y-1]);
        } else if (x==cols-1 && y>0 && y<rows-1) {
          engine_part = engine_part |
								is_symbol(grid[(x-1)*rows + y+1]) | is_symbol(grid[(x+0)*rows + y+1]) |
								is_symbol(grid[(x-1)*rows + y  ]) |
								is_symbol(grid[(x-1)*rows + y-1]) | is_symbol(grid[(x+0)*rows + y-1]);
        } else if (x>0 && y==0 && x<cols-1) {
          engine_part = engine_part |
								is_symbol(grid[(x-1)*rows + y+1]) | is_symbol(grid[(x+0)*rows + y+1]) | is_symbol(grid[(x+1)*rows + y+1]) |
								is_symbol(grid[(x-1)*rows + y  ]) | is_symbol(grid[(x+1)*rows + y  ]);
        } else if (x>0 && y==rows-1 && x<cols-1) {
          engine_part = engine_part |
								is_symbol(grid[(x-1)*rows + y  ]) | is_symbol(grid[(x+1)*rows + y  ]) |
								is_symbol(grid[(x-1)*rows + y-1]) | is_symbol(grid[(x+0)*rows + y-1]) | is_symbol(grid[(x+1)*rows + y-1]);
        }
      } else if (val>0) {
        total += val * engine_part;
        val = 0;
        engine_part = 0;
      }
    }
  }

  printf("Answer to part 1 = %zu\n", total);

  // Count the number of stars
  size_t num_stars = 0;
  for (size_t x=0; x<cols; x++) {
    for (size_t y=0; y<rows; y++) {
      if (grid[x*rows + y] == '*') num_stars++;
    }
  }

  // Convert the grid to a grid of signed integers
  // Symbols, including '.', will be given their ascii values. Digits will be given
  // their numerical value. Asterisks will be given a negative value decrementing from -1
  int *int_grid = calloc(cols*rows, sizeof(int));
  if (int_grid == NULL) {
    fprintf(stderr, "Could not allocate memory. Buy more RAM I guess?\n");
    return 1;
  }
  int marker = -1;
  for (size_t x=0; x<cols; x++) {
    for (size_t y=0; y<rows; y++) {
      char chr = grid[x*rows + y];
      if (is_symbol(chr) && chr != '*') int_grid[x*rows + y] = (int)'.';
      else if (chr == '*')              int_grid[x*rows + y] = marker--;
      else if (chr == '.')              int_grid[x*rows + y] = (int)'.';
      else if (isdigit(chr))            int_grid[x*rows + y] = chr - '0';
    }
  }

  size_t *gear_ratios = calloc(num_stars, sizeof(size_t));
  if (gear_ratios == NULL) {
    fprintf(stderr, "Could not allocate memory. Buy more RAM I guess?\n");
    return 1;
  }
  size_t *neighbours  = calloc(num_stars, sizeof(size_t));
  if (neighbours == NULL) {
    fprintf(stderr, "Could not allocate memory. Buy more RAM I guess?\n");
    return 1;
  }

  for (size_t star=0; star<num_stars; star++) {
    gear_ratios[star] = 1;
    neighbours[star] = 0;
    int marker = -1 - star;
    size_t beside_marker = 0;
    for (size_t x=0; x<cols; x++) {
      for (size_t y=0; y<rows; y++) {
        char chr = grid[x*cols + y];
        if (isdigit(chr)) {
          val = val*10 + chr - '0';
          if (x>0 && y>0 && x<cols-1 && y<rows-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x-1)*rows + y+1] == marker) | (int_grid[(x+0)*rows + y+1] == marker) | (int_grid[(x+1)*rows + y+1] == marker) |
	  							(int_grid[(x-1)*rows + y  ] == marker) | (int_grid[(x+1)*rows + y  ] == marker) |
	  							(int_grid[(x-1)*rows + y-1] == marker) | (int_grid[(x+0)*rows + y-1] == marker) | (int_grid[(x+1)*rows + y-1] == marker);
          } else if (x==0 && y>0 && y<rows-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x+0)*rows + y+1] == marker) | (int_grid[(x+1)*rows + y+1] == marker) |
	  							(int_grid[(x+1)*rows + y  ] == marker) |
	  							(int_grid[(x+0)*rows + y-1] == marker) | (int_grid[(x+1)*rows + y-1] == marker);
          } else if (x==cols-1 && y>0 && y<rows-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x-1)*rows + y+1] == marker) | (int_grid[(x+0)*rows + y+1] == marker) |
	  							(int_grid[(x-1)*rows + y  ] == marker) |
	  							(int_grid[(x-1)*rows + y-1] == marker) | (int_grid[(x+0)*rows + y-1] == marker);
          } else if (x>0 && y==0 && x<cols-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x-1)*rows + y+1] == marker) | (int_grid[(x+0)*rows + y+1] == marker) | (int_grid[(x+1)*rows + y+1] == marker) |
	  							(int_grid[(x-1)*rows + y  ] == marker) | (int_grid[(x+1)*rows + y  ] == marker);
          } else if (x>0 && y==rows-1 && x<cols-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x-1)*rows + y  ] == marker) | (int_grid[(x+1)*rows + y  ] == marker) |
	  							(int_grid[(x-1)*rows + y-1] == marker) | (int_grid[(x+0)*rows + y-1] == marker) | (int_grid[(x+1)*rows + y-1] == marker);
          }
        } else if (val>0) {
          if (beside_marker) {
            neighbours[star]++;
            gear_ratios[star] *= val;
            beside_marker = 0;
          }
          val = 0;
        }
      }
    }
  }
  
  total = 0;
  for (size_t star=0; star<num_stars; star++) {
    if (neighbours[star]==2) total += gear_ratios[star];
  }

  printf("Answer to part 2 = %zu\n", total);

  free(neighbours);
  free(gear_ratios);
  free(int_grid);
  free(grid);
  free(contents);

  return 0;
}

