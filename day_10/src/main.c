#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef enum {
  STATIC = 0,
  FROM_SOUTH,
  FROM_NORTH,
  FROM_WEST,
  FROM_EAST,
} LastMove;

typedef struct {
  size_t x, y;
  LastMove lm;
} Pos;

typedef struct {
  size_t cap;
  size_t length;
  Pos *positions;
} PosArray;

void add_to_posarray(PosArray *pa, Pos p) {
  if (pa->length == pa->cap) {
    pa->positions = realloc(pa->positions, 2*pa->cap*sizeof(Pos));
    pa->cap *= 2;
  }
  pa->positions[pa->length++] = p;
}

void free_posarray(PosArray *pa) {
  free(pa->positions);
  pa->cap = 0;
  pa->length = 0;
}

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

  // printf("\n");
  // printf("Grid:\n");
  Pos loc = {0};
  for (size_t j=0; j<grid_height; j++) {
    for (size_t i=0; i<grid_width; i++) {
      loc.x = i;
      loc.y = j;
      // putc(get_char_at_pos(loc, lines), stdout);
    }
    // putc('\n', stdout);
  }

  // printf("\n===================\n\n");

  PosArray loop = {
    .cap = 4096,
    .length = 0,
    .positions = calloc(4096, sizeof(Pos)),
  };

  for (size_t j=0; j<grid_height; j++) {
    for (size_t i=0; i<grid_width; i++) {
      if (lines[j][i] == 'S') {
        loc.x = i;
        loc.y = j;
        break;
      }
    }
  }

  add_to_posarray(&loop, loc);

  size_t steps = 0;
  // printf("Starting loc = %zu, %zu\n", loc.x, loc.y);
  find_first_step(&loc, lines, grid_height, grid_width);
  add_to_posarray(&loop, loc);
  steps++;
  printf("New loc        = %zu, %zu\n", loc.x, loc.y);
  printf("Character here = %c\n", lines[loc.y][loc.x]);
  while (take_next_step(&loc, lines, grid_height, grid_width) > 0) {
    add_to_posarray(&loop, loc);
    steps++;
    if (get_char_at_pos(loc, lines) == 'S') {
      printf("Completed the loop in %zu steps!!\n", steps);
      break;
    }
    // printf("New loc      = %zu, %zu\n", loc.x, loc.y);
  }

  printf("Answer to part 1 = %zu\n", steps/2);

  char *new_grid = calloc(grid_height*grid_width, sizeof(char));
  memset(new_grid, '.', grid_width*grid_height);

  for (size_t i=0; i<loop.length; i++) {
    size_t x = loop.positions[i].x;
    size_t y = loop.positions[i].y;
    new_grid[y*grid_width + x] = lines[y][x];
  }

  for (size_t i=0; i<loop.length; i++) {
    size_t x = loop.positions[i].x;
    size_t y = loop.positions[i].y;
    char c = new_grid[y*grid_width + x];
    if (c == '|' && loop.positions[i].lm == FROM_SOUTH && x!=0             && new_grid[y*grid_width + x-1]=='.') {
      new_grid[y*grid_width + x-1] = '*';
    }
    if (c == 'F' && loop.positions[i].lm == FROM_SOUTH && x!=0             && new_grid[y*grid_width + x-1]=='.') {
      new_grid[y*grid_width + x-1] = '*';
    }
    if (c == '7' && loop.positions[i].lm == FROM_SOUTH && x!=0             && new_grid[y*grid_width + x-1]=='.') {
      new_grid[y*grid_width + x-1] = '*';
    }
    if (c == 'J' && loop.positions[i].lm == FROM_WEST  && x!=0             && new_grid[y*grid_width + x-1]=='.') {
      new_grid[y*grid_width + x-1] = '*';
    }
    if (c == 'L' && loop.positions[i].lm == FROM_EAST  && x!=0             && new_grid[y*grid_width + x-1]=='.') {
      new_grid[y*grid_width + x-1] = '*';
    }
    if (c == '|' && loop.positions[i].lm == FROM_NORTH && x!=grid_width-1  && new_grid[y*grid_width + x+1]=='.') {
      new_grid[y*grid_width + x+1] = '*';
    }
    if (c == 'F' && loop.positions[i].lm == FROM_EAST  && x!=grid_width-1  && new_grid[y*grid_width + x+1]=='.') {
      new_grid[y*grid_width + x+1] = '*';
    }
    if (c == '7' && loop.positions[i].lm == FROM_WEST  && x!=grid_width-1  && new_grid[y*grid_width + x+1]=='.') {
      new_grid[y*grid_width + x+1] = '*';
    }
    if (c == 'J' && loop.positions[i].lm == FROM_NORTH && x!=grid_width-1  && new_grid[y*grid_width + x+1]=='.') {
      new_grid[y*grid_width + x+1] = '*';
    }
    if (c == 'L' && loop.positions[i].lm == FROM_NORTH && x!=grid_width-1  && new_grid[y*grid_width + x+1]=='.') {
      new_grid[y*grid_width + x+1] = '*';
    }
    if (c == '-' && loop.positions[i].lm == FROM_EAST  && y!=grid_height-1 && new_grid[(y+1)*grid_width + x]=='.') {
      new_grid[(y+1)*grid_width + x] = '*';
    }
    if (c == '-' && loop.positions[i].lm == FROM_WEST  && y!=0             && new_grid[(y-1)*grid_width + x]=='.') {
      new_grid[(y-1)*grid_width + x] = '*';
    }
  }

  for (size_t y=0; y<grid_height; y++) {
    for (size_t x=0; x<grid_width; x++) {
      if (new_grid[y*grid_width + x] == '*') {
        if (new_grid[(y+1)*grid_width + x] == '.') new_grid[(y+1)*grid_width + x] = '*';
        if (new_grid[(y-1)*grid_width + x] == '.') new_grid[(y-1)*grid_width + x] = '*';
        if (new_grid[y*grid_width + x+1] == '.') new_grid[y*grid_width + x+1] = '*';
        if (new_grid[y*grid_width + x-1] == '.') new_grid[y*grid_width + x-1] = '*';
      }
    }
  }

  for (size_t y=0; y<grid_height; y++) {
    for (size_t x=0; x<grid_width; x++) {
      if (new_grid[y*grid_width + x] == '*') {
        if (new_grid[(y+1)*grid_width + x] == '.') new_grid[(y+1)*grid_width + x] = '*';
        if (new_grid[(y-1)*grid_width + x] == '.') new_grid[(y-1)*grid_width + x] = '*';
        if (new_grid[y*grid_width + x+1] == '.') new_grid[y*grid_width + x+1] = '*';
        if (new_grid[y*grid_width + x-1] == '.') new_grid[y*grid_width + x-1] = '*';
      }
    }
  }

  for (int y=0; y<(int)grid_height; y++) {
    for (int x=0; x<(int)grid_width; x++) {
      char ng = new_grid[y*grid_width + x];
      if (ng == '.') new_grid[y*grid_width + x] = ' ';
      if (ng=='|' || ng=='-' || ng=='J' || ng=='L' || ng=='F' || ng == '7' || ng == 'S') break;
    }
  }

  for (int y=0; y<(int)grid_height; y++) {
    for (int x=(int)grid_width-1; x>0; x--) {
      char ng = new_grid[y*grid_width + x];
      if (new_grid[y*grid_width + x] == '.') new_grid[y*grid_width + x] = ' ';
      if (ng=='|' || ng=='-' || ng=='J' || ng=='L' || ng=='F' || ng == '7' || ng == 'S') break;
    }
  }

  for (int x=0; x<(int)grid_width; x++) {
    for (int y=0; y<(int)grid_height; y++) {
      char ng = new_grid[y*grid_width + x];
      if (new_grid[y*grid_width + x] == '.') new_grid[y*grid_width + x] = ' ';
      if (ng=='|' || ng=='-' || ng=='J' || ng=='L' || ng=='F' || ng == '7' || ng == 'S') break;
    }
  }

  for (int x=0; x<(int)grid_width; x++) {
    for (int y=(int)grid_height-1; y>=0; y--) {
      char ng = new_grid[y*grid_width + x];
      if (new_grid[y*grid_width + x] == '.') new_grid[y*grid_width + x] = ' ';
      if (ng=='|' || ng=='-' || ng=='J' || ng=='L' || ng=='F' || ng == '7' || ng == 'S') break;
    }
  }

  size_t counted_dots = 0;
  for (size_t y=0; y<grid_height; y++) {
    for (size_t x=0; x<grid_width; x++) {
      if (new_grid[y*grid_width + x] == '*') {
        printf("*");
        counted_dots++;
      }
      else if (new_grid[y*grid_width + x] == '|') printf("║");
      else if (new_grid[y*grid_width + x] == '-') printf("═");
      else if (new_grid[y*grid_width + x] == 'J') printf("╝");
      else if (new_grid[y*grid_width + x] == 'L') printf("╚");
      else if (new_grid[y*grid_width + x] == 'F') printf("╔");
      else if (new_grid[y*grid_width + x] == '7') printf("╗");
      else if (new_grid[y*grid_width + x] == 'S') printf("║");
      else if (new_grid[y*grid_width + x] == '.') printf(".");
      else if (new_grid[y*grid_width + x] == ' ') printf(" ");
      else {
        printf("Unknown char: %c\n", new_grid[y*grid_width + x]);
        assert(0 && "Not sure what to do with this character\n");
      }
    }
    putc('\n', stdout);
  }

  printf("Answer to part 2 = %zu\n", counted_dots);

  free(buffer);
  free(lines);

  return 0;
}

