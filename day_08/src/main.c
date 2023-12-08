#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct {
  char *name;
  char *left;
  char *right;
} Node;

typedef struct {
  char *key;
  Node value;
} Map_Element;

size_t gcd(size_t a, size_t b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}
 
size_t lcm(size_t arr[], size_t n) {
  size_t ans = arr[0];
 
  for (size_t i = 1; i < n; i++)
    ans = (((arr[i] * ans)) / (gcd(arr[i], ans)));
 
  return ans;
}

size_t get_node_by_name(Node *nodes, size_t num_nodes, char *name) {
  for (size_t i=0; i<num_nodes; i++) {
    if (strncmp(nodes[i].name, name, 3) == 0) return i;
  }
  fprintf(stderr, "Node with name %s not found\n", name);
  exit(1);
}

bool check_all_at_end(Node *nodes, size_t *inds, size_t ind_count) {
  for (size_t i=0; i<ind_count; i++) {
    if (nodes[inds[i]].name[2] != 'Z') return false;
  }
  return true;
}

size_t count_starting_nodes(Map_Element *nodes, size_t num_nodes) {
  size_t result = 0;
  for (size_t i=0; i<num_nodes; i++) {
    if (nodes[i].value.name[2] == 'A') result++;
  }
  return result;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Provide a single input -- the file to be analysed\n");
    return 1;
  }
  char *buffer;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(argv[1], &buffer, &lines);

  size_t instruction_count = strlen(lines[0]);

  size_t num_nodes = num_lines - 2;

  Map_Element *map = NULL;

  // Node *nodes = calloc(num_nodes, sizeof(Node));
  for (size_t i=0; i<num_nodes; i++) {
    char *cursor = lines[i+2];
    Node node = (Node) {0};
    node.name = cursor;
    cursor += 3;
    *cursor = '\0';
    advance_to_char(&cursor, '(');
    cursor++;
    node.left = cursor;
    cursor += 3;
    *cursor = '\0';
    cursor += 2;
    node.right = cursor;
    cursor += 3;
    *cursor = '\0';

    shput(map, node.name, node);
  }

  // size_t current_node = get_node_by_name(nodes, num_nodes, "AAA");
  Node current_node = shget(map, "AAA");
  size_t steps_taken = 0;
  for (size_t i=0; ; i = (i+1) % instruction_count) {
    Node n = current_node;
    if (strncmp(n.name, "ZZZ", 3) == 0) {
      printf("Done!\n");
      break;
    }
    if (lines[0][i] == 'L') current_node = shget(map, n.left);
    else current_node = shget(map, n.right);
    steps_taken++;
  }

  printf("Answer to part 1 = %zu\n", steps_taken);

  size_t num_start_points = count_starting_nodes(map, num_nodes);

  size_t *active_nodes = calloc(num_start_points, sizeof(size_t));
  size_t an_index = 0;
  for (size_t i=0; i<num_nodes; i++) {
    if (map[i].value.name[2] == 'A') active_nodes[an_index++] = i;
  }

  size_t *steps = calloc(num_start_points, sizeof(size_t));

  for (size_t i=0; i<num_start_points; i++) {
    size_t steps_taken = 0;
    for (size_t j=0; ; j = (j+1) % instruction_count) {
      char instruction = lines[0][j];
      Node n = map[active_nodes[i]].value;
      if (n.name[2] == 'Z') {
        steps[i] = steps_taken;
        break;
      }
      if (instruction == 'L') active_nodes[i] = shgeti(map, n.left);
      else active_nodes[i] = shgeti(map, n.right);
      steps_taken++;
    }
  }

  size_t ans = lcm(steps, num_start_points);
  printf("Answer to part 2 = %zu\n", ans);

  free(buffer);
  free(lines);
  hmfree(map);
  free(active_nodes);
  free(steps);

  return 0;
}

