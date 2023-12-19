#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

#define RULE_LEN 32
#define NAME_LEN 16

typedef struct {
  size_t x, m, a, s;
} Part;

typedef struct {
  char *name;
  char *conditions;
} Workflow;

typedef struct {
  size_t x_lo, x_hi;
  size_t m_lo, m_hi;
  size_t a_lo, a_hi;
  size_t s_lo, s_hi;
} Range;

size_t range_size(Range r) {
  if ((r.x_hi<=r.x_lo) || (r.m_hi<=r.m_lo) || (r.a_hi<=r.m_lo) || (r.s_hi<=r.s_lo)) return 0;
  size_t result = 1;
  result *= r.x_hi - r.x_lo + 1;
  result *= r.m_hi - r.m_lo + 1;
  result *= r.a_hi - r.a_lo + 1;
  result *= r.s_hi - r.s_lo + 1;
  return result;
}

char *get_rule_condition(Workflow *workflows, size_t num_workflows, char *name) {
  for (size_t i=0; i<num_workflows; i++) {
    if (strcmp(name, workflows[i].name) == 0) {
      return workflows[i].conditions;
    }
  }
  fprintf(stderr, "You searched for a rule (%s) that doesn't exist\n", name);
  exit(1);
}

size_t get_num_that_pass_condition(Range r, Workflow *wfs, size_t num_wfs, char *workflow_name, size_t acc) {
  if (strcmp("A", workflow_name) == 0) return acc + range_size(r);
  if (strcmp("R", workflow_name) == 0) return 0;
  char *wf = get_rule_condition(wfs, num_wfs, workflow_name);
  
  while (*wf) {
    while (*wf == ',') wf++;
    if (strcmp("A", wf) == 0) return acc + range_size(r);
    if (strcmp("R", wf) == 0) return 0;
    char *destination = calloc(NAME_LEN, sizeof(char));
    size_t val = 0;

    char property = *wf;
    if (!isalpha(*(wf+1))) {
      wf++;
      char test = *wf;
      wf++;
      val = get_next_val_from_string(&wf);
      wf++;
      size_t t = 0;
      while (isalpha(*wf)) {
        destination[t++] = *wf;
        wf++;
      }

      Range new_r = r;
      switch (property) {
        case 'x':
          if (test == '>') {
            if      (new_r.x_lo < val) new_r.x_lo = val;
            else if (new_r.x_hi > val) new_r.x_hi = val;
            if      (r.x_hi > val) r.x_hi = val;
            else if (r.x_lo < val) r.x_lo = val;
          } /*else if (test == '<') {
            if      (new_r.x_hi > val) new_r.x_hi = val;
            else if (new_r.x_lo < val) new_r.x_lo = val;
            if      (r.x_lo < val) r.x_lo = val;
            else if (r.x_hi > val) r.x_hi = val;
          }*/
          acc += get_num_that_pass_condition(new_r, wfs, num_wfs, destination, acc);
          break;
        case 'm':
          if (test=='>') {
            if      (new_r.m_lo < val) new_r.m_lo = val;
            else if (new_r.m_hi > val) new_r.m_hi = val;
            if      (r.m_hi > val) r.m_hi = val;
            else if (r.m_lo < val) r.m_lo = val;
          } /*else if (test == '<') {
            if      (new_r.m_hi > val) new_r.m_hi = val;
            else if (new_r.m_lo < val) new_r.m_lo = val;
            if      (r.m_lo < val) r.m_lo = val;
            else if (r.m_hi > val) r.m_hi = val;
          }*/
          acc += get_num_that_pass_condition(new_r, wfs, num_wfs, destination, acc);
          break;
        case 'a':
          if (test == '>') {
            if      (new_r.a_lo < val) new_r.a_lo = val;
            else if (new_r.a_hi > val) new_r.a_hi = val;
            if      (r.a_hi > val) r.a_hi = val;
            else if (r.a_lo < val) r.a_lo = val;
          } /*else if (test == '<') {
            if      (new_r.a_hi > val) new_r.a_hi = val;
            else if (new_r.a_lo < val) new_r.a_lo = val;
            if      (r.a_lo < val) r.a_lo = val;
            else if (r.a_hi > val) r.a_hi = val;
          }*/
          acc += get_num_that_pass_condition(new_r, wfs, num_wfs, destination, acc);
          break;
        case 's':
          if (test == '>') {
            if      (new_r.s_lo < val) new_r.s_lo = val;
            else if (new_r.s_hi > val) new_r.s_hi = val;
            if      (r.s_hi > val) r.s_hi = val;
            else if (r.s_lo < val) r.s_lo = val;
          } /*else if (test == '<') {
            if      (new_r.s_hi > val) new_r.s_hi = val;
            else if (new_r.s_lo < val) new_r.s_lo = val;
            if      (r.s_lo < val) r.s_lo = val;
            else if (r.s_hi > val) r.s_hi = val;
          }*/
          acc += get_num_that_pass_condition(new_r, wfs, num_wfs, destination, acc);
          break;
        default:
          fprintf(stderr, "Property must be 'x', 'm', 'a', or 's'. Found %c\n", property);
          exit(1);
      }
    } else {
      size_t t = 0;
      while (isalpha(*wf)) {
        destination[t++] = *wf;
        wf++;
      }
      acc += get_num_that_pass_condition(r, wfs, num_wfs, destination, acc);
    }
  }

  return acc;
}

