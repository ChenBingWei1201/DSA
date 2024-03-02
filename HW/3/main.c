#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_min_index(int *a, int l, int r) {
  int m = l;
  for (int i = l + 1; i < r; i++) {
    if (a[m] > a[i]) {
      m = i;
    }
  }
  return m;
}

int selection_sort(int *a, int N, int B) {
  int m = 0;
  int c = 0;
  for (int i = 0; i < N; i++) {
    m = get_min_index(a, i, N);
    if (i != m) {
      a[i] = a[i] ^ a[m];
      a[m] = a[i] ^ a[m];
      a[i] = a[i] ^ a[m];
      c++;
      if (c == B) {
        break;
      }
    }
  }
  return c;
}

int main() {
  int N, B, *a;
  scanf("%d %d", &N, &B);
  a = (int *)malloc(sizeof(int) * N);
  memset(a, 0, sizeof(int) * N);

  for (int i = 0; i < N; i++) {
    scanf("%d", &a[i]);
  }

  B = selection_sort(a, N, B);

  printf("The array is ");
  for (int i = 0; i < N; i++) {
    printf("%d ", a[i]);
  }
  printf("after %d swaps.\n", B);

  free(a);
  return 0;
}
