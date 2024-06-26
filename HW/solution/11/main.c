/*
This is the main solution (readability enhanced version, REV) that will receive
120 pts. The problem is based on traversing through a tree and conducting some
operations. Note that operation 3, 4, 5 are basically three independent problems
and operation 1, 2 are the basics of tree traversal. (Operation 5 is kinda based
on operation 3 but the core of operation 5 is not.) The bonus operation 6 only
really affects operation 4 and my implementation leads to operation 3, 5 being
completely unaffected. However, depending on the approach, you might need to
modify some code to support operation 3, 5 in bonus.

Anyway, enjoy more than 200 lines of code!
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N (int)(1e6 + 10)

// Declaration of some structs that are going to be used

// The stack is an array-based stack that allows random-access to the current
// dfs stack.
int stack[MAX_N];

// This array stores the depth of each layer.
// Each layer is a node visited and stored in the previous stack and depth is
// the distance to root.
long long depth[MAX_N];

// This is the node to store the children of each parent node
typedef struct child_queue_node {
  struct child_queue_node *nxt; // The next child
  int c, d; // The index of node of this child, the distance to that node
} ChildQueueNode;

ChildQueueNode *head[MAX_N], *tail[MAX_N];

ChildQueueNode *newChild(int c, int d) {
  ChildQueueNode *t = (ChildQueueNode *)malloc(sizeof(ChildQueueNode));
  t->nxt = NULL; // Newest child is always the last in the queue so it is
                 // initialized as NULL
  t->c = c;
  t->d = d;
  return t;
};

// This is the node of queues that are created to deal with operation 5
typedef struct treasure_node {
  struct treasure_node *prv, *nxt; // Doubly-linked list is required
  int neg_d;   // The position that the value of the treasure becomes negative
  long long v; // The remaining value of the treasure
} TreasureNode;

TreasureNode *newQueueNode(TreasureNode *oriHead, int neg_d, long long v) {
  TreasureNode *t = (TreasureNode *)malloc(sizeof(TreasureNode));
  t->nxt =
      oriHead; // The new treasure is always pushed into the head of the queue
  t->neg_d = neg_d;
  t->v = v;
  return t;
}

// This is the queue of treasures since there may be multiple queues
// Note that it is actually a deque. However, I found it easier to understand by
// regarding it as a queue since the push_front() here is releatively
// irrelevant. If you find it weird that a queue have pop_front(), then you are
// actually correct. :D
typedef struct queue_of_treasure {
  TreasureNode *head, *tail; // The head and tail nodes
  int top, bot;              // As a queue of treasures
} TreasureQueue;

TreasureQueue *cur_queue[MAX_N]; // This array of pointer stores which queue
                                 // each node belongs to

TreasureQueue *newQueue(int neg_d, long long v, int dep) {
  TreasureQueue *t = (TreasureQueue *)malloc(sizeof(TreasureQueue));
  t->bot = t->top = dep;
  t->head = t->tail = newQueueNode(NULL, neg_d, v);
  t->head->nxt = t->head->prv = NULL;
  return t;
}

// Popping from the head (front) of a queue
void pop_front(TreasureQueue *q) {
  // If the head is the same as tail -> queue only consists of one node ->
  // delete the entire queue
  if (q->head == q->tail) {
    free(q->head);
    cur_queue[q->bot] = NULL;
    free(q);
  }
  // Remove the head
  else {
    q->head = q->head->nxt;
    free(q->head->prv);
    q->head->prv = NULL;

    cur_queue[q->bot--] = NULL;
    cur_queue[q->bot] = q;
  }
}

// Popping from the tail (back) of a queue
// Note that it is very similar to pop_front()
void pop_back(TreasureQueue *q) {
  if (q->head == q->tail) {
    free(q->head);
    cur_queue[q->bot] = NULL;
    free(q);
  } else {
    q->tail = q->tail->prv;
    free(q->tail->nxt);
    q->tail->nxt = NULL;
  }
}

// Pushing to the head (front) of the queue
// This function is complicated since we may result in merging queues.
void push_front(TreasureQueue *q, int neg_d, long long v) {
  // Extend the queue by moving the top upwards by one.
  --(q->top);
  q->head = newQueueNode(q->head, neg_d, v);
  q->head->nxt->prv = q->head;

  // If the top of queue reached root, print the treasure on top.
  if (q->top == 0) {
    if (q->tail->v >= 0)
      printf("value remaining is %lld\n", q->tail->v);
    else
      printf("value lost at %d\n", q->tail->neg_d);
    pop_back(q);
    ++q->top;
  }
  // If the top of queue reached the bottom of another queue, merge the two
  // queues. The implementation of this part may have multiple different
  // approaches. The one provided below may not be the cleanest nor the
  // simplest. It is just the one that I thought of when writing the main
  // solution.
  else if (cur_queue[q->top] != NULL) {
    // First, push the top of current queue to the upper queue
    push_front(cur_queue[q->top], q->tail->neg_d, q->tail->v);
    // Then, pop that node out.
    pop_back(q);
    // Finally, some pointer operations to merge the queues.
    q->tail->nxt = cur_queue[q->top]->head;
    cur_queue[q->top]->head->prv = q->tail;
    q->tail = cur_queue[q->top]->tail;
    q->top = cur_queue[q->top]->top;
    cur_queue[q->top] = q;
    // Note that cur_queue[] array is not properly modified since that will
    // require O(N) time. The reason that it is not required is left as an
    // exercise to the reader. :D
  }
  // The easiest part! Hurray!!!
  else
    cur_queue[q->top] = q;
}

// This part is the node of deques to solve the bonus version of operation 4
// The bonus part of operation 4 is based on a classic problem: sliding window
// maximum https://leetcode.com/problems/sliding-window-maximum The deque below
// is the sliding window maximum deque with additional metadata (t -> c) to
// determine the popping time. The rest is a normal implementation of the
// classic problem.
typedef struct max_child_deque_node {
  int c;
  long long mxv;
  struct max_child_deque_node *prv, *nxt;
} MaxChildDequeNode;

MaxChildDequeNode *mxc_head[MAX_N], *mxc_tail[MAX_N];

MaxChildDequeNode *newMaxChildNode(int c, long long v, MaxChildDequeNode *prv) {
  MaxChildDequeNode *t = (MaxChildDequeNode *)malloc(sizeof(MaxChildDequeNode));
  t->c = c;
  t->mxv = v;
  t->prv = prv;
  t->nxt = NULL;
  return t;
}

// Though named as updatemx(), it is simply popping all the element less than v,
// and pushing the new element to the end of deque.
void updatemx(int i, int c, long long v) {
  // The popping part. Although this may appears O(N), the total count of
  // popping is bounded to the elements pushed so the time required is O(N)
  // throughout the entire lifespan of a deque and O(N)/N = O(1) per update.
  while (mxc_tail[i] != NULL && v > mxc_tail[i]->mxv) {
    MaxChildDequeNode *tmp = mxc_tail[i];
    mxc_tail[i] = mxc_tail[i]->prv;
    free(tmp);
  }

  // Finally, push into the deque.
  if (mxc_tail[i] == NULL) {
    mxc_head[i] = mxc_tail[i] = newMaxChildNode(c, v, NULL);
  } else {
    mxc_tail[i]->nxt = newMaxChildNode(c, v, mxc_tail[i]);
    mxc_tail[i] = mxc_tail[i]->nxt;
  }
}

// dfs() to find the furthest leaf.
long long dfs(int x) {
  for (ChildQueueNode *c = head[x]; c != NULL; c = c->nxt) {
    // For each child, find the deepest leaf within the subtree and add that
    // with the distance to the child. Update the value into the deque.
    updatemx(x, c->c, c->d + dfs(c->c));
  }

  // Return the furthest leaf in this subtree.
  if (mxc_head[x] == NULL)
    return 0; // The furthest leaf can be root and thus a distance of 0.
  else
    return mxc_head[x]->mxv;
}

// A nice, simple, classic, elegant binary search on a stack.
int bin_search(long long x, int cur_layer) {
  int l = 0, r = cur_layer;
  while (l != r) {
    int m = (l + r) / 2;
    if (depth[m] >= x)
      r = m;
    else
      l = m + 1;
  }
  return l;
}

int main() {
  int n, m, q;

  scanf("%d%d%d", &n, &m, &q);

  // Initializtion
  for (int i = 0; i < n; ++i) {
    cur_queue[i] = NULL;
    head[i] = tail[i] = NULL;
    mxc_head[i] = mxc_tail[i] = NULL;
  }

  // Input of the initial edges.
  while (m--) {
    int u, v, w;

    scanf("%d%d%d", &u, &v, &w);
    if (head[u] == NULL) {
      head[u] = tail[u] = newChild(v, w);
    } else {
      tail[u]->nxt = newChild(v, w);
      tail[u] = tail[u]->nxt;
    }
  }

  // Note that dfs() must be after the edges are constructed.
  dfs(0);

  // cur for current node, cur_layer for current layer
  int cur = 0, cur_layer = 0;
  while (q--) {
    int op;

    scanf("%d", &op);
    if (op == 1) {
      // Going downwards
      if (head[cur] != NULL) {
        printf("%d\n", head[cur]->c);
        // The depth array acts as an stack that stores depth of each layer in
        // the tree. It can help solve operation 3 and 5.
        depth[cur_layer + 1] = depth[cur_layer] + head[cur]->d;
        cur = head[cur]->c;
        // A stack that allows random-access. We can know the node at each layer
        // via this array-based stack
        stack[++cur_layer] = cur;
      } else
        printf("-1\n");
    }

    else if (op == 2) {
      // Going upwards
      if (cur) { // If cur == 0, it is at root so no operations are required
        if (cur_queue[cur_layer] != NULL) {
          pop_front(cur_queue[cur_layer]);
        }

        cur = stack[--cur_layer]; // Moving cur upwards

        // If the subtree deleted is the where the current longest leaf belongs
        // to, we have to delete that node from the queue for operation 4
        if (mxc_head[cur]->c == head[cur]->c) {
          MaxChildDequeNode *tmp = mxc_head[cur];
          mxc_head[cur] = mxc_head[cur]->nxt;
          free(tmp);

          if (mxc_head[cur] == NULL)
            mxc_tail[cur] = NULL;
          else
            mxc_head[cur]->prv = NULL;
        }

        // Simply remove the first node (aka the child we just came from) in the
        // child queue
        ChildQueueNode *tmp = head[cur];
        head[cur] = head[cur]->nxt;
        free(tmp);
        printf("%d\n", cur);

        // Note that popping from an array-based stack is not necessary.
      } else
        printf("-1\n");
    }

    else if (op == 3) {
      long long t;

      scanf("%lld", &t);
      // Perform binary search
      printf("%d\n", stack[bin_search(depth[cur_layer] - t, cur_layer)]);
    }

    else if (op == 4) {
      // Note that the bonus part only affects operation 4 and the
      // implementation here is for the bonus version. Basic: A dfs is done in
      // the beginning so we already knew the furthest leaf within each subtree.
      // Then, this problem is simply solving suffix-maximum.
      // Bonus:
      // The bonus part of operation 4 is based on a classic problem: sliding
      // window maximum https://leetcode.com/problems/sliding-window-maximum In
      // the query part of a sliding window maximum problem, it is simply the
      // head of the deque.
      if (head[cur] == NULL)
        printf("0\n");
      else {
        printf("%lld\n", mxc_head[cur]->mxv);
      }
    }

    else if (op == 5) {
      long long p;

      scanf("%lld", &p);
      // First, conduct a binary search similar to operation 3 to determine
      // where the value dropped to below 0 or that it is never negative.
      int neg_id = bin_search(depth[cur_layer] - p, cur_layer);
      p -= depth[cur_layer];

      // Store the treasure into the treasure queue.
      // If no treasure in current layer, simply create one.
      if (cur_queue[cur_layer] == NULL) {
        cur_queue[cur_layer] =
            newQueue(neg_id ? stack[neg_id - 1] : -1, p, cur_layer);
      }
      // Else, push it in the head of current queue. The rest of pushing
      // treasures upwards is handled by the function push_front().
      else {
        push_front(cur_queue[cur_layer], neg_id ? stack[neg_id - 1] : -1, p);
      }
    }

    else {
      // Bonus operation!
      int v, l;

      scanf("%d%d", &v, &l);
      // Here is relatively simple, we just push to the tail of child queue.
      if (head[cur] == NULL) {
        head[cur] = tail[cur] = newChild(v, l);
      } else {
        tail[cur]->nxt = newChild(v, l);
        tail[cur] = tail[cur]->nxt;
      }
      // Update the sliding window maximum deque.
      // Note that since v is never visited, dfs(v) is required to properly
      // calculate the furthest leaf within the subtree of v
      updatemx(cur, v, l + dfs(v));
    }
  }
}
