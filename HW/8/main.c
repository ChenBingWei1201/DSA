#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int data;
  struct Node *next;
} Node;

Node *createNode(int data) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

void floydCycleDetection(Node *head) {
  Node *hare = head;
  Node *tortoise = head;
  while (hare != NULL && hare->next != NULL) {
    printf("%d ", hare->data);
    hare = hare->next->next;
    tortoise = tortoise->next;
    if (hare == tortoise) {
      printf("%d\n", hare->data);
      return;
    }
  }
  if (hare != NULL && hare->next == NULL) {
    printf("%d\n", hare->data);
  }
  return;
}

int main() {
  int N = 0; // numbers of nodes
  scanf("%d", &N);
  Node *nodes[N];
  int nextIndexes[N]; // Store the next indexes here

  for (int i = 0; i < N; i++) {
    int data, next;
    scanf("%d %d", &data, &next);
    nodes[i] = createNode(data);
    nextIndexes[i] = next; // Just store the next index for now
  }

  // Now, all nodes are created. Let's set up the next pointers.
  for (int i = 0; i < N; i++) {
    if (nextIndexes[i] > 0 && nextIndexes[i] <= N) {
      nodes[i]->next = nodes[nextIndexes[i] - 1];
    } else {
      nodes[i]->next = NULL; // Handle invalid next index
    }
  }

  floydCycleDetection(nodes[0]);

  // Free the nodes
  for (int i = 0; i < N; ++i) {
    free(nodes[i]);
  }
  return 0;
}
