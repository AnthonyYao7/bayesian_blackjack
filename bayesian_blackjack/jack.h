#ifndef jack_h
#define jack_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define twenty_one 40

typedef struct {
    uint8_t* counts;
} Deck;

uint8_t card_values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }; // rank value of card from number representation
uint8_t card_indices[] = { 0, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // number representation of card from rank value

void print_deck(Deck deck);
void print_hand(uint8_t hand[21], int cards);
void allocate_deck(Deck* deck);
void copy_deck(Deck deck, Deck* copy);
int hand_value(uint8_t hand[21], int num_cards);
int deck_length(Deck deck);
double kelly(double win, double lose, double odds);
double dealer_win_probability(uint8_t player_hand[21], int player_cards, uint8_t dealer_hand[21], int dealer_cards, Deck deck, double* draw, double* lose);
double win_hand_probability(uint8_t player_hand[21], int player_cards, uint8_t dealer_hand[21], Deck deck, double* draw, double* lose);
double win_probability(Deck deck, double* draw, double* lose);


#endif