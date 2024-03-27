#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct History {
  long long gain_power;
  struct History *next;
  struct History *prev;
} History;

typedef struct PlayerRecord {
  History *head;
  History *tail;
  int size;
} PlayerRecord;

typedef struct Incident3 {
  int rank;
  int label;
  struct Incident3 *next;
} Incident3;

typedef struct Incident3List {
  Incident3 *head;
  Incident3 *tail;
} Incident3List;

typedef struct Incident4 {
  long long total_power_increase;
  struct Incident4 *next;
} Incident4;

typedef struct Incident4List {
  Incident4 *head;
  Incident4 *tail;
} Incident4List;

History *CreateHistory(long long gain_power) {
  History *history = (History *)malloc(sizeof(History));
  history->gain_power = gain_power;
  history->next = NULL;
  history->prev = NULL;
  return history;
}

PlayerRecord *CreatePlayerRecord() {
  PlayerRecord *player = (PlayerRecord *)malloc(sizeof(PlayerRecord));
  player->head = NULL;
  player->tail = NULL;
  player->size = 0;
  return player;
}

void AddHistory(PlayerRecord *player, long long gain_power) {
  // printf("gain_power: %lld\n", gain_power);
  History *history = CreateHistory(gain_power);
  if (player->head == NULL) {
    player->head = history;
    player->tail = history;
  } else {
    player->tail->next = history;
    history->prev = player->tail;
    player->tail = history;
  }
  player->size++;
}

Incident3 *CreateIncident3(int rank, int label) {
  Incident3 *incident3 = (Incident3 *)malloc(sizeof(Incident3));
  incident3->rank = rank;
  incident3->label = label;
  incident3->next = NULL;
  return incident3;
}

Incident3List *CreateIncident3List() {
  Incident3List *incident3_list =
      (Incident3List *)malloc(sizeof(Incident3List));
  incident3_list->head = NULL;
  incident3_list->tail = NULL;
  return incident3_list;
}

Incident3 *AddIncident3(Incident3List *incident3_list, int rank, int label) {
  Incident3 *incident3 = CreateIncident3(rank, label);
  if (incident3_list->head == NULL) {
    incident3_list->head = incident3;
    incident3_list->tail = incident3;
  } else {
    incident3_list->tail->next = incident3;
    incident3_list->tail = incident3;
  }
  return incident3;
}

Incident4 *CreateIncident4(long long total_power_increase) {
  Incident4 *incident4 = (Incident4 *)malloc(sizeof(Incident4));
  incident4->total_power_increase = total_power_increase;
  incident4->next = NULL;
  return incident4;
}

Incident4List *CreateIncident4List() {
  Incident4List *incident4_list =
      (Incident4List *)malloc(sizeof(Incident4List));
  incident4_list->head = NULL;
  incident4_list->tail = NULL;
  return incident4_list;
}

Incident4 *AddIncident4(Incident4List *incident4_list,
                        long long total_power_increase) {
  Incident4 *incident4 = CreateIncident4(total_power_increase);
  if (incident4_list->head == NULL) {
    incident4_list->head = incident4;
    incident4_list->tail = incident4;
  } else {
    incident4_list->tail->next = incident4;
    incident4_list->tail = incident4;
  }
  return incident4;
}

void Swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

// O(1)
void Attack(long long *powers, int *players, int *ranks, int label, long long r,
            int N, PlayerRecord *player_record) {
  int j = players[label]; // j: the rank of the student labeled as a_i
  // long long a = powers[ranks[j - 1] - 1] + (N - j) * r;
  long long a = powers[label];
  // find the power of the student of rank j-1
  // long long b = powers[ranks[j - 1] - 2] + (N - (j - 1)) * r;
  long long b = powers[ranks[j - 1 - 1] - 1];
  if (a == b) {
    // TODO: Exchange their rank and the attack from student labeled as a_i
    // still count

    AddHistory(player_record, 0LL);
  } else {
    // TODO: The power of the student labeled as a_i
    // rank j equals to the power of the student rank j-1 and exchange their
    // rank

    AddHistory(player_record, b - a);
    powers[label] = b;
  }

  // Swap two students' rank
  Swap(&players[label], &players[ranks[j - 1 - 1] - 1]);
  Swap(&ranks[j - 1], &ranks[j - 2]);
  return;
}

