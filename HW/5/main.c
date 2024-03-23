#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// O(1)
void Attack(long long *powers, int *players, int label) {
  if (powers[label] == powers[label - 1]) {
    // TODO: Exchange their rank and the attack from student labeled as a_i
    // still count
    return;
  }
}

// O(1)
void Reward() {
  // TODO: reward powers to each stduent with their rank
}

// O(lg(n))
void QueryTheLastRank(long long *powers, int *players, int q_i) {
  // TODO: query the last rank that is of power ≥ q_i , and report the label of
  // the classmate at that rank.
}

// O(1)
void QueryTheTotalPowerIncrease(long long *powers, int *players, int b_i,
                                int m_i) {
  // TODO: query the total power increase of classmate labeled bi from the last
  // m_i attacks, where m_i is no more than some known constant M .
}

int main() {
  int N, T, M = 0;
  long long *powers, *history;
  int *players;

  // N: number of players, T: number of commands
  scanf("%d %d %d", &N, &T, &M);

  // index+1 for stduent's label, value for student's powers
  powers = (long long *)malloc(N * sizeof(long long));
  memset(powers, 0, N * sizeof(long long));
  for (int i = 0; i < N; i++) {
    scanf("%lld", &powers[i]);
  }

  // index+1 for stdent's label, value for rank
  players = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    players[i] = i + 1;
  }

  // op: operation
  for (int i = 0; i < T; i++) {
    int op;
    scanf("%d", &op);
    if (op == 1) {
      int a_i; // the student labeled as a_i rank j attacks the one rank j-1
      scanf("%d", &a_i);
      if (a_i != 1) {
        Attack(powers, players, a_i - 1); // O(1)
      }
    } else if (op == 2) {
      Reward(); // O(1)
    } else if (op == 3) {
      long long q_i;
      scanf("%lld", &q_i);
      QueryTheLastRank(powers, players, q_i); // Binary-Search O(lg(n))
    } else {
      int b_i, m_i;
      scanf("%d%d", &b_i, &m_i);
      QueryTheTotalPowerIncrease(powers, players, b_i, m_i); // O(1)
    }
  }
  return 0;
}
