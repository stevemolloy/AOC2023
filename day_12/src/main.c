#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "aoc_lib.h"

#define INITIAL_CAP 32

size_t checked = 0;

typedef struct {
  size_t cap, len;
  size_t *vals;
} ValArray;

size_t count_char_in_line(char *line, char c) {
  size_t result = 0;
  while (*line) {
    if (*line == c) result++;
    line++;
  }
  return result;
}

ValArray new_valarray(void) {
  ValArray result = {0};
  result.cap = INITIAL_CAP;
  result.vals = calloc(INITIAL_CAP, sizeof(size_t));
  return result;
}

void push_val_to_array(ValArray *va, size_t val) {
  if (va->len == va->cap) {
    va->cap *= 2;
    va->vals = realloc(va->vals, va->cap*sizeof(size_t));
  }
  va->vals[va->len++] = val;
}

void free_valarray(ValArray *va) {
  va->len = 0;
  va->cap = 0;
  free(va->vals);
}

bool are_arrays_equal(ValArray a, ValArray b) {
  checked++;
  size_t Na = a.len;
  size_t Nb = b.len;
  if (Na != Nb) return false;
  for (size_t i=0; i<Na; i++) {
    if (a.vals[i] != b.vals[i]) return false;
  }
  return true;
}

ValArray get_chunks_from_line(char *line) {
  ValArray result = new_valarray();
  char *cursor = line;
  size_t ctr = 0;
  while (*cursor!=' ') {
    if (*cursor == '.') {
      if (ctr != 0) push_val_to_array(&result, ctr);
      ctr = 0;
      advance_past_chars(&cursor, ".");
    } else {
      ctr++;
      cursor++;
    }
  }
  if (ctr > 0) {
    push_val_to_array(&result, ctr);
  }
  return result;
}

ValArray analyse_line(char *line) {
  ValArray result = new_valarray();

  char *cursor = line;
  size_t ctr = 0;
  while (*cursor != ' ') {
    if (*cursor == '?') {
      fprintf(stderr, "Found '?' which should have been eliminated\n");
      exit(1);
    }
    if (*cursor == '.') {
      if (ctr>0) push_val_to_array(&result, ctr);
      ctr = 0;
      cursor++;
    } else if (*cursor == '#') {
      ctr++;
      cursor++;
    }
  }
  
  if (ctr>0) push_val_to_array(&result, ctr);

  return result;
}

ValArray get_vals_from_line(char *line) {
  ValArray result = new_valarray();

  char *cursor = line;
  advance_past_chars(&cursor, ".?# ");
  
  while (*cursor) {
    size_t next_val = get_next_val_from_string(&cursor);
    push_val_to_array(&result, next_val);
    advance_past_chars(&cursor, ",");
  }

  return result;
}

size_t apply_map_to_line_and_test(char *line, size_t num_qms, ValArray testvals) {
  char *temp = calloc(strlen(line), sizeof(char));
  size_t map = (1 << num_qms) - 1;

  size_t result = 0;
  for (size_t i=0; i<=map; i++) {
    memcpy(temp, line, strlen(line));
    char *cursor = temp;
    for (size_t j=0; j<num_qms; j++) {
      advance_past_chars(&cursor, ".#");
      *cursor = (i & (1 << j)) > 0 ? '#' : '.';
    }

    if (are_arrays_equal(testvals, analyse_line(temp))) {
      result++;
    }
  }
  return result;
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;

  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  size_t ans_1 = 0;

  for (size_t i=0; i<num_lines; i++) {
    if (i%10 == 0) printf("Line numnber = %zu\n", i);
    ValArray vals = get_vals_from_line(lines[i]);
    size_t num_qms = count_char_in_line(lines[i], '?');
    size_t num = apply_map_to_line_and_test(lines[i], num_qms, vals);
    ans_1 += num;
  }

  printf("Answer to part 1 = %zu (I checked %zu arrays)\n", ans_1, checked);

  return 0;
}
