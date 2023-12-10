#include "aoc_lib.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  STATIC,
  FROM_SOUTH,
  FROM_NORTH,
  FROM_WEST,
  FROM_EAST,
} LastMove;

typedef struct {
  size_t x, y;
  LastMove lm;
} Pos;

char get_char_at_pos(Pos p, char **lines) {
  return lines[p.y][p.x];
}

int find_first_step(Pos *loc, char **lines, size_t grid_height, size_t grid_width) {
  if (loc->x > 0) {
    Pos test_loc = {loc->x-1, loc->y, FROM_EAST};
    char c = get_char_at_pos(test_loc, lines);
    if (c == '-' || c == 'L' || c == 'F') {
      *loc = test_loc;
      return 1;
    }
  }
  if (loc->x < grid_width-1) {
    Pos test_loc = {loc->x+1, loc->y, FROM_WEST};
    char c = get_char_at_pos(test_loc, lines);
    if (c == '-' || c == 'L' || c == 'J') {
      *loc = test_loc;
      return 1;
    }
  }
  if (loc->y > 0) {
    Pos test_loc = {loc->x, loc->y-1, FROM_SOUTH};
    char c = get_char_at_pos(test_loc, lines);
    if (c == '|' || c == 'L' || c == 'F') {
      *loc = test_loc;
      return 1;
    }
  }
  if (loc->y < grid_height-1) {
    Pos test_loc = {loc->x, loc->y+1, FROM_NORTH};
    char c = get_char_at_pos(test_loc, lines);
    if (c == '|' || c == '7' || c == 'F') {
      *loc = test_loc;
      return 1;
    }
  }
  return -1;
}

int take_next_step(Pos *loc, char **lines, size_t grid_height, size_t grid_width) {
  char c = get_char_at_pos(*loc, lines);
  if (c=='.') return -1;

  switch (loc->lm) {
    case STATIC:
      fprintf(stderr, "STATIC appearance impossible\n");
      exit(1);
    case FROM_NORTH:
      if (c== '-' || c=='7' || c=='F') return -1;
      else if (c=='|') {
        if (loc->y==grid_height-1) return -1;
        loc->y++;
        loc->lm = FROM_NORTH;
        return 1;
      } else if (c=='L') {
        if (loc->x==grid_width-1) return -1;
        loc->x++;
        loc->lm = FROM_WEST;
        return 1;
      } else if (c=='J') {
        if (loc->x==0) return -1;
        loc->x--;
        loc->lm = FROM_EAST;
        return 1;
      } else {
        assert(0 && "Unreachable");
      }
    case FROM_SOUTH:
      if (c== '-' || c=='L' || c=='J') return -1;
      else if (c=='|') {
        if (loc->y==0) return -1;
        loc->y--;
        loc->lm = FROM_SOUTH;
        return 1;
      } else if (c=='7') {
        if (loc->x==0) return -1;
        loc->x--;
        loc->lm = FROM_EAST;
        return 1;
      } else if (c=='F') {
        if (loc->x==grid_width-1) return -1;
        loc->x++;
        loc->lm = FROM_WEST;
        return 1;
      } else {
        assert(0 && "Unreachable");
      }
    case FROM_EAST:
      if (c== '|' || c=='J' || c=='7') return -1;
      else if (c=='-') {
        if (loc->x==0) return -1;
        loc->x--;
        loc->lm = FROM_EAST;
        return 1;
      } else if (c=='L') {
        if (loc->y==0) return -1;
        loc->y--;
        loc->lm = FROM_SOUTH;
        return 1;
      } else if (c=='F') {
        if (loc->y==grid_height-1) return -1;
        loc->y++;
        loc->lm = FROM_NORTH;
        return 1;
      } else {
        assert(0 && "Unreachable");
      }
    case FROM_WEST:
      if (c== '|' || c=='L' || c=='F') return -1;
      else if (c=='-') {
        if (loc->x==grid_width-1) return -1;
        loc->x++;
        loc->lm = FROM_WEST;
        return 1;
      } else if (c=='J') {
        if (loc->y==0) return -1;
        loc->y--;
        loc->lm = FROM_SOUTH;
        return 1;
      } else if (c=='7') {
        if (loc->y==grid_height-1) return -1;
        loc->y++;
        loc->lm = FROM_NORTH;
        return 1;
      } else {
        assert(0 && "Unreachable");
      }
  }

  return -1;
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;
  size_t grid_width = read_entire_file_to_lines(file_path, &buffer, &lines);
  size_t grid_height = strlen(lines[0]);

  printf("\n");
  printf("Grid:\n");
  Pos loc = {0};
  for (size_t j=0; j<grid_height; j++) {
    for (size_t i=0; i<grid_width; i++) {
      loc.x = i;
      loc.y = j;
      putc(get_char_at_pos(loc, lines), stdout);
    }
    putc('\n', stdout);
  }

  printf("\n===================\n\n");

  for (size_t j=0; j<grid_height; j++) {
    for (size_t i=0; i<grid_width; i++) {
      if (lines[j][i] == 'S') {
        loc.x = i;
        loc.y = j;
        break;
      }
    }
  }

  size_t steps = 0;
  printf("Starting loc = %zu, %zu\n", loc.x, loc.y);
  find_first_step(&loc, lines, grid_height, grid_width);
  steps++;
  printf("New loc      = %zu, %zu\n", loc.x, loc.y);
  while (take_next_step(&loc, lines, grid_height, grid_width) > 0) {
    steps++;
    if (get_char_at_pos(loc, lines) == 'S') {
      printf("Completed the loop in %zu steps!!\n", steps);
      break;
    }
    printf("New loc      = %zu, %zu\n", loc.x, loc.y);
  }

  printf("Answer to part 1 = %zu\n", steps/2);

  return 0;
}

