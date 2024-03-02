#include <stdio.h>
#include <stdlib.h>

void insert(int *a, int m, int *c, int B) {
  int data = a[m];
  int i = m - 1;
  while (i >= 0 && a[i] > data) {
    a[i + 1] = a[i];
    i--;
    (*c)++;
    if (*c == B) {
      a[i + 1] = data;
      return;
    }
  }
  a[i + 1] = data;
}

int insertion_sort(int *a, int N, int B) {
  int c = 0;
  for (int i = 0; i < N; i++) {
    insert(a, i, &c, B);
    if (c == B) {
      break;
    }
  }
  return c;
}

int main() {
  int N, B, *a;
  scanf("%d %d", &N, &B);
  a = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    scanf("%d", &a[i]);
  }

  int total = insertion_sort(a, N, B);

  printf("The array is ");
  for (int i = 0; i < N; i++) {
    printf("%d ", a[i]);
  }
  printf("after %d back-moves.\n", total);
  free(a);
  return 0;
}
