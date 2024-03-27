#include <stdio.h>
#include <stdlib.h>

// Given a "linked list" where each node contains {data, next} and next points
// to another node, run the Floyd Cycle Detection algorithm below to detect
// whether there is a reachable cycle from the head. You need to print out all
// the data within the nodes that the hare visited before meeting the tortoise.

typedef struct Node {
  int data;
  struct Node *next;
} Node;

Node *create_node(int data) {
  Node *new_node = (Node *)malloc(sizeof(Node *));
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

typedef struct List {
  Node *head;
} List;

int main() {
  int N = 0; // numbers of nodes
  List *a = (List *)malloc(sizeof(List *));
  free(a);
  return 0;
}