int main(void) {
  // char *file_path = "./my_test.txt";
  char *file_path = "./test_input.txt";
  // char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;

  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  char **parts_list;
  size_t parts_list_len = 0;
  char **workflow_list = lines;
  size_t workflow_list_len = 0;
  for (size_t i=0; i<num_lines; i++) {
    if (*lines[i] == '{') {
      parts_list = &lines[i];
      parts_list_len = num_lines - i;
      workflow_list_len = i - 1;
      break;
    }
  }

  Workflow *workflows = calloc(workflow_list_len, sizeof(Workflow));
  for (size_t i=0; i<workflow_list_len; i++) {
    workflows[i] = (Workflow) {0};
    char *text = workflow_list[i];
    workflows[i].name = calloc(NAME_LEN, sizeof(char));
    size_t t = 0;
    while (isalpha(*text)) {
      workflows[i].name[t++] = *text;
      text++;
    }
    assert(*text=='{' && "Something has gone wrong with parsing the workflow");
    text++;
    workflows[i].conditions = calloc(RULE_LEN, sizeof(char));
    t = 0;
    while (*text != '}') {
      workflows[i].conditions[t++] = *text;
      text++;
    }
  }

  Part *parts = calloc(parts_list_len, sizeof(Part));
  for (size_t i=0; i<parts_list_len; i++) {
    parts[i] = (Part) {0};
    char *text = parts_list[i];
    assert(*text == '{');
    text++;
    while (1) {
      if (*text == 'x') {
        text += 2;
        parts[i].x = get_next_val_from_string(&text);
      } else if (*text == 'm') {
        text += 2;
        parts[i].m = get_next_val_from_string(&text);
      } else if (*text == 'a') {
        text += 2;
        parts[i].a = get_next_val_from_string(&text);
      } else if (*text == 's') {
        text += 2;
        parts[i].s = get_next_val_from_string(&text);
      } else if (*text == '}') {
        break;
      } else if (*text == ',') {
        text++;
      } else {
        assert(0 && "Unreachable code");
      }
    }
  }

  size_t total = 0;
  char *destination = calloc(NAME_LEN, sizeof(char));
  for (size_t p=0; p<parts_list_len; p++) {
    char *rule_to_apply = "in";
    bool terminated = false;
    do {
      char *cond = get_rule_condition(workflows, workflow_list_len, rule_to_apply);

      while (1) {
        if (*cond == 'x' && !isalpha(*(cond+1))) {
          cond++;
          if (*cond == '<') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].x < val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else if (*cond == '>') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].x > val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else assert(0 && "Unreachable if there are only < and > conditions");
        } else if (*cond == 'm' && !isalpha(*(cond+1))) {
          cond++;
          if (*cond == '<') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].m < val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else if (*cond == '>') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].m > val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else assert(0 && "Unreachable if there are only < and > conditions");
        } else if (*cond == 'a' && !isalpha(*(cond+1))) {
          cond++;
          if (*cond == '<') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].a < val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else if (*cond == '>') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].a > val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else assert(0 && "Unreachable if there are only < and > conditions");
        } else if (*cond == 's' && !isalpha(*(cond+1))) {
          cond++;
          if (*cond == '<') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].s < val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else if (*cond == '>') {
            cond++;
            size_t val = get_next_val_from_string(&cond);
            cond++;
            memset(destination, 0, NAME_LEN*sizeof(char));
            size_t t = 0;
            while (isalpha(*cond)) {
              destination[t++] = *cond;
              cond++;
            }
            if (parts[p].s > val) {
              if (strcmp("A", destination)==0) {
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                terminated = true;
                break;
              }
              rule_to_apply = destination;
              break;
            }
          } else {
            fprintf(stderr, "Cond \"%s\" cannot be parsed\n", cond);
            exit(1);
          }
        } else {
          memset(destination, 0, NAME_LEN*sizeof(char));
          size_t t = 0;
          while (isalpha(*cond)) {
            destination[t++] = *cond;
            cond++;
          }
          if (strcmp("A", destination)==0) {
            total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
            terminated = true;
          }
          if (strcmp("R", destination)==0) {
            terminated = true;
          }
          rule_to_apply = destination;
          break;
        }
        cond++;
      }
    } while (!terminated);
  }

  printf("Answer to part 1 = %zu\n", total);

  Range range = {
    .x_lo=1, .x_hi=4000,
    .m_lo=1, .m_hi=4000,
    .a_lo=1, .a_hi=4000,
    .s_lo=1, .s_hi=4000,
  };
  size_t ans2 = get_num_that_pass_condition(range, workflows, workflow_list_len, "in", 0);
  printf("Answer to part 2 = %zu (cf 167409079868000 for test input)\n", ans2);
  printf("Ratio = %f\n", (float)ans2 / 167409079868000.0);

  for (size_t i=0; i<workflow_list_len; i++) {
    free(workflows[i].name);
    free(workflows[i].conditions);
  }
  free(workflows);
  free(destination);
  free(parts);
  free(lines);
  free(buffer);

  return 0;
}

