#include <assert.h>
#include <ctype.h>
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

char *get_rule_condition(Workflow *workflows, size_t num_workflows, char *name) {
  for (size_t i=0; i<num_workflows; i++) {
    if (strcmp(name, workflows[i].name) == 0) {
      return workflows[i].conditions;
    }
  }
  fprintf(stderr, "You searched for a rule (%s) that doesn't exist\n", name);
  exit(1);
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";
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
      printf("%s --> ", rule_to_apply);
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
                printf("Accepted!\n");
                total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
                terminated = true;
                break;
              }
              if (strcmp("R", destination)==0) {
                printf("Rejected!\n");
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
            printf("Accepted!\n");
            total += parts[p].x + parts[p].m + parts[p].a + parts[p].s;
            terminated = true;
          }
          if (strcmp("R", destination)==0) {
            printf("Rejected!\n");
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

  for (size_t i=0; i<workflow_list_len; i++) {
    free(workflows[i].name);
    free(workflows[i].conditions);
  }
  free(workflows);
  free(parts);
  free(lines);
  free(buffer);

  return 0;
}

