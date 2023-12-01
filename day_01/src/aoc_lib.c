#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

