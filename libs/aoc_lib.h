#ifndef _AOC_LIB_H
#define _AOC_LIB_H

#include <stdio.h>
#include <stdbool.h>

typedef struct {
  size_t x,y;
} Pos;

int get_value_from_str(char **str, int *first, int*last);

size_t get_value_from_line(char **string);

char *read_entire_file(char *file_path);

size_t read_entire_file_to_lines(char *file_path, char **buffer, char ***lines);

void get_dims_of_char_matrix(char *contents, size_t *w, size_t *h);

void make_data_grid_from_string(char *string, size_t cols, size_t rows, char *grid);

size_t sum_vector(size_t *vect, size_t N);

void advance_past_chars(char **string, char *chars);

void advance_to_next_line(char **string);

size_t count_lines(char *contents);

void advance_to_char(char **string, char c);

size_t string_to_lines(char **string, char ***lines);

size_t get_next_val_from_string(char **string);

int get_next_int_from_string(char **string);

size_t count_digits(size_t val);

size_t ten_to_the_power_of(size_t exp);

size_t triangular_number(size_t n);

size_t dist_between_pos(Pos a, Pos b);

void get_all_distances(Pos *pos_list, size_t N, size_t *distances);

bool is_symbol_in_row(char **lines, size_t row_num, char c);

bool is_symbol_in_col(char **lines, size_t num_lines, size_t col_num, char c);

#endif /* ifndef _AOC_LIB_H */

