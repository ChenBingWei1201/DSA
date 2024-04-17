// max-heap
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  int job_id;
  int priority;
  struct Node *parent;
  struct Node *child;
  struct Node *next_sibling;
  struct Node *prev_sibling;
  int degree;
} Node;

typedef struct BinomialHeap {
  Node *head;
} BinomialHeap;

Node *createNode(int job_id, int priority) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->job_id = job_id;
  newNode->priority = priority;
  newNode->degree = 0;
  newNode->parent = NULL;
  newNode->child = NULL;
  newNode->next_sibling = NULL;
  newNode->prev_sibling = NULL;
  return newNode;
}

BinomialHeap *MakeEmptyBinomialHeap() {
  BinomialHeap *H = (BinomialHeap *)malloc(sizeof(BinomialHeap));
  H->head = NULL;
  return H;
}

// TODO: BinomialHeapMerge(H1, H2)
Node *BinomialHeapMerge(BinomialHeap *H1, BinomialHeap *H2) {
  BinomialHeap *H = MakeEmptyBinomialHeap();
  Node *head = H->head;
  Node *tail = H->head;
  Node *x = H1->head;
  Node *y = H2->head;
  if (x == NULL && y != NULL) {
    return y;
  } else if (x != NULL && y == NULL) {
    return x;
  } else if (x != NULL && y != NULL) {
    while (x != NULL && y != NULL) {
      if (x->degree <= y->degree) {
        if (head == NULL) {
          head = x;
          tail = x;
        } else {
          tail->next_sibling = x;
          x->prev_sibling = tail;
          tail = x;
        }
        x = x->next_sibling;
      } else {
        if (head == NULL) {
          head = y;
          tail = y;
        } else {
          tail->next_sibling = y;
          y->prev_sibling = tail;
          tail = y;
        }
        y = y->next_sibling;
      }
    }
    if (x != NULL) {
      tail->next_sibling = x;
      x->prev_sibling = tail;
    } else if (y != NULL) {
      tail->next_sibling = y;
      y->prev_sibling = tail;
    }
  }
  return head;
}

// TODO: BinomialLink(y, z)
void BinomialLink(Node *y, Node *z) {
  y->parent = z;
  y->next_sibling = z->child;
  if (z->child != NULL) {
    z->child->prev_sibling = y;
  }
  z->child = y;
  z->degree++;
}

// TODO: BinomialHeapUnion(H1, H2)
BinomialHeap *BinomialHeapUnion(BinomialHeap *H1, BinomialHeap *H2) {
  BinomialHeap *H = MakeEmptyBinomialHeap();
  H->head = BinomialHeapMerge(H1, H2);
  if (H->head == NULL) {
    return H;
  }
  Node *prev = NULL;
  Node *x = H->head;
  Node *next = x->next_sibling;
  while (next != NULL) {
    if ((x->degree != next->degree) ||
        (next->next_sibling != NULL &&
         next->next_sibling->degree == x->degree)) {
      prev = x;
      x = next;
    } else {
      if (x->priority > next->priority) {
        x->next_sibling = next->next_sibling;
        if (next->next_sibling != NULL) {
          next->next_sibling->prev_sibling = x;
        }
        // insert next as a child of x, and update x.degree
        BinomialLink(next, x);
      } else {
        if (prev == NULL) {
          H->head = next;
        } else {
          prev->next_sibling = next;
          if (next != NULL) {
            next->prev_sibling = prev;
          }
        }
        // insert x as a child of next, and update next.degree
        BinomialLink(x, next);
        x = next;
      }
    }
    next = x->next_sibling;
  }
  // Node *test = H->head;
  // while (test != NULL) {
  //   printf("hello job_id: %d and priority: %d\n", test->job_id,
  //   test->priority); test = test->child;
  // }
  // free(test);
  return H;
}

// TODO: BinomialHeapInsert(H, x)
// To insert a node x into a binomial heap H, you can create a new binomial heap
// H′ containing only the node x, and then run BinomialHeapUnion(H, H′).
BinomialHeap *BinomialHeapInsert(BinomialHeap *H, Node *x) {
  BinomialHeap *H1 = MakeEmptyBinomialHeap();
  x->parent = NULL;
  x->child = NULL;
  x->next_sibling = NULL;
  x->prev_sibling = NULL;
  x->degree = 0;
  H1->head = x;
  return BinomialHeapUnion(H, H1);
}

