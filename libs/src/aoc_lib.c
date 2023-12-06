#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../aoc_lib.h"

size_t read_entire_file_to_lines(char *file_path, char **buffer, char ***lines) {
  *buffer = read_entire_file(file_path);
  size_t num_lines = string_to_lines(buffer, lines);
  return num_lines;
}

char *read_entire_file(char *file_path) {
  // Reads an entire file into a char array, and returns a ptr to this. The ptr should be freed by the caller
  FILE *f = fopen(file_path, "r");
  if (f==NULL) {
    fprintf(stderr, "Could not read %s: %s\n", file_path, strerror(errno));
    exit(1);
  }

  fseek(f, 0L, SEEK_END);
  int sz = ftell(f);
  fseek(f, 0L, SEEK_SET);

  char *contents = calloc(2*sz, sizeof(char));
  if (contents==NULL) {
    fprintf(stderr, "Could not allocate memory. Buy more RAM I guess?\n");
    exit(1);
  }
  fread(contents, 1, sz, f);

  fclose(f);
  
  return contents;
}

size_t string_to_lines(char **string, char ***lines) {
  char *cursor = *string;
  size_t num_lines = count_lines(cursor);

  *lines = calloc(num_lines, sizeof(char*));

  size_t line_ctr = 0;
  while (*cursor) {
    (*lines)[line_ctr] = cursor;
    assert(line_ctr < num_lines);
    advance_to_char(&cursor, '\n');
    *cursor = '\0';
    (cursor)++;
    line_ctr++;
  }

  return num_lines;
}

int get_value_from_str(char **str, int *first, int *last) {
  // Searches for number strings at the start of a c string
  // If the search is successful, populate *first and *last appropriately
  // Return 1 if successful and 0 if not
  const char* digit_strings[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

  for (size_t i=0; i<9; i++) {
    if (strncmp(digit_strings[i], *str, strlen(digit_strings[i]))==0) {
      *last = i+1;
      if (*first < 0) *first = i+1;
      (*str)++;
      return 1;
    }
  }
  (*str)++;
  return 0;
}

size_t get_value_from_line(char **string) {
  // Returns the number encoded in the line of text held in the string (stopping at '\n')
  int last = 0, first = -1;
  while (**string != '\n') {
    if (isdigit(**string)) {
      last = **string - '0';
      if (first < 0) first = **string - '0';
      (*string)++;
      continue;
    }
    if (get_value_from_str(string, &first, &last)) {
      continue;
    }
  }
  (*string)++;

  return first*10 + last;
}

void get_dims_of_char_matrix(char *contents, size_t *w, size_t *h) {
  char *cursor = contents;

  *w = 0;
  *h = 0;

  while(*cursor) {
    if (*cursor=='\n') break;
    (*w)++;
    cursor++;
  }

  cursor = contents;
  while (*cursor) {
    if (*cursor == '\n') (*h)++;
    cursor++;
  }
}

void make_data_grid_from_string(char *string, size_t cols, size_t rows, char *grid) {
  char *cursor = string;
  for (size_t x=0; x<cols; x++) {
    for (size_t y=0; y<rows; y++) {
      if (*cursor == '\n') cursor++;
      grid[x*cols + y] = *cursor;
      cursor++;
    }
  }
}

size_t sum_vector(size_t *vect, size_t N) {
  size_t result = 0;
  for (size_t i=0; i<N; i++) {
    result += vect[i];
  }
  return result;
}

void advance_past_chars(char **string, char *chars) {
  if (**string == '\0') return;
  while (strchr(chars, **string)) {
    (*string)++;
  }
}

void advance_to_next_line(char **string) {
  while (**string != '\n') (*string)++;
  (*string)++;
}

size_t count_lines(char *contents) {
  size_t result = 0;
  while (*contents) {
    if (*contents == '\n') result++;
    contents++;
  }
  return result;
}

void advance_to_char(char **string, char c) {
  while (**string != c) (*string)++;
}

size_t get_next_val_from_string(char **string) {
  if (!isdigit(**string)) {
    fprintf(stderr, "%s does not begin with a digit", *string);
    exit(1);
  }
  size_t result = 0;

  do {
    result = result*10 + **string - '0';
    (*string)++;
  } while (isdigit(**string));

  return result;
}

