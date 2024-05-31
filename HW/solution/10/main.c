#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1000001
#define MAX_M 1000001

typedef struct binomial_node {
  int key;    // priority of the job
  int id;     // job id
  int degree; // degree of the binomial heap
  struct binomial_node *parent;
  struct binomial_node *child; // pointer of the first child
  struct binomial_node *sibling;
} BinomialNode;

BinomialNode *MakeBinomialHeap() { return NULL; }

int BinomialHeapMaximum(
    BinomialNode *H) {           // find the maximun of the binomial heap `H`
  BinomialNode *temp = H;        // temp node to traversal the binomial heap
  BinomialNode *max_node = NULL; // the node with maximum
  int max = -1;                  // record the maximum key
  while (temp != NULL) {
    if (temp->key > max) {
      max_node = temp;
      max = temp->key;
    }
    temp = temp->sibling;
  }
  return max;
}

// Make binomial heap `y` be the first child of `z`
void BinomialLink(BinomialNode *y, BinomialNode *z) {
  y->parent = z;
  y->sibling = z->child;
  z->child = y;
  z->degree += 1;
  return;
}

// Merge binomial heap `H1`, `H2` into an ordered linked list (sorted by degree)
BinomialNode *BinomialHeapMerge(BinomialNode *H1, BinomialNode *H2) {
  if (H1 == NULL)
    return H2;
  if (H2 == NULL)
    return H1;

  BinomialNode *head = NULL;
  BinomialNode *tail = NULL;
  BinomialNode *temp;

  while (H1 != NULL && H2 != NULL) {
    // `temp` = `H1` if the degree of `H1` is smaller
    if (H1->degree <= H2->degree) {
      temp = H1;
      H1 = H1->sibling;
    }
    // `temp` = `H2` if the degree of `H2` is smaller
    else {
      temp = H2;
      H2 = H2->sibling;
    }

    // add `temp` behind the tail
    if (head == NULL) {
      head = temp;
      tail = temp;
    } else {
      tail->sibling = temp;
      tail = temp;
    }
  }

  if (H1 != NULL)
    tail->sibling = H1;
  else
    tail->sibling = H2;

  return head;
}

// Union the two binomial heap `H1`,`H2`
BinomialNode *BinomialHeapUnion(BinomialNode *H1, BinomialNode *H2) {
  BinomialNode *H = BinomialHeapMerge(H1, H2);
  if (H == NULL)
    return H;
  BinomialNode *prev = NULL;          // previous node
  BinomialNode *curr = H;             // current node
  BinomialNode *next = curr->sibling; // next node
  while (next != NULL) {
    if ((curr->degree != next->degree) ||
        (next->sibling != NULL && curr->degree == next->sibling->degree)) {
      prev = curr;
      curr = next;
    } else {
      if (curr->key > next->key) {
        curr->sibling = next->sibling;
        BinomialLink(next, curr);
      } else {
        if (prev == NULL) {
          H = next;
        } else {
          prev->sibling = next;
        }
        BinomialLink(curr, next);
        curr = next;
      }
    }
    next = curr->sibling;
  }
  return H;
}

// Insert a job with (job_id, priority) = (`id`, `key`) to the binomial heap `H`
BinomialNode *BinomialHeapInsert(BinomialNode *H, int key, int id) {
  BinomialNode *new_node = (BinomialNode *)malloc(sizeof(BinomialNode));
  new_node->key = key;
  new_node->id = id;
  new_node->degree = 0;
  new_node->parent = NULL;
  new_node->child = NULL;
  new_node->sibling = NULL;
  H = BinomialHeapUnion(H, new_node);
  return H;
}

// Remove the node with maximum key in binomial heap `H`
BinomialNode *BinomialHeapExtractMax(BinomialNode *H) {
  if (H == NULL) {
    printf("no documents in queue\n");
    return H;
  }
  BinomialNode *temp = H;
  BinomialNode *prev = NULL;
  BinomialNode *max_node = NULL; // the node with maximum key
  BinomialNode *max_prev = NULL; // the node before max_node
  int max = -1;                  // record the maximum key
  while (temp != NULL) {
    if (temp->key > max) {
      max_prev = prev;
      max_node = temp;
      max = temp->key;
    }
    prev = temp;
    temp = temp->sibling;
  }

  printf("%d printed\n", max_node->id);

  // remove max_node from the root list of H
  if (max_prev != NULL)
    max_prev->sibling = max_node->sibling;
  else
    H = H->sibling;

  // reverse child linked list of min_node
  prev = NULL;
  BinomialNode *curr = max_node->child;
  BinomialNode *next = NULL;

  while (curr != NULL) {
    next = curr->sibling;
    curr->sibling = prev;
    prev = curr;
    curr = next;
  }

  max_node->child = prev;

  H = BinomialHeapUnion(H, max_node->child);
  return H;
}

typedef struct printer {
  BinomialNode *heap; // each printer is a binomial heap
  int count;          // how many jobs in this printer
} Printer;

Printer P[MAX_N];

int main() {

  int N, M;
  scanf("%d%d", &N, &M);
  for (int i = 1; i <= N; i++) {
    P[i].heap = MakeBinomialHeap();
    P[i].count = 0;
  }
  for (int i = 0; i < M; i++) {
    int op = 0;
    scanf("%d", &op);
    if (op == 1) {
      int job_id, priority, printer_id;
      scanf("%d%d%d", &job_id, &priority, &printer_id);
      P[printer_id].heap =
          BinomialHeapInsert(P[printer_id].heap, priority, job_id);
      P[printer_id].count += 1;
      printf("%d jobs waiting on printer %d\n", P[printer_id].count,
             printer_id);
    } else if (op == 2) {
      int printer_id;
      scanf("%d", &printer_id);
      P[printer_id].heap = BinomialHeapExtractMax(P[printer_id].heap);
      if (P[printer_id].count > 0)
        P[printer_id].count -= 1;
    } else {
      int printer_id1, printer_id2;
      scanf("%d%d", &printer_id1, &printer_id2);
      P[printer_id2].heap =
          BinomialHeapUnion(P[printer_id1].heap, P[printer_id2].heap);
      P[printer_id1].heap = MakeBinomialHeap();
      P[printer_id2].count += P[printer_id1].count;
      P[printer_id1].count = 0;
      printf("%d jobs waiting on printer %d after moving\n",
             P[printer_id2].count, printer_id2);
    }
  }
  return 0;
}
