#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aoc_lib.h"

#define INI_CAP 32

typedef struct {
  size_t cap;
  size_t len;
  char **data;
} DynArr;

DynArr new_dyn_arr(void) {
  DynArr result = {0};
  result.cap = INI_CAP;
  result.data = calloc(INI_CAP, sizeof(char*));
  return result;
}

void add_to_dyn_array(DynArr *da, char *data) {
  da->len++;
  if (da->len == da->cap) {
    da->cap *= 2;
    da->data = realloc(da->data, da->cap*sizeof(size_t));
  }
  da->data[da->len-1] = data;
}

typedef struct {
  DynArr input_inds;
  DynArr last_recvd;
  DynArr dests;
} Con_Module;

typedef struct {
  DynArr dests;
} Broadcaster;

typedef struct {
  bool state;
  DynArr dests;
} FF_Module;

typedef enum {
  CON,
  BROAD,
  FF,
} ModType;

typedef enum {
  NONE,
  HI,
  LO,
} SigType;

typedef struct {
  char *name;
  ModType type;
  SigType last_sig_recvd;
  union {
    Con_Module con_module;
    Broadcaster broadcaster;
    FF_Module ff_module;
  } as;
} Module;

void print_module(Module mod) {
  printf("%s: ", mod.name);
  switch (mod.type) {
    case CON:
      printf("conjunction module\n");
      printf("\tDests = ");
      for (size_t i=0; i<mod.as.con_module.dests.len; i++) {
        printf("%s ", mod.as.con_module.dests.data[i]);
      }
      printf("\n");
      break;
    case BROAD:
      printf("broadcaster module\n");
      printf("\tDests = ");
      for (size_t i=0; i<mod.as.broadcaster.dests.len; i++) {
        printf("%s ", mod.as.broadcaster.dests.data[i]);
      }
      printf("\n");
      break;
    case FF:
      printf("flip-flop module\n");
      printf("\tDests = ");
      for (size_t i=0; i<mod.as.ff_module.dests.len; i++) {
        printf("%s ", mod.as.ff_module.dests.data[i]);
      }
      printf("\n");
      break;
  }
}

Broadcaster new_broadcaster(void) {
  Broadcaster result = {0};
  result.dests = new_dyn_arr();
  return result;
}

Con_Module new_con_module(void) {
  Con_Module result = {0};

  result.input_inds = new_dyn_arr();
  result.last_recvd = new_dyn_arr();
  result.dests = new_dyn_arr();

  return result;
}

FF_Module new_ff_module(void) {
  FF_Module result = {0};
  
  result.dests = new_dyn_arr();

  return result;
}

void add_strings_to_dynarr(DynArr *da, char *str) {
  advance_past_chars(&str, " ");
  assert(isalpha(*str) && "Expected a name here, but the value is not alphabetical");

  char *val = str;
  while (*str) {
    str++;
    if (*str == ',') {
      *str = '\0';
      str++;
      add_to_dyn_array(da, val);
      advance_past_chars(&str, " ");
      val = str;
    }
    advance_past_chars(&str, " ");
  }
  add_to_dyn_array(da, val);
}

int main(void) {
  char *file_path = "./test_input.txt";
  char *buffer;
  char **lines;

  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  Module *modules = calloc(num_lines, sizeof(Module));

  for (size_t i=0; i<num_lines; i++) {
    char *l = lines[i];

    char *name;
    modules[i] = (Module) {0};
    switch (*l) {
      case '%':
        l++;
        name = l;
        advance_to_char(&l, ' ');
        *l = '\0';
        modules[i].name = name;
        modules[i].type = FF;
        modules[i].as.ff_module = new_ff_module();

        modules[i].last_sig_recvd = NONE;

        l++;
        advance_past_chars(&l, " ->");

        add_strings_to_dynarr(&modules[i].as.ff_module.dests, l);

        break;
      case '&':
        l++;
        name = l;
        advance_to_char(&l, ' ');
        *l = '\0';
        modules[i].name = name;
        modules[i].type = CON;
        modules[i].as.con_module = new_con_module();

        modules[i].last_sig_recvd = NONE;

        l++;
        advance_past_chars(&l, " ->");

        add_strings_to_dynarr(&modules[i].as.con_module.dests, l);

        break;
      default:
        assert(*l == 'b' && "Expecting a 'broadcaster' node here, but didn't find it.");
        name = l;
        advance_to_char(&l, ' ');
        *l = '\0';
        modules[i].name = name;
        modules[i].type = BROAD;
        modules[i].as.broadcaster = new_broadcaster();

        modules[i].last_sig_recvd = LO;

        l++;
        advance_past_chars(&l, " ->");

        add_strings_to_dynarr(&modules[i].as.broadcaster.dests, l);

        break;
    }
  }

  for (size_t i=0; i<num_lines; i++) {
    print_module(modules[i]);
  }

  return 0;
}