// TODO: BinomialHeapExtractMAX(H)
// Extracting the node with minimum key
// The following procedure extracts the node with the maximum key from binomial
// heap H and returns a pointer to the extracted node.
// BINOMIAL-HEAP-EXTRACT-MAX(H)
// 1 find the root x with the maximum key in the root list of H, and remove x
// from the root list of H
// 2 H1= MAKE-BINOMIAL-HEAP()
// 3 reverse the order of the linked list of x’s children, and set H1.head to
// point to the head of the resulting list
// 4 H = BINOMIAL-HEAP-UNION(H, H1)
// 5 return x

void ReverseChildren(Node *node) {
  Node *current = node->child;
  node->child = NULL;
  Node *prev = NULL;

  while (current != NULL) {
    Node *next = current->next_sibling;
    current->next_sibling = prev;
    current->prev_sibling = next;
    prev = current;
    current = next;
  }

  node->child = prev;
}

Node *FindMax(BinomialHeap *H) {
  if (H->head == NULL) {
    return NULL;
  }
  Node *max = H->head;
  Node *current = H->head->next_sibling;
  while (current != NULL) {
    if (current->priority > max->priority) {
      max = current;
    }
    current = current->next_sibling;
  }
  return max;
}

void RemoveNode(BinomialHeap *H, Node *nodeToRemove) {
  if (nodeToRemove == H->head) {
    H->head = nodeToRemove->next_sibling;
    if (H->head != NULL) {
      H->head->prev_sibling = NULL;
    }
  } else {
    Node *prev = nodeToRemove->prev_sibling;
    prev->next_sibling = nodeToRemove->next_sibling;
    if (nodeToRemove->next_sibling != NULL) {
      nodeToRemove->next_sibling->prev_sibling = prev;
    }
  }
  nodeToRemove->next_sibling = NULL;
  nodeToRemove->prev_sibling = NULL;
}

Node *BinomialHeapExtractMax(BinomialHeap **H) {
  if ((*H)->head == NULL) {
    return NULL;
  }
  Node *max = FindMax(*H);
  RemoveNode(*H, max);
  ReverseChildren(max);
  BinomialHeap *H1 = MakeEmptyBinomialHeap();
  H1->head = max->child;
  *H = BinomialHeapUnion(*H, H1);
  max->child = NULL;
  return max;
}

// TODO: BinomialHeapMove(H1, H2)
// Transfer all jobs from printer printer_id1 to printer printer_id2,
// maintaining their respective priorities.
BinomialHeap *BinomialHeapMove(BinomialHeap *H1, BinomialHeap *H2) {
  H2 = BinomialHeapUnion(H1, H2);
  H1->head = NULL;
  return H2;
}

int main() {
  int N, M, *num;
  scanf("%d %d", &N, &M);
  BinomialHeap **H = (BinomialHeap **)malloc(sizeof(BinomialHeap *) * N);
  num = (int *)malloc(sizeof(int) * N);
  memset(num, 0, sizeof(int) * N);
  for (int i = 0; i < N; i++) {
    H[i] = MakeEmptyBinomialHeap();
  }
  for (int i = 0; i < M; i++) {
    int opt;
    scanf("%d", &opt);
    if (opt == 1) {
      int job_id, priority, printer_id;
      scanf("%d %d %d", &job_id, &priority, &printer_id);
      Node *newNode = createNode(job_id, priority);
      H[printer_id - 1] = BinomialHeapInsert(H[printer_id - 1], newNode);
      printf("%d jobs waiting on printer %d\n", ++num[printer_id - 1],
             printer_id);
    } else if (opt == 2) {
      int printer_id;
      scanf("%d", &printer_id);
      Node *max = BinomialHeapExtractMax(&H[printer_id - 1]);
      if (max == NULL) {
        printf("no documents in queue\n");
      } else {
        printf("%d printed\n", max->job_id);
        num[printer_id - 1]--;
        free(max);
      }
    } else if (opt == 3) {
      int printer_id1, printer_id2;
      scanf("%d %d", &printer_id1, &printer_id2);
      H[printer_id2 - 1] =
          BinomialHeapMove(H[printer_id1 - 1], H[printer_id2 - 1]);
      num[printer_id2 - 1] += num[printer_id1 - 1];
      num[printer_id1 - 1] = 0;
      printf("%d jobs waiting on printer %d after moving\n",
             num[printer_id2 - 1], printer_id2);
    }
  }

  for (int i = 0; i < N; i++) {
    free(H[i]);
  }
  free(H);
  free(num);
  return 0;
}
