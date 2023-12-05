#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

#define MAPS_COUNT 7
const char *map_names[] = {
  "seed-to-soil",
  "soil-to-fertilizer",
  "fertilizer-to-water",
  "water-to-light",
  "light-to-temperature",
  "temperature-to-humidity",
  "humidity-to-location",
};

bool does_seed_exist(size_t seed_num, char* line) {
  char *seed_cursor = line;
  advance_to_char(&seed_cursor, ':');
  advance_past_chars(&seed_cursor, ": ");

  while (*seed_cursor) {
    size_t seed_start = get_next_val_from_string(&seed_cursor);
    seed_cursor++;
    size_t seed_length = get_next_val_from_string(&seed_cursor);
    seed_cursor++;
    if (seed_num>=seed_start && seed_num<(seed_start+seed_length)) return true;
  }
  return false;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide a single argument -- the file to be parsed.\n");
    return 1;
  }
  char *contents;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(argv[1], &contents, &lines);

  size_t map_lines[MAPS_COUNT];
  for (size_t line=0; line<num_lines; line++) {
    for (size_t map=0; map<MAPS_COUNT; map++) {
      if (strncmp(lines[line], map_names[map], strlen(map_names[map]))==0) map_lines[map] = line;
    }
  }

  char *seed_cursor = lines[0];
  advance_to_char(&seed_cursor, ':');
  advance_past_chars(&seed_cursor, ": ");

  size_t lowest = SIZE_MAX;

  while (*seed_cursor) {
    size_t seed = get_next_val_from_string(&seed_cursor);
    seed_cursor++;

    size_t key = seed;
    for (size_t map=0; map<MAPS_COUNT; map++) {
      size_t end_of_map = (map<MAPS_COUNT-1) ? map_lines[map+1] - 1 : num_lines;
      for (size_t i=map_lines[map]+1; i<end_of_map; i++) {
        char *cursor = lines[i];
        size_t dst_range_start = get_next_val_from_string(&cursor);
        cursor++;
        size_t src_range_start = get_next_val_from_string(&cursor);
        cursor++;
        size_t range_length = get_next_val_from_string(&cursor);

        if (key>=src_range_start && key<(src_range_start+range_length)) {
          key = dst_range_start + (key - src_range_start);
          break;
        }
      }
    }
    if (key < lowest) lowest = key;
  }

  printf("Answer to part 1: %zu\n", lowest);

  seed_cursor = lines[0];
  advance_to_char(&seed_cursor, ':');
  advance_past_chars(&seed_cursor, ": ");

  size_t location;
  size_t step_size = 100000;
  for (location = 0; ; location += step_size) {
    size_t key = location;
    for (int map=MAPS_COUNT-1; map>=0; map--) {
      size_t end_of_map = (map<MAPS_COUNT-1) ? map_lines[map+1] - 1 : num_lines;
      for (size_t i=map_lines[map]+1; i<end_of_map; i++) {
        char *cursor = lines[i];
        size_t dst_range_start = get_next_val_from_string(&cursor);
        cursor++;
        size_t src_range_start = get_next_val_from_string(&cursor);
        cursor++;
        size_t range_length = get_next_val_from_string(&cursor);

        if (key>=dst_range_start && key<(dst_range_start+range_length)) {
          key = src_range_start + (key - dst_range_start);
          break;
        }
      }
    }
    if (does_seed_exist(key, lines[0])) {
      break;
    }
  }

  location -= step_size;
  for (; ; location++) {
    size_t key = location;
    for (int map=MAPS_COUNT-1; map>=0; map--) {
      size_t end_of_map = (map<MAPS_COUNT-1) ? map_lines[map+1] - 1 : num_lines;
      for (size_t i=map_lines[map]+1; i<end_of_map; i++) {
        char *cursor = lines[i];
        size_t dst_range_start = get_next_val_from_string(&cursor);
        cursor++;
        size_t src_range_start = get_next_val_from_string(&cursor);
        cursor++;
        size_t range_length = get_next_val_from_string(&cursor);

        if (key>=dst_range_start && key<(dst_range_start+range_length)) {
          key = src_range_start + (key - dst_range_start);
          break;
        }
      }
    }
    if (does_seed_exist(key, lines[0])) {
      printf("Answer to part 2 = %zu\n", location);
      break;
    }
  }

  free(lines);
  free(contents);

  return 0;
}

