#include <stdio.h>
#include <stdlib.h>

int bin_search(int *arr, int key, int l, int r) {
  while (l <= r) {
    printf("Searching %d in range [%d, %d].\n", key, l + 1, r + 1);
    int m = (l + r) / 2;
    if (arr[m] == key) {
      return m;
    } else if (arr[m] > key) {
      r = m - 1;
    } else if (arr[m] < key) {
      l = m + 1;
    }
  }
  printf("Searching %d in range [%d, %d].\n", key, l + 1, r + 1);
  return -1;
}

int main() {
  int N, key, *arr, index;
  scanf("%d %d", &N, &key);
  arr = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    scanf("%d", &arr[i]);
  }

  index = bin_search(arr, key, 0, N - 1);
  if (index == -1) {
    printf("Not found.\n");
  } else {
    printf("Found at index %d.\n", index + 1);
  }

  free(arr);
  return 0;
}
