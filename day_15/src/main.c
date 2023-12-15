#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef struct {
  char *label;
  int F;
} Lens;

#define BOX_QUEUE_SIZE 16

typedef struct {
  size_t cap;
  size_t len;
  Lens* queue;
} LensQueue;

LensQueue empty_lens_queue(void) {
  LensQueue result = {
    .cap = BOX_QUEUE_SIZE,
    .len = 0,
    .queue = calloc(BOX_QUEUE_SIZE, sizeof(Lens)),
  };
  return result;
}

void add_to_lens_queue(LensQueue *lq, Lens lens) {
  if (lq->len == lq->cap) {
    lq->queue = realloc(lq->queue, lq->cap*2 * sizeof(Lens));
    lq->cap *= 2;
  }
  lq->queue[lq->len++] = lens;
  return;
}

int remove_item_from_lens_queue(LensQueue *lq, char *label) {
  int removed = -1;
  for (size_t i=0; i<lq->len; i++) {
    if (strcmp(lq->queue[i].label, label) == 0) {
      removed = (int)i;
      break;
    }
  }
  if (removed >= 0) {
    free(lq->queue[removed].label);
    for (size_t i=(size_t)removed; i<lq->len-1; i++) {
      lq->queue[i] = lq->queue[i+1];
    }
    lq->len--;
  }
  return removed;
}

int swap_item_in_lens_queue(LensQueue *lq, char *label, size_t fl) {
  int removed = -1;
  for (size_t i=0; i<lq->len; i++) {
    if (strcmp(lq->queue[i].label, label) == 0) {
      removed = (int)i;
      lq->queue[i].F = fl;
      return removed;
    }
  }
  add_to_lens_queue(lq, (Lens){label, fl});
  return removed;
}

void free_lens_queue(LensQueue *lq) {
  lq->len = 0;
  lq->cap = 0;
  free(lq->queue);
  return;
}

LensQueue Box[256];

size_t hash_current_char(char c, size_t curr_hash) {
  curr_hash += c;
  curr_hash = (curr_hash%256) * 17;
  curr_hash %= 256;
  return curr_hash;
}

int main(void) {
  // char *file_path = "./test_input.txt";
  char *file_path = "./real_input.txt";

  char *contents = read_entire_file(file_path);

  char *cursor = contents;
  size_t hash = 0;
  size_t total = 0;
  while (*cursor) {
    if (*cursor == '\n') {
    } else if (*cursor == ',') {
      total += hash;
      hash = 0;
    } else {
      hash = hash_current_char(*cursor, hash);
    }
    cursor++;
  }
  total += hash;
  printf("Answer to part 1 = %zu (should be 510013)\n", total);

  cursor = contents;

  for (size_t i=0; i<256; i++) Box[i] = empty_lens_queue();

  while (*cursor) {
    size_t addr = 0;
    char *label = calloc(1024, sizeof(char));
    size_t ind = 0;
    while (*cursor != '=' && *cursor != '-') {
      label[ind++] = *cursor;
      addr = hash_current_char(*cursor, addr);
      cursor++;
    }
    assert(addr < 256);
    if (*cursor == '=') {
      cursor++;
      swap_item_in_lens_queue(&Box[addr], label, *cursor - '0');
    } else if (*cursor == '-') {
      remove_item_from_lens_queue(&Box[addr], label);
      free(label);
    } else {
      fprintf(stderr, "Error: Could not understand '%c' character\n", *cursor);
      return 1;
    }
    cursor += 2;
  }

  // for (size_t i=0; i<256; i++) {
  //   if (Box[i].len > 0) {
  //     printf("Box[%zu]: ", i);
  //     for (size_t l=0; l<Box[i].len; l++) {
  //       printf("label = %s, FL = %d :", Box[i].queue[l].label, Box[i].queue[l].F);
  //     }
  //     printf("\n");
  //   }
  // }
  total = 0;
  for (size_t b=0; b<256; b++) {
    for (size_t l=0; l<Box[b].len; l++) {
      size_t prod = (b+1) * (l+1) * Box[b].queue[l].F;
      total += prod;
    }
  }

  printf("Answer to part 2 = %zu (should be 268497)\n", total);

  for (size_t i=0; i<256; i++) {
    for (size_t l=0; l<Box[l].len; l++) {
      free(Box[i].queue[l].label);
    }
    free_lens_queue(&Box[i]);
  }
  free(contents);

  return 0;
}
