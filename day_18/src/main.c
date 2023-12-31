#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "aoc_lib.h"

typedef struct {
  int64_t x, y;
} Point;

int64_t det(Point pt1, Point pt2) {
  return pt1.x * pt2.y - pt2.x *pt1.y;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide a single input -- the file to be parsed\n");
    return 1;
  }
  char *buffer;
  char **lines;

  size_t num_rows = read_entire_file_to_lines(argv[1], &buffer, &lines);
  Point *points = malloc(num_rows * sizeof(Point));

  {
    int64_t total_dist = 0;
    points[0] = (Point) {0, 0};
    for (size_t i=0; i<num_rows; i++) {
      char *line = lines[i];
      char command = *line;
      line++;
      advance_past_chars(&line, " ");
      size_t distance = get_next_int_from_string(&line);
      total_dist += distance;
      points[i] = points[i-1];
      if (command == 'R') points[i].x += distance;
      else if (command == 'L') points[i].x -= distance;
      else if (command == 'U') points[i].y -= distance;
      else if (command == 'D') points[i].y += distance;
    }

    int64_t total = 0;
    for (size_t i=1; i<num_rows; i++) {
      total += det(points[i-1], points[i]);
    }
    total += det(points[num_rows-1], points[0]);

    printf("Answer to part 1 = %ld (should be 34329)\n", (total/2) + (total_dist/2) + 1);
  }

  {
    int64_t total_dist = 0;
    points[0] = (Point) {0, 0};
    for (size_t i=0; i<num_rows; i++) {
      char *line = lines[i];
      advance_past_chars(&line, "LRUD 0123456789(");
      line++;

      size_t distance = 0;
      for (size_t n=0; n<5; n++) {
        if (isdigit(*line)) distance = distance*16 + *line - '0';
        else                distance = distance*16 + *line - 'a' + 10;
        line++;
      }
      total_dist += distance;

      char command;
      if (*line == '0') command = 'R';
      else if (*line == '1') command = 'D';
      else if (*line == '2') command = 'L';
      else if (*line == '3') command = 'U';
      else assert (0 && "Unreachable");
      line++;

      points[i] = points[i-1];
      if (command == 'R') points[i].x += distance;
      else if (command == 'L') points[i].x -= distance;
      else if (command == 'U') points[i].y -= distance;
      else if (command == 'D') points[i].y += distance;
    }

    int64_t total = 0;
    for (size_t i=1; i<num_rows; i++) total += det(points[i-1], points[i]);
    total += det(points[num_rows-1], points[0]);

    printf("Answer to part 2 = %ld (should be 42617947302920)\n", (total/2) + (total_dist/2) + 1);
  }

  free(points);
  free(lines);
  free(buffer);

  return 0;
}

