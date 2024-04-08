#include <stdio.h>
#include <stdlib.h>

// Define a structure for each card in the deck
typedef struct Card {
  int num;          // Record the number on the card
  struct Card *nxt; // Pointer to the below card
} Card;

Card *top[100001] = {},
     *bottom[100001] =
         {};           // Record the top card and the bottom card for each deck
int size[100001] = {}; // Record the number of cards for each deck

// Type 1 operation: Add num to the idx-th deck
void Add(int idx, int num) {
  Card *new_card = (Card *)malloc(sizeof(Card));
  new_card->num = num;
  new_card->nxt = top[idx];
  top[idx] = new_card;
  size[idx]++;
  if (size[idx] == 1) {
    // If the idx-th deck was empty, modify the bottom
    bottom[idx] = new_card;
  }
}

// Type 2 operation: Remove a card from the idx-th deck
void Remove(int idx) {
  Card *cur_top = top[idx];
  // Modify the top card of idx-th deck
  top[idx] = cur_top->nxt;
  size[idx]--;
  if (size[idx] == 0) {
    // If the idx-th deck is empty, modify the top and bottom to NULL
    bottom[idx] = NULL;
    top[idx] = NULL;
  }
  free(cur_top);
}

// Type 3 operation: Move the idx1-th deck onto the idx2-th deck
void Move(int idx1, int idx2) {
  if (top[idx1] == NULL)
    return; // Nothing happens if the idx1-th deck is empty
  bottom[idx1]->nxt = top[idx2];
  top[idx2] = top[idx1];
  if (bottom[idx2] == NULL) {
    bottom[idx2] = bottom[idx1];
  }
  size[idx2] += size[idx1];
  // The idx1-th deck is empty, modify the top and bottom to NULL
  top[idx1] = NULL;
  bottom[idx1] = NULL;
  size[idx1] = 0;
}

// Type 4 operation: Merge the idx1-th deck into the idx2-th deck
void Shuffle(int idx1, int idx2) {
  Card *p1 = top[idx1], *p2 = top[idx2];
  Card *new_top = (p1) ? p1 : p2;
  Card *nxt_p1, *nxt_p2;

  while (p1 != NULL && p2 != NULL) {
    // move the top card of the idx1-th deck and the idx2-th deck to the
    // temporary deck p1: the top of the idx1-th deck now p2: the top of the
    // idx2-th deck now
    nxt_p1 = p1->nxt;
    nxt_p2 = p2->nxt;
    p1->nxt = p2;
    if (nxt_p1 != NULL)
      p2->nxt = nxt_p1;
    p1 = nxt_p1;
    p2 = nxt_p2;
  }

  // If there are remaining cards in the idx1-th deck, update the bottom of the
  // idx2-th deck
  if (p1 != NULL) {
    bottom[idx2] = bottom[idx1];
  }

  // Update the size and top of the idx2-th deck after merging
  size[idx2] += size[idx1];
  top[idx2] = new_top;

  // The idx1-th deck is empty, modify the top and bottom to NULL
  bottom[idx1] = NULL;
  top[idx1] = NULL;
  size[idx1] = 0;
}

int main() {
  int n, m;
  scanf("%d%d", &n, &m);
  for (int i = 0; i < m; i++) {
    int op;
    scanf("%d", &op);
    if (op == 1) {
      int idx, num;
      scanf("%d%d", &idx, &num);
      Add(idx, num);
    } else if (op == 2) {
      int idx;
      scanf("%d", &idx);
      Remove(idx);
    } else if (op == 3) {
      int idx1, idx2;
      scanf("%d%d", &idx1, &idx2);
      Move(idx1, idx2);
    } else {
      int idx1, idx2;
      scanf("%d%d", &idx1, &idx2);
      Shuffle(idx1, idx2);
    }
  }

  // Print the final state of each deck
  for (int i = 1; i <= n; i++) {
    printf("%d ", size[i]);
    Card *p = top[i];
    while (p != NULL) {
      printf("%d ", p->num);
      p = p->nxt;
    }
    printf("\n");
  }

  return 0;
}
