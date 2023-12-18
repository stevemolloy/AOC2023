#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef enum {
  NONE = 0,
  RIGHT,
  LEFT,
  UP,
  DOWN,
  FILLED,
} SquareType;

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;

  size_t num_rows = read_entire_file_to_lines(file_path, &buffer, &lines);

  int max_right = 0;
  int max_left  = 0;
  int max_up    = 0;
  int max_down  = 0;

  int horizontal_loc = 0;
  int vertical_loc = 0;
  for (size_t r=0; r<num_rows; r++) {
    char *line = lines[r];
    char command = *line;
    line++;
    if (command == 'R') {
      advance_past_chars(&line, " ");
      horizontal_loc += get_next_val_from_string(&line);
    }
    else if (command == 'L') {
      advance_past_chars(&line, " ");
      horizontal_loc -= get_next_val_from_string(&line);
    }
    else if (command == 'U') {
      advance_past_chars(&line, " ");
      vertical_loc -= get_next_val_from_string(&line);
    }
    else if (command == 'D') {
      advance_past_chars(&line, " ");
      vertical_loc += get_next_val_from_string(&line);
    }

   if (horizontal_loc > max_right) max_right = horizontal_loc;
   if (-horizontal_loc > max_left) max_left = -horizontal_loc;
   if (vertical_loc > max_down) max_down = vertical_loc;
   if (-vertical_loc > max_up) max_up = -vertical_loc;
  }

  size_t grid_rows = 2 * (max_down - max_up + 1);
  size_t grid_cols = 2 * (max_right - max_left + 1);

  SquareType *dig_grid = malloc(grid_rows*grid_cols*sizeof(SquareType));
  for (size_t j=0; j<grid_rows; j++) {
    for (size_t i=0; i<grid_cols; i++) {
      dig_grid[j*grid_cols + i] = NONE;
    }
  }

  size_t curr_x = max_left;
  size_t curr_y = max_up;

  for (size_t r=0; r<num_rows; r++) {
    char *line = lines[r];
    char command = *line;
    line++;
    if (command == 'R') {
      advance_past_chars(&line, " ");
      size_t next_val = get_next_val_from_string(&line);
      printf("Moving R by %zu\n", next_val);
      for (size_t i=0; i<next_val; i++) {
        curr_x++;
        dig_grid[curr_y*grid_cols + curr_x] = RIGHT;
      }
    }
    else if (command == 'L') {
      advance_past_chars(&line, " ");
      size_t next_val = get_next_val_from_string(&line);
      printf("Moving L by %zu\n", next_val);
      for (size_t i=0; i<next_val; i++) {
        curr_x--;
        dig_grid[curr_y*grid_cols + curr_x] = LEFT;
      }
    }
    else if (command == 'U') {
      advance_past_chars(&line, " ");
      size_t next_val = get_next_val_from_string(&line);
      printf("Moving U by %zu\n", next_val);
      for (size_t i=0; i<next_val; i++) {
        curr_y--;
        dig_grid[curr_y*grid_cols + curr_x] = UP;
      }
    }
    else if (command == 'D') {
      advance_past_chars(&line, " ");
      size_t next_val = get_next_val_from_string(&line);
      printf("Moving D by %zu\n", next_val);
      for (size_t i=0; i<next_val; i++) {
        curr_y++;
        dig_grid[curr_y*grid_cols + curr_x] = DOWN;
      }
    }
  }

  for (size_t j=1; j<grid_rows-1; j++) {
    for (size_t i=1; i<grid_cols-1; i++) {
      if (dig_grid[j*grid_cols + i] != NONE) continue;
      if (dig_grid[j*grid_cols + i-1] == UP || dig_grid[j*grid_cols + i+1] == DOWN) {
        dig_grid[j*grid_cols + i] = FILLED;
      }
      if (dig_grid[(j-1)*grid_cols + i] == RIGHT || dig_grid[(j+1)*grid_cols + i] == LEFT) {
        dig_grid[j*grid_cols + i] = FILLED;
      }
    }
  }

  for (size_t j=1; j<grid_rows-1; j++) {
    for (size_t i=1; i<grid_cols-1; i++) {
      if (dig_grid[j*grid_cols + i] == NONE && dig_grid[j*grid_cols + i-1] == FILLED) {
        dig_grid[j*grid_cols + i] = FILLED;
      }
      if (dig_grid[j*grid_cols + i] == NONE && dig_grid[(j-1)*grid_cols + i] == FILLED) {
        dig_grid[j*grid_cols + i] = FILLED;
      }
    }
  }

  size_t path_length = 0;
  for (size_t j=0; j<grid_rows; j++) {
    for (size_t i=0; i<grid_cols; i++) {
      switch (dig_grid[j*grid_cols + i]) {
        case RIGHT:
          putc('>', stdout);
          break;
        case LEFT:
          putc('<', stdout);
          break;
        case UP:
          putc('^', stdout);
          break;
        case DOWN:
          putc('v', stdout);
          break;
        case FILLED:
          putc('#', stdout);
          break;
        case NONE:
          putc('.', stdout);
      }
      if (dig_grid[j*grid_cols + i] != NONE) path_length++;
    }
    printf("\n");
  }

  printf("Answer to part 1 = %zu\n", path_length);

  return 0;
}
