#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#define CAPACITY 5000000
#define MAX_SIZE 20

bool is_beautiful(int *a, int N) {
  for (int i = 1; i < N - 1; i++) {
    if ((a[i] < a[i - 1] && a[i] > a[i + 1]) ||
        (a[i] > a[i - 1] && a[i] < a[i + 1]) || a[i] == a[i - 1] ||
        a[i] == a[i + 1]) {
      return false;
    }
  }
  return true;
}

void swap(int *x, int *y) {
  int t = *x;
  *x = *y;
  *y = t;
}

int get_min_index(int *a, int l, int r) {
  int m = l;
  for (int i = l + 1; i < r; i++) {
    if (a[m] > a[i]) {
      m = i;
    }
  }
  return m;
}

void selection_sort(int *a, int N) {
  int m = 0;
  for (int i = 0; i < N; i++) {
    m = get_min_index(a, i, N);
    if (i != m) {
      swap(&a[i], &a[m]);
    }
  }
}

// Recursive function to generate and check beautiful permutations
void generate_permutations(int *nums, int *numbers, int N, int **permutations,
                           int *temp, int tempSize, int *c) {
  if (tempSize == N) {
    for (int i = 0; i < N; i++) {
      permutations[*c][i] = temp[i];
    }
    (*c)++;
    return;
  }

  for (int i = 0; i < N; i++) {
    if (numbers[i] > 0) {
      temp[tempSize] = nums[i];
      numbers[i]--;
      // Check if temp is beautiful up to tempSize + 1
      if (is_beautiful(temp, tempSize + 1)) {
        generate_permutations(nums, numbers, N, permutations, temp,
                              tempSize + 1, c);
      }
      numbers[i]++; // backtrack
    }
  }
}
int main() {
  int N, *a;
  scanf("%d", &N);

  a = (int *)malloc(N * sizeof(int));

  for (int i = 0; i < N; i++) {
    scanf("%d", &a[i]);
  }

  selection_sort(a, N); // sort the array first, [1, 1, 1, 2, 3, 3]
  // nums = [1, 2, 3], numbers = [3, 1, 2]
  int num, count = 0;
  int *nums, *numbers;
  nums = (int *)malloc(sizeof(int) * N);
  numbers = (int *)malloc(sizeof(int) * N);
  nums[count] = a[0];
  numbers[count] = 1;
  count++;
  for (int i = 1; i < N; i++) {
    num = a[i];
    if (num != a[i - 1]) {
      nums[count] = num;
      numbers[count] = 1;
      count++;
    } else {
      numbers[count - 1]++;
    }
  }

  int **permutations;
  permutations = (int **)malloc(CAPACITY * sizeof(int *));

  for (int i = 0; i < CAPACITY; i++) {
    permutations[i] = (int *)malloc(N * sizeof(int));
  }

  int c = 0; // couter: number of beautiful permutations
  int *temp = (int *)malloc(MAX_SIZE * sizeof(int));

  if (N == 2) {
    if (a[0] != a[1]) {
      printf("%d\n", N);
      printf("%d %d\n", a[0], a[1]);
      printf("%d %d\n", a[1], a[0]);
    } else {
      printf("%d\n", 1);
      printf("%d %d\n", a[0], a[1]);
    }
  } else if (N == 1) {
    printf("%d\n", N);
    printf("%d\n", a[0]);
  } else {
    generate_permutations(nums, numbers, N, permutations, temp, 0, &c);
    printf("%d\n", c);
    for (int i = 0; i < c; i++) {
      for (int j = 0; j < N; j++)
        printf("%d ", permutations[i][j]);
      printf("\n");
    }
  }

  free(a);
  for (int i = 0; i < CAPACITY; i++) {
    free(permutations[i]);
  }
  free(permutations);
  free(numbers);
  free(nums);
  return 0;
}
