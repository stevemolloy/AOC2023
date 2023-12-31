#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_lib.h"

typedef struct {
  char *hand;
  size_t bid;
} Play;

typedef enum {
  HIGH_CARD = 0,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_KIND,
  FULL_HOUSE,
  FOUR_KIND,
  FIVE_KIND,
} Type;

char *type_string(Type t) {
  switch (t) {
    case HIGH_CARD:     return "HIGH_CARD";
    case ONE_PAIR:      return "ONE_PAIR";
    case TWO_PAIR:      return "TWO_PAIR";
    case THREE_OF_KIND: return "THREE_OF_KIND";
    case FULL_HOUSE:    return "FULL_HOUSE";
    case FOUR_KIND:     return "FOUR_KIND";
    case FIVE_KIND:     return "FIVE_KIND";
  }
}

size_t convert_card(char card) {
  if (isdigit(card)) return card - '0';
  else if (card == 'T') return 10;
  else if (card == 'J') return 11;
  else if (card == 'Q') return 12;
  else if (card == 'K') return 13;
  else if (card == 'A') return 14;
  else {
    fprintf(stderr, "Unreachable\n");
    exit(1);
  }
}

size_t convert_card_with_jokers(char card) {
  if (isdigit(card)) return card - '0';
  else if (card == 'T') return 10;
  else if (card == 'J') return 0;
  else if (card == 'Q') return 12;
  else if (card == 'K') return 13;
  else if (card == 'A') return 14;
  else {
    fprintf(stderr, "Unreachable\n");
    exit(1);
  }
}

Type get_hand_type(Play play) {
  size_t counts[15] = {0};
  for (size_t i=0; i<5; i++) {
    counts[convert_card(play.hand[i])]++;
  }

  bool found_a_pair = false;
  bool found_a_three = false;
  for (size_t c=1; c<15; c++) {
    if (counts[c] == 5)                       return FIVE_KIND;
    else if (counts[c] == 4)                  return FOUR_KIND;
    else if (counts[c] == 3)                  found_a_three = true;
    else if (counts[c] == 2 && found_a_pair)  return TWO_PAIR;
    else if (counts[c] == 2 && found_a_three) return FULL_HOUSE;
    else if (counts[c] == 2)                  found_a_pair = true;
  }
  if (found_a_pair && found_a_three)          return FULL_HOUSE;
  else if (found_a_pair)                      return ONE_PAIR;
  else if (found_a_three)                     return THREE_OF_KIND;

  return HIGH_CARD;
}

