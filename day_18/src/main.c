#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef struct {
  int x, y;
} Point;

int det(Point pt1, Point pt2) {
  return pt1.x * pt2.y - pt2.x *pt1.y;
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;

  size_t num_rows = read_entire_file_to_lines(file_path, &buffer, &lines);
  Point *points = malloc(num_rows * sizeof(Point));

  int total_dist = 0;
  points[0] = (Point) {0, 0};
  for (size_t i=0; i<num_rows; i++) {
    char *line = lines[i];
    char command = *line;
    line++;
    advance_past_chars(&line, " ");
    size_t distance = get_next_int_from_string(&line);
    total_dist += distance;
    printf("distance = %zu, total_distance = %d\n", distance, total_dist);
    points[i] = points[i-1];
    if (command == 'R') points[i].x += distance;
    else if (command == 'L') points[i].x -= distance;
    else if (command == 'U') points[i].y -= distance;
    else if (command == 'D') points[i].y += distance;
  }

  int total = 0;
  for (size_t i=1; i<num_rows; i++) {
    total += det(points[i-1], points[i]);
  }
  total += det(points[num_rows-1], points[0]);

  printf("Answer to part 1 = %d\n", (total/2) + (total_dist/2) + 1);

  return 0;
}