// O(lg(n))
void QueryTheLastRank(long long *powers, int *players, int *ranks, int q_i,
                      int N, int reward, Incident3List *incident_3_list) {
  // TODO: query the last rank that is of power ≥ q_i , and report the label of
  // the classmate at that rank and his label, if no such classmate exists,
  // add {0, 0} to incident_3_list

  int *answer = (int *)malloc(2 * sizeof(int));
  memset(answer, 0, 2 * sizeof(int));

  // Binary-Search (powers is in reverse order)
  int l = 0, r = N - 1;
  while (l < r) {
    int mid = (l + r) / 2;
    if (powers[ranks[mid] - 1] >= q_i) {
      l = mid + 1;
    } else {
      r = mid;
    }
  }

  if (powers[ranks[l - 1] - 1] >= q_i) {
    answer[0] = players[ranks[l - 1] - 1];
    answer[1] = ranks[l - 1];
  }

  AddIncident3(incident_3_list, answer[0], answer[1]);
  free(answer);
}

int main() {
  int N, T, M = 0;
  long long r = 0;
  long long *powers;
  int *players, *ranks;
  PlayerRecord **players_record;
  Incident3List *incident_3_list = CreateIncident3List();
  Incident4List *incident_4_list = CreateIncident4List();

  // N: number of players, T: number of commands
  scanf("%d %d %d", &N, &T, &M);

  // index+1 for student's label, value for student's powers
  powers = (long long *)malloc(N * sizeof(long long));
  memset(powers, 0, N * sizeof(long long));
  for (int i = 0; i < N; i++) {
    scanf("%lld", &powers[i]);
  }

  // index+1 for student's label, value for student's rank
  players = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    players[i] = i + 1;
  }

  // index+1 for student's rank, value for student's label
  ranks = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    ranks[i] = i + 1;
  }

  players_record = (PlayerRecord **)malloc(N * sizeof(PlayerRecord *));
  for (int i = 0; i < N; i++) {
    players_record[i] = CreatePlayerRecord();
  }

  // op: operation
  for (int i = 0; i < T; i++) {
    int op;
    scanf("%d", &op);
    if (op == 1) {
      int a_i; // the student labeled as a_i rank j attacks the one rank j-1
      scanf("%d", &a_i);
      if (players[a_i - 1] != 1) {
        Attack(powers, players, ranks, a_i - 1, r, N,
               players_record[a_i - 1]); // O(1)
      }
    } else if (op == 2) { // O(1)
      // r: the counter of rewards
      for (int i = 0; i < N; i++) {
        powers[i] += N - players[i];
      }
    } else if (op == 3) {
      long long q_i;
      scanf("%lld", &q_i);
      // Binary-Search O(lg(n))
      QueryTheLastRank(powers, players, ranks, q_i, N, r, incident_3_list);
    } else {
      int b_i, m_i;
      scanf("%d%d", &b_i, &m_i);
      int total_power_increase = 0;
      History *current = players_record[b_i - 1]->tail;
      // int length = players_record[b_i - 1]->size;
      if (current != NULL) {
        for (int i = 0; i < m_i; i++) {
          total_power_increase += current->gain_power;
          if (current->prev == NULL) {
            break;
          }
          current = current->prev;
        }
        AddIncident4(incident_4_list, total_power_increase);
      } else {
        AddIncident4(incident_4_list, 0LL);
      }
    }
  }

  // print all incident3
  for (Incident3 *current = incident_3_list->head; current != NULL;
       current = current->next) {
    if (current == NULL) {
      break;
    }
    printf("%d %d\n", current->rank, current->label);
  }

  // print all incident4
  for (Incident4 *current = incident_4_list->head; current != NULL;
       current = current->next) {
    if (current == NULL) {
      printf("0\n");
      break;
    } else {
      printf("%lld\n", current->total_power_increase);
    }
  }

  printf("\n");

  // print all players' record
  for (int i = 0; i < N; i++) {
    History *current = players_record[i]->head;
    printf("%d ", players_record[i]->size);
    while (current != NULL) {
      printf("%lld ", current->gain_power);
      current = current->next;
    }
    printf("\n");
  }

  // printf all players' powers
  // for (int i = 0; i < N; i++) {
  //   printf("%lld ", powers[i]);
  // }
  // printf("\n");

  // printf all players' ranks
  // for (int i = 0; i < N; i++) {
  //   printf("%d ", players[i]);
  // }
  // printf("\n");

  free(powers);

  free(players);
  free(ranks);
  Incident3 *current_incident3 = incident_3_list->head;
  while (current_incident3 != NULL) {
    Incident3 *temp = current_incident3;
    current_incident3 = current_incident3->next;
    free(temp);
  }
  free(incident_3_list);

  Incident4 *current_incident4 = incident_4_list->head;
  while (current_incident4 != NULL) {
    Incident4 *temp = current_incident4;
    current_incident4 = current_incident4->next;
    free(temp);
  }
  free(incident_4_list);

  for (int i = 0; i < N; i++) {
    History *current_history = players_record[i]->head;
    while (current_history != NULL) {
      History *temp_history = current_history;
      current_history = current_history->next;
      free(temp_history);
    }
    free(players_record[i]);
  }
  free(players_record);
  return 0;
}
