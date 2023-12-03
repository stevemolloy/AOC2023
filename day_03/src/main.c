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

int main(void) {
  // char *file_path = "./test_input_1.txt";
  char *file_path = "./real_input.txt";
  char *contents = read_entire_file(file_path);

  size_t cols = 0;
  size_t rows = 0;

  char *cursor = contents;
  while(*cursor) {
    if (*cursor=='\n') break;
    cols++;
    cursor++;
  }

  cursor = contents;
  while (*cursor) {
    if (*cursor == '\n') rows++;
    cursor++;
  }
  
  // printf("cols = %zu\n", cols);
  // printf("rows = %zu\n", rows);

  char *grid = calloc(cols*rows, sizeof(char));

  cursor = contents;
  for (size_t x=0; x<cols; x++) {
    for (size_t y=0; y<rows; y++) {
      if (*cursor == '\n') cursor++;
      grid[x*cols + y] = *cursor;
      cursor++;
    }
  }

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
								is_symbol(grid[(x-1)*rows + y+1]) |
								is_symbol(grid[(x+0)*rows + y+1]) |
								is_symbol(grid[(x+1)*rows + y+1]) |
								is_symbol(grid[(x-1)*rows + y  ]) |
								is_symbol(grid[(x+1)*rows + y  ]) |
								is_symbol(grid[(x-1)*rows + y-1]) |
								is_symbol(grid[(x+0)*rows + y-1]) |
								is_symbol(grid[(x+1)*rows + y-1]);
        } else if (x==0 && y>0 && y<rows-1) {
          engine_part = engine_part |
								is_symbol(grid[(x+0)*rows + y+1]) |
								is_symbol(grid[(x+1)*rows + y+1]) |
								is_symbol(grid[(x+1)*rows + y  ]) |
								is_symbol(grid[(x+0)*rows + y-1]) |
								is_symbol(grid[(x+1)*rows + y-1]);
        } else if (x==cols-1 && y>0 && y<rows-1) {
          engine_part = engine_part |
								is_symbol(grid[(x-1)*rows + y+1]) |
								is_symbol(grid[(x+0)*rows + y+1]) |
								is_symbol(grid[(x-1)*rows + y  ]) |
								is_symbol(grid[(x-1)*rows + y-1]) |
								is_symbol(grid[(x+0)*rows + y-1]);
        } else if (x>0 && y==0 && x<cols-1) {
          engine_part = engine_part |
								is_symbol(grid[(x-1)*rows + y+1]) |
								is_symbol(grid[(x+0)*rows + y+1]) |
								is_symbol(grid[(x+1)*rows + y+1]) |
								is_symbol(grid[(x-1)*rows + y  ]) |
								is_symbol(grid[(x+1)*rows + y  ]);
        } else if (x>0 && y==rows-1 && x<cols-1) {
          engine_part = engine_part |
								is_symbol(grid[(x-1)*rows + y  ]) |
								is_symbol(grid[(x+1)*rows + y  ]) |
								is_symbol(grid[(x-1)*rows + y-1]) |
								is_symbol(grid[(x+0)*rows + y-1]) |
								is_symbol(grid[(x+1)*rows + y-1]);
        }
      } else if (val>0) {
        // printf("val = %zu, engine_part = %zu\n", val, engine_part);
        total += val * engine_part;
        val = 0;
        engine_part = 0;
      }
    }
  }

  printf("Answer to part 1 = %zu\n", total);

  int *int_grid = calloc(cols*rows, sizeof(int));

  printf("Grid (before):\n%s\n", grid);
  size_t num_stars = 0;
  int marker = -1;
  for (size_t x=0; x<cols; x++) {
    for (size_t y=0; y<rows; y++) {
      char chr = grid[x*rows + y];
      if (is_symbol(chr) && chr != '*') {
        int_grid[x*rows + y] = (int)'.';
      } else if (chr == '*') {
        num_stars++;
        int_grid[x*rows + y] = marker--;
      } else if (chr == '.') {
        int_grid[x*rows + y] = (int)'.';
      } else if (isdigit(chr)) {
        int_grid[x*rows + y] = chr - '0';
      }
    }
  }

  for (size_t x=0; x<cols; x++) {
    for (size_t y=0; y<rows; y++) {
      printf("%d, ", int_grid[x*rows + y]);
    }
    printf("\n");
  }

  printf("num_stars = %zu\n", num_stars);

  size_t *gear_ratios = calloc(num_stars, sizeof(size_t));
  size_t *neighbours  = calloc(num_stars, sizeof(size_t));

  printf("Grid (after):\n%s\n", grid);

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
	  							(int_grid[(x-1)*rows + y+1] == marker) |
	  							(int_grid[(x+0)*rows + y+1] == marker) |
	  							(int_grid[(x+1)*rows + y+1] == marker) |
	  							(int_grid[(x-1)*rows + y  ] == marker) |
	  							(int_grid[(x+1)*rows + y  ] == marker) |
	  							(int_grid[(x-1)*rows + y-1] == marker) |
	  							(int_grid[(x+0)*rows + y-1] == marker) |
	  							(int_grid[(x+1)*rows + y-1] == marker);
          } else if (x==0 && y>0 && y<rows-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x+0)*rows + y+1] == marker) |
	  							(int_grid[(x+1)*rows + y+1] == marker) |
	  							(int_grid[(x+1)*rows + y  ] == marker) |
	  							(int_grid[(x+0)*rows + y-1] == marker) |
	  							(int_grid[(x+1)*rows + y-1] == marker);
          } else if (x==cols-1 && y>0 && y<rows-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x-1)*rows + y+1] == marker) |
	  							(int_grid[(x+0)*rows + y+1] == marker) |
	  							(int_grid[(x-1)*rows + y  ] == marker) |
	  							(int_grid[(x-1)*rows + y-1] == marker) |
	  							(int_grid[(x+0)*rows + y-1] == marker);
          } else if (x>0 && y==0 && x<cols-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x-1)*rows + y+1] == marker) |
	  							(int_grid[(x+0)*rows + y+1] == marker) |
	  							(int_grid[(x+1)*rows + y+1] == marker) |
	  							(int_grid[(x-1)*rows + y  ] == marker) |
	  							(int_grid[(x+1)*rows + y  ] == marker);
          } else if (x>0 && y==rows-1 && x<cols-1) {
            beside_marker = beside_marker |
	  							(int_grid[(x-1)*rows + y  ] == marker) |
	  							(int_grid[(x+1)*rows + y  ] == marker) |
	  							(int_grid[(x-1)*rows + y-1] == marker) |
	  							(int_grid[(x+0)*rows + y-1] == marker) |
	  							(int_grid[(x+1)*rows + y-1] == marker);
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
    int marker = -1 - star;
    printf("marker = %d: neighbours = %zu, gear_ratio = %zu\n", marker, neighbours[star], gear_ratios[star]);
    if (neighbours[star]==2) total += gear_ratios[star];
  }

  printf("Answer to part 2 = %zu\n", total);

  free(neighbours);
  free(gear_ratios);
  free(grid);

  return 0;
}

