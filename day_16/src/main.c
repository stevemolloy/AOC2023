#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

const size_t RIGHT = 1 << 0;
const size_t LEFT  = 1 << 1;
const size_t DOWN  = 1 << 2;
const size_t UP    = 1 << 3;

char get_char_at_point(size_t x, size_t y, char**grid) {
  return grid[y][x];
}

void solve_path(int X, int Y, size_t incoming_dir, char **mirror_grid, size_t *laser_grid, size_t width, size_t height) {
  if ((laser_grid[Y*width + X] & incoming_dir) > 0) return;  // We have already tracked this beam, so no need to follow it again
  if (X<0 || (size_t)X==width || Y<0 || (size_t)Y==height) return;
  laser_grid[Y*width + X] |= incoming_dir;    // OR the laser_grid with the incoming beam to mark it
  char object = get_char_at_point(X, Y, mirror_grid);  // Get the mirror/splitter/nothing that is at this point in the grid
  size_t outgoing_dir;
  // Set the outgoing_dir based on the object we found and the incoming_dir
  switch (object) {
    case '.':
      outgoing_dir = incoming_dir;
      break;
    case '/':
      if      (incoming_dir == RIGHT) outgoing_dir = UP;
      else if (incoming_dir == LEFT)  outgoing_dir = DOWN;
      else if (incoming_dir == DOWN)  outgoing_dir = LEFT;
      else if (incoming_dir == UP)    outgoing_dir = RIGHT;
      else assert(0 && "Unreachable");
      break;
    case '\\':
      if      (incoming_dir == RIGHT) outgoing_dir = DOWN;
      else if (incoming_dir == LEFT)  outgoing_dir = UP;
      else if (incoming_dir == DOWN)  outgoing_dir = RIGHT;
      else if (incoming_dir == UP)    outgoing_dir = LEFT;
      else assert(0 && "Unreachable");
      break;
    case '-':
      if      (incoming_dir == RIGHT) outgoing_dir = RIGHT;
      else if (incoming_dir == LEFT)  outgoing_dir = LEFT;
      else if (incoming_dir == DOWN)  outgoing_dir = LEFT | RIGHT;
      else if (incoming_dir == UP)    outgoing_dir = LEFT | RIGHT;
      else assert(0 && "Unreachable");
      break;
    case '|':
      if      (incoming_dir == RIGHT) outgoing_dir = UP | DOWN;
      else if (incoming_dir == LEFT)  outgoing_dir = UP | DOWN;
      else if (incoming_dir == DOWN)  outgoing_dir = DOWN;
      else if (incoming_dir == UP)    outgoing_dir = UP;
      else assert(0 && "Unreachable");
      break;
  }
  // Now that we know the outgoing_dir, recurse into the next step
  // Note the bounds checks that cause an early return
  if ((outgoing_dir & RIGHT) > 0) {
    solve_path(X+1, Y, RIGHT, mirror_grid, laser_grid, width, height);
  }
  if ((outgoing_dir & LEFT) > 0) {
    solve_path(X-1, Y, LEFT, mirror_grid, laser_grid, width, height);
  }
  if ((outgoing_dir & DOWN) > 0) {
    solve_path(X, Y+1, DOWN, mirror_grid, laser_grid, width, height);
  }
  if ((outgoing_dir & UP) > 0) {
    solve_path(X, Y-1, UP, mirror_grid, laser_grid, width, height);
  }
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  // char *file_path = "./test1.txt";
  char *buffer;
  char **lines;

  size_t num_rows = read_entire_file_to_lines(file_path, &buffer, &lines);
  size_t num_cols = strlen(lines[0]);

  size_t *laser_grid = calloc(num_cols*num_rows, sizeof(size_t));
  solve_path(0, 0, RIGHT, lines, laser_grid, num_cols, num_rows);
  printf("\n");

  size_t total = 0;
  for (size_t y=0; y<num_rows; y++) {
    for (size_t x=0; x<num_cols; x++) {
      if (laser_grid[y*num_cols + x] > 0) total++;
      if (lines[y][x] != '.') {
        putchar(lines[y][x]);
      } else if (laser_grid[y*num_cols + x] > 0) {
        putchar('#');
      }
      else putchar('.');
    }
    printf("\n");
  }

  // 8012 is too low
  printf("Answer to part 1 = %zu\n", total);

  return 0;
}
