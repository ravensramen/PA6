#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct card {
    int face_index;
    int suit_index;
}Card;

typedef struct hand {
    Card hand_array[5];
}Hand;

void shuffle(int wDeck[][13]);

void display_menu();

void deal(Hand* player, int current_player, int wDeck[][13], const char* wFace[], const char* wSuit[]);

void select_cards(Hand* player, Hand* selection_array, int wDeck[][13], const char* wFace[], const char* wSuit[]);

void draw_cards(Hand* player, Hand* selection_array, int wDeck[][13], const char* wFace[], const char* wSuit[]);

void redraw_dealer(Hand* player, Hand* selection_array, int wDeck[][13], const char* wFace[], const char* wSuit[]);

int determine_pair(Hand* player, Hand* selection_array);

int determine_three(Hand* player, Hand* selection_array);

int determine_four(Hand* player, Hand* selection_array);

int determine_flush(Hand* player, Hand* selection_array);

int determine_straight(Hand* player, Hand* selection_array);

void determine_winner(int*winner, int pair_p1, int pair_p2, int three_of_a_kind_p1, int three_of_a_kind_p2, int four_of_a_kind_p1, int four_of_a_kind_p2, int flush_p1, int flush_p2, int straight_p1, int straight_p2);

void display_rules(void);
