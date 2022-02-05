#include "jack.h"

double __dealer_win_probability(uint8_t player_hand[21], int player_cards, uint8_t dealer_hand[21], int dealer_cards, Deck deck, double* draw, double* lose, int recursion_depth) {
	int player_hand_value = hand_value(player_hand, player_cards);
	int dealer_hand_value = hand_value(dealer_hand, dealer_cards);

	// printf("hand_values: %d, %d, recursion_depth: %d\n", player_hand_value, dealer_hand_value, recursion_depth);

	if (player_hand_value == twenty_one) {
		if (dealer_hand_value == twenty_one) {
			*draw = 1.; // push
			*lose = 0.;
			return 0.;
		}
		*draw = 0.;
		*lose = 1.; // dealer looses
		return 0.;
	}

	if (player_hand_value > 21) {
		*draw = 0.;
		*lose = 0.;
		return 1.; // dealer wins
	}

	if (dealer_hand_value == twenty_one) {
		*draw = 0.;
		*lose = 0.;
		return 1.; // dealer wins
	}

	if (dealer_hand_value >= 17) {
		if (dealer_hand_value > player_hand_value) {
			*draw = 0.;
			*lose = 0.;
			return 1.;
		}
		else if (dealer_hand_value < player_hand_value) {
			*draw = 0.;
			*lose = 1.;
			return 0.;
		}
		*draw = 1.;
		*lose = 0.;
		return 0.;
	}

	uint8_t temp_dealer_hand[21];
	int temp_dealer_cards;
	int temp_dealer_hand_value;

	double dealer_win = 0.;
	double dealer_draw = 0.;
	double dealer_lose = 0.;

	double temp_win, temp_draw, temp_lose, draw_card_prob;
	int len_deck = deck_length(deck);

	Deck temp_deck;
	allocate_deck(&temp_deck);
	for (int i = 0; i < 10; i++) {
		// printf("index: %d\n", i);
		if (deck.counts[i] == 0) {
			continue;
		}

		memcpy(temp_dealer_hand, dealer_hand, dealer_cards);
		temp_dealer_cards = dealer_cards;
		temp_dealer_hand[temp_dealer_cards] = i;
		temp_dealer_cards++;
		temp_dealer_hand_value = hand_value(temp_dealer_hand, temp_dealer_cards);

		draw_card_prob = (float)deck.counts[i] / len_deck;
		// printf("card probability: %.7f, card: %d, card counts: %d, len deck: %d\n", draw_card_prob, card_values[i], deck.counts[i], len_deck);

		if (temp_dealer_hand_value > 21) {
			// printf("bust\n");
			dealer_lose += draw_card_prob;
			continue;
		}
		copy_deck(deck, &temp_deck);
		temp_deck.counts[i]--;

		temp_win = __dealer_win_probability(player_hand, player_cards, temp_dealer_hand, temp_dealer_cards, temp_deck, &temp_draw, &temp_lose, recursion_depth + 1);

		// printf("temp probabilities: %.7f, %.7f, %.7f\n", temp_win, temp_draw, temp_lose);

		dealer_win += draw_card_prob * temp_win;
		dealer_draw += draw_card_prob * temp_draw;
		dealer_lose += draw_card_prob * temp_lose;
	}

	*draw = dealer_draw;
	*lose = dealer_lose;
	return dealer_win;

}

double __win_hand_probability(uint8_t player_hand[21], int player_cards, uint8_t dealer_hand[21], Deck deck, double* draw, double* lose) {
	double player_win = 0;
	double player_draw = 0;
	double player_lose = 0;

	double dealer_draw; // draw
	double dealer_lose; // player win
	double dealer_win = dealer_win_probability(player_hand, player_cards, dealer_hand, 2, deck, &dealer_draw, &dealer_lose); // dealer win, player lose

	double hit_dealer_draw;
	double hit_dealer_lose;
	double hit_dealer_win;

	uint8_t temp_player_hand[21];
	memcpy(temp_player_hand, player_hand, player_cards);
	int temp_player_cards = player_cards + 1;
	int temp_player_hand_value;
	double card_prob;
	int len_deck = deck_length(deck);

	Deck temp_deck;
	allocate_deck(&temp_deck);

	for (int i = 0; i < 10; i++) {
		if (deck.counts[i] == 0) {
			continue;
		}
		copy_deck(deck, &temp_deck);
		temp_deck.counts[i]--;

		card_prob = (float)deck.counts[i] / len_deck;

		temp_player_hand[player_cards];
		temp_player_hand_value = hand_value(temp_player_hand, temp_player_cards);
		if (temp_player_hand_value > 21) {
			player_lose += card_prob;
			continue;
		}
		hit_dealer_win = dealer_win_probability(temp_player_hand, temp_player_cards, dealer_hand, 2, temp_deck, &hit_dealer_draw, &hit_dealer_lose);
		player_win += hit_dealer_lose * card_prob;
		player_draw += hit_dealer_draw * card_prob;
		player_lose += hit_dealer_win * card_prob;
	}

	if (player_win >= dealer_lose) {
		// printf("win: %.7f, draw: %.7f, lose: %.7f\n", player_win, player_draw, player_lose);

		*draw = player_draw;
		*lose = player_lose;
		return player_win;
	}
	// printf("win: %.7f, draw: %.7f, lose: %.7f\n", dealer_lose, dealer_draw, lose_on_stay);

	*draw = dealer_draw;
	*lose = dealer_win;
	return dealer_lose;
}