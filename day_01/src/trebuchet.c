#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const char* digit_strings[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

int main(void) {
  char *file_path = "./real_input.txt";
  FILE *f = fopen(file_path, "r");
  if (f==NULL) {
    fprintf(stderr, "Could not read %s: %s\n", file_path, strerror(errno));
  }

  fseek(f, 0L, SEEK_END);
  int sz = ftell(f);
  fseek(f, 0L, SEEK_SET);

  char *contents = calloc(2*sz, sizeof(char));
  fread(contents, 1, sz, f);

  char *cursor = contents;
  int sum = 0;
  while (*cursor) {
    int last = 0;
    int first = -1;
    while (*cursor != '\n') {
      if (isdigit(*cursor)) {
        last = *cursor - '0';
        if (first < 0) first = *cursor - '0';
      }
      for (size_t i=0; i<9; i++) {
        if (strncmp(digit_strings[i], cursor, strlen(digit_strings[i]))==0) {
          last = i+1;
          if (first < 0) first = i+1;
        }
      }
      cursor++;
    }
    sum += first*10 + last;
    cursor++;
  }

  printf("Sum = %d\n", sum);

  free(contents);
  fclose(f);

  return 0;
}