Type get_hand_type_with_jokers(Play play) {
  size_t counts[15] = {0};
  for (size_t i=0; i<5; i++) {
    size_t card_val = convert_card_with_jokers(play.hand[i]);
    counts[card_val]++;
  }

  Type result = HIGH_CARD;
  size_t pairs_found = 0;
  bool found_a_three = false;
  for (size_t c=1; c<15; c++) {
    if (counts[c] == 5) {
      result = FIVE_KIND;
      break;
    } else if (counts[c] == 4) {
      result = FOUR_KIND;
      break;
    } else if (counts[c] == 3) found_a_three = true;
    else if (counts[c] == 2 && pairs_found == 1) {
      result = TWO_PAIR;
      pairs_found++;
      break;
    } else if (counts[c] == 2 && found_a_three) {
      result = FULL_HOUSE;
      break;
    } else if (counts[c] == 2) pairs_found = 1;
  }
  if (pairs_found==1 && found_a_three) result = FULL_HOUSE;
  else if (pairs_found == 1) result = ONE_PAIR;
  else if (found_a_three && result != FULL_HOUSE) result = THREE_OF_KIND;

  if (counts[0] == 0) return result;
  else if (counts[0] == 1) {
    switch (result) {
      case HIGH_CARD:
        result = ONE_PAIR;
				break;
      case ONE_PAIR:
        result = THREE_OF_KIND;
				break;
      case TWO_PAIR:
        result = FULL_HOUSE;
				break;
      case THREE_OF_KIND:
        result = FOUR_KIND;
				break;
      case FOUR_KIND:
        result = FIVE_KIND;
				break;
      case FULL_HOUSE:
      case FIVE_KIND:
        assert(0 && "Unreachable");
				break;
    }
  } else if (counts[0] == 2) {
    switch (result) {
      case HIGH_CARD:
        result = THREE_OF_KIND;
				break;
      case ONE_PAIR:
        result = FOUR_KIND;
				break;
      case THREE_OF_KIND:
        result = FIVE_KIND;
				break;
      case TWO_PAIR:
      case FULL_HOUSE:
      case FOUR_KIND:
      case FIVE_KIND:
        assert(0 && "Unreachable");
				break;
    }
  } else if (counts[0] == 3) {
    switch (result) {
      case HIGH_CARD:
        result = FOUR_KIND;
				break;
      case ONE_PAIR:
        result = FIVE_KIND;
				break;
      case TWO_PAIR:
      case THREE_OF_KIND:
      case FULL_HOUSE:
      case FOUR_KIND:
      case FIVE_KIND:
        assert(0 && "Unreachable");
				break;
    }
  } else if (counts[0] == 4) {
    switch (result) {
      case HIGH_CARD:
        result = FIVE_KIND;
				break;
      case ONE_PAIR:
      case TWO_PAIR:
      case THREE_OF_KIND:
      case FULL_HOUSE:
      case FOUR_KIND:
      case FIVE_KIND:
        assert(0 && "Unreachable");
				break;
    }
  } else if (counts[0] == 5) {
    result = FIVE_KIND;
  }

  return result;
}

int compare_hands(const void* a, const void* b) { 
  Play play_1 = *(Play*)a;
  Play play_2 = *(Play*)b;

  Type hand_1_type = get_hand_type(play_1);
  Type hand_2_type = get_hand_type(play_2);

  if (hand_1_type != hand_2_type) {
    return hand_1_type - hand_2_type;
  }
  for (size_t c=0; c<5; c++) {
    size_t a_val = convert_card(play_1.hand[c]);
    size_t b_val = convert_card(play_2.hand[c]);
    if (a_val != b_val) return a_val - b_val;
  }

  return 0;
}

int compare_hands_with_jokers(const void* a, const void* b) { 
  Play play_1 = *(Play*)a;
  Play play_2 = *(Play*)b;

  Type hand_1_type = get_hand_type_with_jokers(play_1);
  Type hand_2_type = get_hand_type_with_jokers(play_2);

  if (hand_1_type != hand_2_type) {
    return hand_1_type - hand_2_type;
  }

  for (size_t c=0; c<5; c++) {
    size_t a_val = convert_card_with_jokers(play_1.hand[c]);
    size_t b_val = convert_card_with_jokers(play_2.hand[c]);
    if (a_val != b_val) return a_val - b_val;
  }

  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Please provide a single input -- the file with the data.\n");
    return 1;
  }

  char *buffer;
  char **lines;
  size_t num_lines = read_entire_file_to_lines(argv[1], &buffer, &lines);

  Play *plays = calloc(num_lines, sizeof(Play));

  for (size_t l=0; l<num_lines; l++) {
    char *cursor = lines[l];
    plays[l].hand = cursor;
    advance_to_char(&cursor, ' ');
    *cursor = '\0';
    cursor++;
    plays[l].bid = get_next_val_from_string(&cursor);
  }

  size_t total;

  qsort(plays, num_lines, sizeof(Play), compare_hands);
  total = 0;
  for (size_t i=0; i<num_lines; i++) {
    size_t rank = i + 1;
    total += plays[i].bid * rank;
  }
  printf("Answer to part 1 = %zu\n", total);

  qsort(plays, num_lines, sizeof(Play), compare_hands_with_jokers);
  total = 0;
  for (size_t i=0; i<num_lines; i++) {
    size_t rank = i + 1;
    total += plays[i].bid * rank;
  }
  printf("Answer to part 2 = %zu\n", total);

  free(plays);
  free(lines);
  free(buffer);

  return 0;
}

