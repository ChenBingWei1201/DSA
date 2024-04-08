
#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000006
typedef long long ll;
int N, M;
int n_up = 0; // number of incident 2

typedef struct Node { // Node of skip list
  struct Node *pre, *nxt, *below;
  ll value; // sum of increased powers from the 1st attack to ind-th attack
  int ind, level;
} Node;

Node *new_node(ll v, int ind, int level) { // add a new node of skip list
  Node *node = (Node *)malloc(sizeof(Node));
  node->value = v;
  node->ind = ind;
  node->level = level;
  node->nxt = NULL;
  node->pre = NULL;
  node->below = NULL;
  return node;
}

Node *buildup(Node *node) { // create a new layer above the node
  Node *tmp = new_node(node->value, node->ind, node->level + 1);
  tmp->below = node;
  return tmp;
}

typedef struct Classmate {
  ll p;              // power
  Node *record_head; // head of skip list
  int n_record;      // number of attacks
  int rank;          // classmate -> rank
  int last_up;       // the n_up stamp of the last power update for incident 2
} Classmate;

Classmate player[MAXN];
int rank_table[MAXN]; // correspond from rank to the lable of classmate

void player_init(
    int i, int p,
    int rank) { // init the i-th classmate with power `p` and rank `rank`
  player[i].p = p;
  player[i].record_head = new_node(0, 0, 0);
  player[i].n_record = 0;
  player[i].rank = rank;
  player[i].last_up = 0;
}

void swap_rank(
    int a, int b) { // swap the rank of the `a`-th player and the `b`-th player
  int tmp = player[a].rank;
  player[a].rank = player[b].rank;
  player[b].rank = tmp;
  rank_table[player[a].rank] = a;
  rank_table[player[b].rank] = b;
}

void append_back(int i,
                 ll v) { // append a new record to the rear of the record list
  // 2*lg(5e5) < 40, a reasonable max height of skip list
  // path[i] is the last node in the `i`-th player
  Node *path[40] = {};
  int level[40] = {};
  Node *tmp = player[i].record_head;
  Node *head = player[i].record_head;

  // FastGet the last node of the record list of the i-th player and store at
  // tmp store the last node of each layer at `path` `level` correspond from the
  // layer to the index of `path`
  for (int j = 1; 1; j++) {
    while (tmp->nxt)
      tmp = tmp->nxt;
    path[j] = tmp;
    level[tmp->level] = j;
    if (tmp->level == 0)
      break;
    else
      tmp = tmp->below;
  }

  // add a new node at the end of the bottom layer
  int new_ind = tmp->ind + 1;
  int new_level = 0;
  v += tmp->value;
  Node *below = new_node(v, new_ind, new_level);
  tmp->nxt = below;

  while (rand() & 1) { // use random to decide whether the node should be placed
                       // at upper layer
    tmp = path[level[++new_level]];
    // create the new head if the `new_level`-th layer does not exist
    if (!tmp) {
      player[i].record_head = new_node(head->value, head->ind, head->level + 1);
      player[i].record_head->below = head;
      head = player[i].record_head;
      tmp = head;
    }

    // add a new node in the `new-level`-th layer
    tmp->nxt = new_node(v, new_ind, new_level);
    tmp->nxt->below = below;
    below = tmp->nxt;
  }
  player[i].n_record++;
}

ll query(int i, int ind) { // query the value of the `ind`-th node in the skip
                           // list of classmate `i`
  Node *tmp = player[i].record_head;
  while (tmp->ind < ind) {
    if (tmp->nxt && tmp->nxt->ind <= ind)
      tmp = tmp->nxt;
    else
      tmp = tmp->below;
  }
  return tmp->value;
}

ll update(int i) { // update the power of classmate labeled i
  player[i].p += (ll)(N - player[i].rank) * (n_up - player[i].last_up);
  player[i].last_up = n_up;
  return player[i].p;
}

void top_up(int i, ll power) { // increase the power of `i`-th player by `power`
  player[i].p += power;
  append_back(i, power);
}

int binary_search(ll q) { // search the last rank that is of power >= `q`
  if (q > update(rank_table[1]))
    return -1;
  int l = 1, r = N + 1;
  while (r - l > 1) {
    int m = (l + r) / 2;
    ll power = update(rank_table[m]);
    if (power < q)
      r = m;
    else
      l = m;
  }
  return l;
}

void incident() {
  int type;
  scanf("%d", &type);
  if (type == 1) {
    int a;
    scanf("%d", &a);
    int rank = player[a].rank;
    if (rank == 1)
      return;
    int pre_rank = rank - 1;
    int pre = rank_table[pre_rank];
    ll diff = update(pre) - update(a);
    top_up(a, diff);
    swap_rank(a, pre);
  } else if (type == 2) {
    n_up++;
  } else if (type == 3) {
    ll q;
    scanf("%lld", &q);
    int rank = binary_search(q);
    if (rank == -1)
      printf("0 0\n");
    else
      printf("%d %d\n", rank, rank_table[rank]);
  } else if (type == 4) {
    int b, m;
    scanf("%d", &b);
    scanf("%d", &m);
    int r = player[b].n_record;
    int l = (r - m) > 0 ? (r - m) : 0;
    printf("%lld\n", query(b, r) - query(b, l));
  }
}

int main() {
  srand(48763);
  int T;
  scanf("%d%d%d", &N, &T, &M);
  for (int i = 1; i <= N; i++) {
    int p;
    scanf("%d", &p);
    player_init(i, p, i);
    rank_table[i] = i;
  }
  while (T--) {
    incident();
  }
  printf("\n");
  for (int i = 1; i <= N; i++) {
    printf("%d", player[i].n_record);
    Node *it = player[i].record_head;
    while (it->level)
      it = it->below;
    ll tmp = 0;
    it = it->nxt;
    while (it != NULL) {
      printf(" %lld", it->value - tmp);
      tmp = it->value;
      it = it->nxt;
    }
    printf("\n");
  }
  return 0;
}
