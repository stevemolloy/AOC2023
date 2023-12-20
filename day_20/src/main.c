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
  DynArr inputs;
  DynArr last_recvd;
} Con_Module;

typedef struct {
  bool nop;
} Broadcaster;

typedef struct {
  bool state;
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
  char *sign_recvd_from;
  SigType sending;
  DynArr dests;
  union {
    Con_Module con_module;
    Broadcaster broadcaster;
    FF_Module ff_module;
  } as;
} Module;

int find_module_by_name(Module *modules, size_t num_modules, char *name) {
  for (size_t i=0; i<num_modules; i++) {
    if (strcmp(modules[i].name, name)==0) return i;
  }
  return -1;
}

void send_signal(Module *mod, Module *modules, size_t num_modules) {
  for (size_t i=0; i<mod->dests.len; i++) {
    int mod_num = find_module_by_name(modules, num_modules, mod->dests.data[i]);
    if (mod_num < 0) return;

    if (mod->sending != NONE) modules[mod_num].last_sig_recvd = mod->sending;

    if (modules[mod_num].type == CON) {
      for (size_t j=0; j<modules[mod_num].as.con_module.inputs.len; j++) {
        if (strcmp(modules[mod_num].as.con_module.inputs.data[j], mod->name)==0) {
          if (mod->sending==HI) {
            modules[mod_num].as.con_module.last_recvd.data[j] = "T";
          } else if (mod->sending==LO) {
            modules[mod_num].as.con_module.last_recvd.data[j] = "F";
          }
          break;
        }
      }
    }
  }
}

void process_signal(Module *mod) {
  bool all_conj_true = true;
  switch (mod->type) {
    case BROAD:
      mod->sending = mod->last_sig_recvd;
      break;
    case CON:
      if (mod->last_sig_recvd != NONE) {
        for (size_t i=0; i<mod->as.con_module.inputs.len; i++) {
          if (*mod->as.con_module.last_recvd.data[i] != 'T') {
            all_conj_true = false;
            break;
          }
        }
        if (!all_conj_true) {
          mod->sending = HI;
        } else {
          mod->sending = LO;
        }
      } else {
        mod->sending = NONE;
      }
      break;
    case FF:
      switch (mod->last_sig_recvd) {
        case NONE:
        case HI:
          mod->sending = NONE;
          break;
        case LO:
          // printf("FF state before LO: %d\n", mod->as.ff_module.state);
          if (!mod->as.ff_module.state) {
            mod->as.ff_module.state = true;
            mod->sending = HI;
          } else {
            mod->as.ff_module.state = false;
            mod->sending = LO;
          }
          // printf("FF state after LO: %d\n", mod->as.ff_module.state);
          break;
      }
      break;
  }
  mod->last_sig_recvd = NONE;
}

void print_module(Module mod) {
  printf("%s: ", mod.name);
  switch (mod.type) {
    case CON:
      printf("conjunction module: ");
      break;
    case BROAD:
      printf("broadcaster module: ");
      break;
    case FF:
      printf("flip-flop module (state = %d): ", mod.as.ff_module.state);
      break;
  }
  if (mod.sending == HI) {
    printf("Sending HI to ");
    for (size_t i=0; i<mod.dests.len; i++) {
      printf("%s ", mod.dests.data[i]);
    }
    printf("\n");
  }
  else if (mod.sending == LO) {
    printf("Sending LO to ");
    for (size_t i=0; i<mod.dests.len; i++) {
      printf("%s ", mod.dests.data[i]);
    }
    printf("\n");
  }
  else printf("\n");

}

Broadcaster new_broadcaster(void) {
  Broadcaster result = {0};
  return result;
}

Con_Module new_con_module(void) {
  Con_Module result = {0};

  result.inputs = new_dyn_arr();
  result.last_recvd = new_dyn_arr();

  return result;
}

FF_Module new_ff_module(void) {
  FF_Module result = {0};
  
  return result;
}

void add_strings_to_dynarr(DynArr *da, char *str) {
  advance_past_chars(&str, " ");
  if (strlen(str) == 0) return;

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
  // char *file_path = "./test_input.txt";
  char *file_path = "./test_input2.txt";
  char *buffer;
  char **lines;

  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  Module *modules = calloc(num_lines, sizeof(Module));

  for (size_t i=0; i<num_lines; i++) {
    char *l = lines[i];

    char *name;
    modules[i] = (Module) {0};
    modules[i].dests = new_dyn_arr();
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
        modules[i].sending = NONE;

        l++;
        advance_past_chars(&l, " ->");

        printf("Adding %s to FF\n", l);
        add_strings_to_dynarr(&modules[i].dests, l);

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
        modules[i].sending = NONE;

        l++;
        advance_past_chars(&l, " ->");

        printf("Adding %s to CON\n", l);
        add_strings_to_dynarr(&modules[i].dests, l);

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
        modules[i].sending = NONE;

        l++;
        advance_past_chars(&l, " ->");

        printf("Adding %s to BROAD\n", l);
        add_strings_to_dynarr(&modules[i].dests, l);

        break;
    }
  }

  for (size_t i=0; i<num_lines; i++) {
    for (size_t d=0; d<modules[i].dests.len; d++) {
      int mod_num = find_module_by_name(modules, num_lines, modules[i].dests.data[d]);
      if (mod_num < 0) continue;
      if (modules[mod_num].type == CON) {
        add_to_dyn_array(&modules[mod_num].as.con_module.inputs, modules[i].name);
        add_to_dyn_array(&modules[mod_num].as.con_module.last_recvd, "F");
      }
    }
  }

  size_t steps = 8;
  printf("\n\nPushing the button!!!\n");
  for (size_t step=0; step<steps; step++) {
    for (size_t i=0; i<num_lines; i++) process_signal(&modules[i]);
    for (size_t i=0; i<num_lines; i++) send_signal(&modules[i], modules, num_lines);

    printf("Step %zu\n", step);
    for (size_t i=0; i<num_lines; i++) print_module(modules[i]);
  }

  printf("\n\nPushing the button a second time!!!\n");
  int bcast_num = find_module_by_name(modules, num_lines, "broadcaster");
  modules[bcast_num].last_sig_recvd = LO;

  for (size_t step=0; step<steps; step++) {
    for (size_t i=0; i<num_lines; i++) process_signal(&modules[i]);
    for (size_t i=0; i<num_lines; i++) send_signal(&modules[i], modules, num_lines);

    printf("Step %zu\n", step);
    for (size_t i=0; i<num_lines; i++) print_module(modules[i]);
  }

  printf("\n\nPushing the button a third time!!!\n");
  modules[bcast_num].last_sig_recvd = LO;

  for (size_t step=0; step<steps; step++) {
    for (size_t i=0; i<num_lines; i++) process_signal(&modules[i]);
    for (size_t i=0; i<num_lines; i++) send_signal(&modules[i], modules, num_lines);

    printf("Step %zu\n", step);
    for (size_t i=0; i<num_lines; i++) print_module(modules[i]);
  }

  printf("\n\nPushing the button a fourth time!!!\n");
  modules[bcast_num].last_sig_recvd = LO;

  for (size_t step=0; step<steps; step++) {
    for (size_t i=0; i<num_lines; i++) process_signal(&modules[i]);
    for (size_t i=0; i<num_lines; i++) send_signal(&modules[i], modules, num_lines);

    printf("Step %zu\n", step);
    for (size_t i=0; i<num_lines; i++) print_module(modules[i]);
  }

  return 0;
}

