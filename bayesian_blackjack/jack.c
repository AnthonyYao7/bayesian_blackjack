#include "jack.h"

typedef struct {
	uint8_t* counts;
} Deck;

void print_deck(Deck deck) {
	for (size_t i = 0; i < 10; i++) {
		printf("%u: %u\n", i + 2, deck.counts[i]);
	}
}

void allocate_deck(Deck* deck) {
	deck->counts = (uint8_t*)malloc(10 * sizeof(uint8_t));
}

void create_copy(Deck deck, Deck* copy) {
	allocate_deck(copy);
	memcpy(copy->counts, deck.counts, sizeof(uint8_t) * 10);
}

int deck_length(Deck deck) {
	int ret = 0;
	for (size_t i = 0; i < 10; i++) {
		ret += deck.counts[i];
	}
	return ret;
}

float win_probability(Deck deck) {
	float win_prob = 0;
	float draw_prob = 0;
	float lose_prob = 0;

	int len_deck = deck_length(deck);

	uint8_t hand[4] = { 0, 0, 0, 0 };
	Deck ilevel;
	Deck jlevel;
	Deck klevel;
	Deck llevel;
	int len_ilevel;
	int len_jlevel;
	int len_klevel;
	int len_llevel;
	float hand_prob;

	for (size_t i = 0; i < 10; i++) {
		if (deck.counts[i] == 0) // ignore when there are none of those cards left
			continue;
		
		create_copy(deck, &ilevel); // create copy of deck with the first card drawn as i, the iterator if it isn't zero
		ilevel.counts[i]--; // decrement the amount of that card in the deck by one
		hand[0] = i; // set the first card in the hand to that card, i
		len_ilevel = deck_length(ilevel);

		for (size_t j = 0; i < 10; i++) {
			if (ilevel.counts[j] == 0)
				continue;
			
			create_copy(ilevel, &jlevel);
			jlevel.counts[j]--;
			hand[1] = j;
			len_jlevel = deck_length(jlevel);

			for (size_t k = 0; i < 10; i++) {
				if (jlevel.counts[k] == 0)
					continue;
				
				create_copy(jlevel, &klevel);
				klevel.counts[k]--;
				hand[2] = k;
				len_klevel = deck_length(klevel);

				for (size_t l = 0; i < 10; i++) {
					if (klevel.counts[l] == 0)
						continue;
					
					create_copy(klevel, &llevel);
					llevel.counts[l]--;
					hand[3] = l;
					len_llevel = deck_length(llevel);

					/* the interesting stuff starts now */

					// calculate the probability of this hand
					hand_prob = deck.counts[i] * ilevel.counts[j] * jlevel.counts[k] * klevel.counts[l] / (len_deck * len_ilevel * len_jlevel * len_klevel);
					
				}
			}
		}
	}

}

int main() {
	uint8_t counts[] = { 4, 4, 4, 4, 4, 4, 4, 4, 16, 4 };

	Deck deck;
	deck.counts = counts;

	print_deck(deck);

}