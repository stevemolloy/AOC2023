#ifndef _AOC_LIB_H
#define _AOC_LIB_H

#include <stdio.h>

int get_value_from_str(char **str, int *first, int*last);

size_t get_value_from_line(char **string);

char *read_entire_file(char *file_path);

#endif /* ifndef _AOC_LIB_H */

