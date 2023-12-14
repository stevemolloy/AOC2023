#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

size_t get_load_on_north(char **lines, size_t num_rows, size_t num_cols) {
  size_t total = 0;
  for (size_t row=0; row<num_rows; row++) {
    for (size_t col=0; col<num_cols; col++) {
      if (lines[row][col] == 'O') total += num_rows - row;
    }
  }

  return total;
}

// Experimentally I found that the repeated cycle starts at 122 and is 21 steps long.
// (1_000_000_000 - 122) % 21 = 17
// This means that the answer is found after calculating 122 + 17 = 139 total cycles.
#define NUMCYCLES 139

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;

  size_t num_rows = read_entire_file_to_lines(file_path, &buffer, &lines);
  size_t num_cols = strlen(lines[0]);

  char **columns = calloc(num_cols, sizeof(char*));
  for (size_t i=0; i<num_cols; i++) columns[i] = calloc(num_rows+1, sizeof(char));
  
  for (size_t cycle_num=0; cycle_num<NUMCYCLES; cycle_num++) {
    // Transpose for tilting NORTH
    for (size_t i=0; i<num_cols; i++) {
      for (size_t j=0; j<num_rows; j++) {
        columns[i][j] = lines[j][i];
      }
    }

    // Tilt NORTH
    for (size_t col=0; col<num_cols; col++) {
      size_t available_slot = strlen(columns[0]);
      for (size_t i=0; i<strlen(columns[col]); i++) {
        if (columns[col][i] == 'O') {
          if (i != (strlen(columns[col]) - available_slot)) {
            columns[col][strlen(columns[col]) - available_slot] = 'O';
            columns[col][i] = '.';
          }
          available_slot--;
        } else if (columns[col][i] == '#') {
          available_slot = strlen(columns[col]) - i - 1;
        }
      }
    }

    if (cycle_num==0) {
      size_t total = get_load_on_north(lines, num_rows, num_cols);
      printf("Answer to part 1 = %zu (should be 109638)\n", total);
    }

    // Transpose for printing and tilting WEST
    for (size_t i=0; i<num_cols; i++) {
      for (size_t j=0; j<num_rows; j++) {
        lines[i][j] = columns[j][i];
      }
    }

    // Tilt WEST
    for (size_t row=0; row<num_rows; row++) {
      size_t available_slot = strlen(lines[0]);
      for (size_t i=0; i<strlen(lines[row]); i++) {
        if (lines[row][i] == 'O') {
          if (i != (strlen(lines[row]) - available_slot)) {
            lines[row][strlen(lines[row]) - available_slot] = 'O';
            lines[row][i] = '.';
          }
          available_slot--;
        } else if (lines[row][i] == '#') {
          available_slot = strlen(lines[row]) - i - 1;
        }
      }
    }

    // Transpose for tilting SOUTH
    for (size_t i=0; i<num_cols; i++) {
      for (size_t j=0; j<num_rows; j++) {
        columns[i][j] = lines[num_rows - j - 1][i];
      }
    }

    // Tilt SOUTH
    for (size_t col=0; col<num_cols; col++) {
      size_t available_slot = strlen(columns[0]);
      for (size_t i=0; i<strlen(columns[col]); i++) {
        if (columns[col][i] == 'O') {
          if (i != (strlen(columns[col]) - available_slot)) {
            columns[col][strlen(columns[col]) - available_slot] = 'O';
            columns[col][i] = '.';
          }
          available_slot--;
        } else if (columns[col][i] == '#') {
          available_slot = strlen(columns[col]) - i - 1;
        }
      }
    }

    // Transpose for printing
    for (size_t i=0; i<num_cols; i++) {
      for (size_t j=0; j<num_rows; j++) {
        lines[i][j] = columns[j][num_cols - i - 1];
      }
    }

    // Tilt EAST
    for (size_t row=0; row<num_rows; row++) {
      size_t available_slot = num_cols - 1;
      for (int col=num_cols-1; col>=0; col--) {
        if (lines[row][col] == 'O') {
          if (available_slot != (size_t)col) {
            lines[row][available_slot] = 'O';
            lines[row][col] = '.';
          }
          available_slot--;
        } else if (lines[row][col] == '#') {
          available_slot = col - 1;
        }
      }
    }
  }

  size_t total = get_load_on_north(lines, num_rows, num_cols);
  printf("Answer to part 2 = %zu (should be 102657)\n", total);

  for (size_t col=0; col<num_cols; col++) free(columns[col]);
  free(columns);
  free(lines);
  free(buffer);

  return 0;
}
