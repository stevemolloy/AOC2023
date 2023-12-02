#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

void get_game_stats(char *cursor, size_t *game_total, size_t *power) {
  while (*cursor) {
    if (strncmp(cursor, "Game ", 5) != 0) {
      fprintf(stderr, "Problem parsing file\n");
      exit(1);
    }
    cursor += 5;

    size_t game_num = 0;
    while (*cursor != ':') {
      game_num = game_num*10 + *cursor - '0';
      cursor++;
    }

    cursor += 2;
    while (*cursor == ' ') cursor++;

    size_t max_blue = 0, max_red = 0, max_green = 0;
    while (*cursor != '\n') {
      size_t ball_count = 0;
      while (*cursor != ' ') {
        ball_count = ball_count*10 + *cursor - '0';
        cursor++;
      }
      cursor++;

      if (strncmp(cursor, "blue", 4)==0) {
        max_blue = ball_count>max_blue ? ball_count : max_blue;
        cursor += 4;
      } else if (strncmp(cursor, "red", 3)==0) {
        max_red = ball_count>max_red ? ball_count : max_red;
        cursor += 3;
      } else if (strncmp(cursor, "green", 5)==0) {
        max_green = ball_count>max_green ? ball_count : max_green;
        cursor += 5;
      }

      while (*cursor == ' ' || *cursor == ';' || *cursor == ',') cursor++;
    }
    cursor++;

    if (max_blue <= 14 && max_red <= 12 && max_green <= 13) *game_total += game_num;
    *power += max_blue * max_red * max_green;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide a single input. The file to be parsed.\n");
    exit(1);
  }
  char *file_path = argv[1];
  char *contents = read_entire_file(file_path);

  size_t game_total = 0;
  size_t power = 0;
  
  get_game_stats(contents, &game_total, &power);

  printf("Answer to part 1 = %zu\n", game_total);
  printf("Answer to part 2 = %zu\n", power);

  free(contents);

  return 0;
}

