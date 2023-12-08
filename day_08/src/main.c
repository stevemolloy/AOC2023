#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef struct {
  char *name;
  char *left;
  char *right;
} Node;

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

size_t count_starting_nodes(Node *nodes, size_t num_nodes) {
  size_t result = 0;
  for (size_t i=0; i<num_nodes; i++) {
    if (nodes[i].name[2] == 'A') result++;
  }
  return result;
}

int main(void) {
  // char *file_path = "./test_input_1.txt";
  // char *file_path = "./test_input_2.txt";
  // char *file_path = "./test_for_part2.txt";
  char *file_path = "./real_input.txt";
  char *buffer;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(file_path, &buffer, &lines);

  size_t instruction_count = strlen(lines[0]);

  size_t num_nodes = num_lines - 2;

  Node *nodes = calloc(num_nodes, sizeof(Node));
  for (size_t i=0; i<num_nodes; i++) {
    char *cursor = lines[i+2];
    nodes[i] = (Node) {0};
    nodes[i].name = cursor;
    advance_to_char(&cursor, '(');
    cursor++;
    nodes[i].left = cursor;
    cursor += 3 + 2;
    nodes[i].right = cursor;
  }

  // size_t current_node = get_node_by_name(nodes, num_nodes, "AAA");
  // size_t steps_taken = 0;
  // for (size_t i=0; ; i = (i+1) % instruction_count) {
  //   Node n = nodes[current_node];
  //   printf("Name of current node = %s\n", n.name);
  //   if (strncmp(n.name, "ZZZ", 3) == 0) {
  //     printf("Done!\n");
  //     break;
  //   }
  //   if (lines[0][i] == 'L') {
  //     current_node = get_node_by_name(nodes, num_nodes, n.left);
  //   } else {
  //     current_node = get_node_by_name(nodes, num_nodes, n.right);
  //   }
  //   steps_taken++;
  // }
  //
  // printf("Answer to part 1 = %zu\n", steps_taken);
  //
  size_t num_start_points = count_starting_nodes(nodes, num_nodes);
  printf("Number of starting nodes = %zu\n", num_start_points);

  size_t *active_nodes = calloc(num_start_points, sizeof(size_t));
  size_t an_index = 0;
  for (size_t i=0; i<num_nodes; i++) {
    Node n = nodes[i];
    if (n.name[2] == 'A') active_nodes[an_index++] = i;
  }

  size_t *steps = calloc(num_start_points, sizeof(size_t));

  for (size_t i=0; i<num_start_points; i++) {
    size_t steps_taken = 0;
    for (size_t j=0; ; j = (j+1) % instruction_count) {
      char instruction = lines[0][j];
      Node n = nodes[active_nodes[i]];
      if (n.name[2] == 'Z') {
        steps[i] = steps_taken;
        printf("Done with node %zu in %zu steps\n", i, steps_taken);
        break;
      }
      if (instruction == 'L') {
        active_nodes[i] = get_node_by_name(nodes, num_nodes, n.left);
      } else {
        active_nodes[i] = get_node_by_name(nodes, num_nodes, n.right);
      }
      steps_taken++;
    }
  }

  size_t ans = lcm(steps, num_start_points);
  printf("Answer to part 2 = %zu\n", ans);

  return 0;
}

