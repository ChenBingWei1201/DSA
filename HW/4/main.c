#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  long long value;
  struct Node *next;
  struct Node *below;
} Node;

typedef struct SkipList {
  Node *head;
  int fastLayers;
} SkipList;

Node *create_node(long long value) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->value = value;
  node->next = NULL;
  node->below = NULL;
  return node;
}

SkipList *create_skip_list() {
  SkipList *list = (SkipList *)malloc(sizeof(SkipList));
  list->head = create_node(-1); // Sentinel node
  list->fastLayers = 0;
  return list;
}

bool is_odd(long long k) { return k & 1; }

bool coin_flip(long long k, int i) { return is_odd(k >> (i - 1)); }

Node *slow_get(SkipList *L, long long data) {
  Node *node = L->head;
  while (node->below != NULL)
    node = node->below; // Go to bottom layer
  while (node->next != NULL && data <= node->next->value) {
    printf("%lld ", node->next->value);
    node = node->next;
  }
  if (node->value >= data)
    return node;
  else
    return NULL;
}

Node *fast_get(SkipList *L, long long data) {
  Node *node = L->head;
  while (node->below != NULL) {
    while (node->next != NULL && data <= node->next->value) {
      printf("%lld ", node->next->value);
      node = node->next;
    }
    if (node->below->value != -1) {
      printf("%lld ", node->below->value);
    }
    node = node->below;
  }
  while (node->next != NULL && data <= node->next->value) {
    printf("%lld ", node->next->value);
    node = node->next;
  }
  if (node->value >= data)
    return node;
  else
    return NULL;
}

/**
 insert(L, data)
 Insertion can be done by first taking fast_get while storing the “previous”
 node of each layer. Then, one can insert the new node after the “previous”
 node of each layer probabilistically. Note that if a node is inserted into
 the top layer, then an additional coin flipping is needed to see if one more
 layer should be added. To make insertion deterministic (which facilitates the
 judge system to do grading), please use the coin_flip() routine to
 determine whether a number k should be inserted into the i-th layer for i > 0.
*/

void insert(SkipList *L, long long k) {
  Node *prev[80] = {NULL}; // Initialize all elements to NULL
  Node *temp = L->head;
  int level = 0;

  // Traverse down the skip list to collect the previous nodes at each level
  for (int i = L->fastLayers; i >= 0; i--) {
    while (temp->next != NULL && temp->next->value >= k) {
      temp = temp->next;
    }
    prev[i] = temp; // Store the node before the insertion point at this level
    if (temp->below != NULL) {
      temp = temp->below; // Move down a level
    }
  }

  // Insert the new node at the bottom level
  Node *newNode = create_node(k);
  newNode->next = prev[0]->next;
  prev[0]->next = newNode;

  // Probabilistically build upwards
  while (coin_flip(k, ++level)) {
    if (level > L->fastLayers) {
      // Add a new layer
      Node *newHead = create_node(-1);
      newHead->below = L->head;
      L->head = newHead;
      L->fastLayers++;
      prev[level] = newHead;
    }

    // Insert a new node at this level
    Node *newUpperNode = create_node(k);
    newUpperNode->next = prev[level]->next;
    newUpperNode->below = newNode;
    prev[level]->next = newUpperNode;
    newNode = newUpperNode; // Move up for the next iteration
  }
}

/*
  remove_node(L, data)
  Removal can be done by first taking fast_get and removing the target nodes (in
  all layers) once they have been found. Note that if all nodes in the top layer
  are removed, remember to remove the top layer by updating L->fastLayers.
*/

void remove_node(SkipList *L, long long k) {
  Node *prev[80] = {NULL}; // Initialize all elements to NULL
  Node *temp = L->head;

  // Traverse down the skip list to collect the previous nodes at each level
  for (int i = L->fastLayers; i >= 0; i--) {
    while (temp->next != NULL && temp->next->value > k) {
      temp = temp->next;
    }
    prev[i] = temp; // Store the node before the insertion point at this level
    if (temp->below != NULL) {
      temp = temp->below; // Move down a level
    }
  }

  // Remove the target node at the bottom level
  if (prev[0]->next != NULL && prev[0]->next->value == k) {
    Node *target = prev[0]->next;
    prev[0]->next = target->next;
    free(target);
  }

  // Remove the target node at the upper levels
  for (int i = 1; i <= L->fastLayers; i++) {
    if (prev[i]->next != NULL && prev[i]->next->value == k) {
      Node *target = prev[i]->next;
      prev[i]->next = target->next;
      free(target);
    }
  }

  // Remove the top layer if all nodes are removed
  while (L->fastLayers > 0 && L->head->next == NULL) {
    Node *temp = L->head;
    L->head = L->head->below;
    free(temp);
    L->fastLayers--;
  }
}

int main() {
  long long M;
  long long *a;
  scanf("%lld", &M);
  a = (long long *)malloc(sizeof(long long) * M * 2LL);
  for (int i = 0; i < M * 2; i++) {
    scanf("%lld", &a[i]);
  } // even index for Type, odd index for Key
  SkipList *list = create_skip_list();
  for (int i = 0; i < M * 2; i += 2) {
    if (a[i] == 1) {
      Node *node = slow_get(list, a[i + 1]);
      if (node == NULL)
        printf("-1");
      printf("\n");
    } else if (a[i] == 2) {
      Node *node = fast_get(list, a[i + 1]);
      if (node == NULL)
        printf("-1");
      printf("\n");
    } else if (a[i] == 3) {
      insert(list, a[i + 1]);
    } else if (a[i] == 4) {
      remove_node(list, a[i + 1]);
    }
  };
  free(list);
  return 0;
}
