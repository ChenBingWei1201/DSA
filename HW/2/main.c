#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Node {
  int val;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct Deck {
  Node *head;
  Node *tail;
  int size;
} Deck;

Node *createNode(int val) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->val = val;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

Deck *createDeck() {
  Deck *deck = (Deck *)malloc(sizeof(Deck));
  deck->head = NULL;
  deck->tail = NULL;
  deck->size = 0;
  return deck;
}

void push(Deck *deck, int val) {
  Node *node = createNode(val);
  if (deck->size == 0) {
    deck->head = node;
    deck->tail = node;
  } else {
    deck->tail->next = node;
    node->prev = deck->tail;
    deck->tail = node;
  }
  deck->size++;
}

void pop(Deck *deck) {
  if (deck->size == 0) {
    return;
  }
  if (deck->size == 1) {
    free(deck->head);
    deck->head = NULL;
    deck->tail = NULL;
  } else {
    Node *temp = deck->tail;
    deck->tail = deck->tail->prev;
    deck->tail->next = NULL;
    free(temp);
  }
  deck->size--;
}

void move(Deck *deck1, Deck *deck2) {
  if (deck1->size == 0) {
    return;
  }
  if (deck2->size == 0) {
    deck2->head = deck1->head;
    deck2->tail = deck1->tail;
  } else {
    deck2->tail->next = deck1->head;
    deck1->head->prev = deck2->tail;
    deck2->tail = deck1->tail;
  }
  deck2->size += deck1->size;
  deck1->head = NULL;
  deck1->tail = NULL;
  deck1->size = 0;
}

void merge(Deck *deck1, Deck *deck2) {
  if (deck1->size == 0) {
    return;
  }
  if (deck2->size == 0) {
    deck2->head = deck1->head;
    deck2->tail = deck1->tail;
  } else {
    Node *current1 = deck1->tail;
    Node *current2 = deck2->tail;
    Node *next1 = NULL;
    Node *next2 = NULL;

    while (current1 != NULL && current2 != NULL) {
      next1 = current1->prev;
      next2 = current2->prev;

      current1->next = current2->next;
      current1->prev = current2;
      if (current2->next != NULL) {
        current2->next->prev = current1;
      } else {
        deck2->tail = current1;
      }
      current2->next = current1;

      current1 = next1;
      current2 = next2;
    }

    if (current1 != NULL) {
      deck2->head->prev = current1;
      current1->next = deck2->head;
      deck2->head = deck1->head;
    }
  }

  deck2->size += deck1->size;
  deck1->head = NULL;
  deck1->tail = NULL;
  deck1->size = 0;
}

void print_deck(Deck *deck) {
  Node *temp = deck->tail;
  printf("%d ", deck->size);
  while (temp != NULL) {
    printf("%d ", temp->val);
    temp = temp->prev;
  }
  printf("\n");
}

int main() {
  int N, M, **a, *b;
  scanf("%d %d", &N, &M);

  a = (int **)malloc(sizeof(int *) * M); // number of operations
  for (int i = 0; i < M; i++) {
    a[i] = (int *)malloc(sizeof(int) * 3);
    memset(a[i], 0, sizeof(int) * 3);
  }

  b = (int *)malloc(sizeof(int) * M * 3);
  memset(b, 0, sizeof(int) * M * 3);
  for (int i = 0; i < M * 3; i++) {
    scanf("%d", &b[i]);
    if (i % 3 == 0 && b[i] == 2) {
      i++;
    }
  }

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < 3; j++) {
      a[i][j] = b[i * 3 + j];
    }
  }

  Deck **decks = (Deck **)malloc(sizeof(Deck *) * N); // number of decks
  for (int i = 0; i < N; i++) {
    decks[i] = createDeck();
  }

  for (int i = 0; i < M; i++) {
    if (a[i][0] == 1) {
      push(decks[a[i][1] - 1], a[i][2]);
    } else if (a[i][0] == 2) {
      pop(decks[a[i][2] - 1]);
    } else if (a[i][0] == 3) {
      move(decks[a[i][1] - 1], decks[a[i][2] - 1]);
    } else if (a[i][0] == 4) {
      merge(decks[a[i][1] - 1], decks[a[i][2] - 1]);
    }
  }

  for (int i = 0; i < N; i++) {
    print_deck(decks[i]);
  }

  for (int i = 0; i < M; i++) {
    free(a[i]);
  }
  free(a);
  free(b);
  for (int i = 0; i < N; i++) {
    free(decks[i]);
  }
  free(decks);
  return 0;
}
