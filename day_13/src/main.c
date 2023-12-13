#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "aoc_lib.h"

bool compare_two_lines(char *l1, char *l2) {
  assert(strlen(l1) == strlen(l2) && "Line lengths do not match");
  for (size_t i=0; i<strlen(l1); i++) {
    if (l1[i] != l2[i]) return false;
  }
  return true;
}

bool compare_two_columns(char **lines, size_t start_row, size_t end_row, size_t col1, size_t col2) {
  for (size_t i=start_row; i<end_row; i++) {
    if (lines[i][col1] != lines[i][col2]) return false;
  }
  return true;
}

int main(void) {
  // char *file_path = "./my_test_input.txt";
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;

  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  size_t num_grids = 1;
  for (size_t i=0; i<num_lines; i++) {
    if (strlen(lines[i]) == 0) num_grids++;
  }
  size_t *grid_starts = calloc(num_grids, sizeof(size_t));
  size_t *grid_ends   = calloc(num_grids, sizeof(size_t));
  size_t ind = 0;
  grid_starts[ind] = 0;
  for (size_t i=0; i<num_lines; i++) {
    if (strlen(lines[i]) == 0) {
      grid_ends[ind] = i;
      ind++;
      grid_starts[ind] = i+1;
    }
  }
  grid_ends[ind] = num_lines;

  printf("I found %zu grids\n", num_grids);

  size_t ans_1 = 0;
  for (size_t gridN=0; gridN<num_grids; gridN++) {
    int start = grid_starts[gridN];
    int end   = grid_ends[gridN];

    size_t lines_before;

    bool found_a_match = false;

    // Search to find any lines that match the first line
    char *line_to_compare_with = lines[start];
    int match = start + 1;
    if (!found_a_match) {
      for ( ; match<end; match++) {
        if (compare_two_lines(line_to_compare_with, lines[match])) {
          found_a_match = true;
          int s1 = start + 1;
          int s2 = match - 1;
          while (s1 < s2) {
            if (!compare_two_lines(lines[s1], lines[s2])) {
              found_a_match = false;
              break;
            }
            s1++;
            s2--;
          }
          if (found_a_match) {
            printf("Found a horizontal mirror for grid %zu at %d\n", gridN, match);
            lines_before = (match - start) / 2 + 1;
            printf("\tAns for grid %zu = %zu\n", gridN, lines_before);
            ans_1 += 100 * lines_before;
            break;
          }
        }
      }
    }

    // Search to find any lines that match the last line
    line_to_compare_with = lines[end-1];
    match -= 2;
    if (!found_a_match) {
      for ( ; match>=start; match--) {
        if (compare_two_lines(line_to_compare_with, lines[match])) {
          found_a_match = true;
          int s1 = match + 1;
          int s2 = end - 1 - 1;
          while (s1 < s2) {
            if (!compare_two_lines(lines[s1], lines[s2])) {
              found_a_match = false;
              break;
            }
            s1++;
            s2--;
          }
          if (found_a_match) {
            printf("Found a horizontal mirror for grid %zu at %d\n", gridN, match);
            lines_before = 1 + match + ((end-1) - match)/2 - start;
            printf("\tAns for grid %zu = %zu\n", gridN, lines_before);
            ans_1 += 100 * lines_before;
            break;
          }
        }
      }
    }

    // Search to find any columns that match the first one
    size_t col_to_compare_with = 0;
    match = 1;
    if (!found_a_match) {
      for ( ; match<(int)strlen(lines[start]); match++) {
        if (compare_two_columns(lines, start, end, col_to_compare_with, match)) {
          found_a_match = true;
          int s1 = col_to_compare_with + 1;
          int s2 = match - 1;
          while (s1 < s2) {
            if (!compare_two_columns(lines, start, end, s1, s2)) {
              found_a_match = false;
              break;
            }
            s1++;
            s2--;
          }
          if (found_a_match) {
            printf("Found a vertical mirror for grid %zu at %d\n", gridN, match);
            lines_before = (match + 1) / 2;
            printf("\tAns for grid %zu = %zu\n", gridN, lines_before);
            ans_1 += lines_before;
            break;
          }
        }
      }
    }

    // Search to find any columns that match the last one
    col_to_compare_with = strlen(lines[start]) - 1;
    match -= 2;
    if (!found_a_match) {
      for ( ; match>=0; match--) {
        if (compare_two_columns(lines, start, end, col_to_compare_with, match)) {
          found_a_match = true;
          int s1 = match + 1;
          int s2 = col_to_compare_with - 1;
          while (s1 < s2) {
            if (!compare_two_columns(lines, start, end, s1, s2)) {
              found_a_match = false;
              break;
            }
            s1++;
            s2--;
          }
          if (found_a_match) {
            printf("Found a vertical mirror for grid %zu at %d\n", gridN, match);
            // printf("\tFindings: start = %d, end = %d, col_to_compare_with = %zu, match = %d\n", start, end, col_to_compare_with, match);
            lines_before = match + 1 + (col_to_compare_with - match)/2;
            printf("\tAns for grid %zu = %zu\n", gridN, lines_before);
            ans_1 += lines_before;
            break;
          }
        }
      }
    }

    if (!found_a_match) {
      printf("DID NOT FIND A MATCH FOR GRID %zu\n", gridN);
    }
  }

  printf("Answer to part 1 = %zu\n", ans_1);

  return 0;
}
