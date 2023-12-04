#ifndef _AOC_LIB_H
#define _AOC_LIB_H

#include <stdio.h>

int get_value_from_str(char **str, int *first, int*last);

size_t get_value_from_line(char **string);

char *read_entire_file(char *file_path);

void get_dims_of_char_matrix(char *contents, size_t *w, size_t *h);

void make_data_grid_from_string(char *string, size_t cols, size_t rows, char *grid);

size_t sum_vector(size_t *vect, size_t N);

void advance_past_chars(char **string, char *chars);

void go_to_next_line(char **string);

size_t count_lines(char *contents);

#endif /* ifndef _AOC_LIB_H */

