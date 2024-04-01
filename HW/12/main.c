#include <stdio.h>
#include <stdlib.h>

void max_heap_insert(int *A, int key, int heap_size) {
  int i = heap_size;
  A[i] = key;
  while (i > 0 && A[(i - 1) / 2] < A[i]) {
    // Swap(A[Parent(i)], A[i])
    int temp = A[i];
    A[i] = A[(i - 1) / 2];
    A[(i - 1) / 2] = temp;
    i = (i - 1) / 2; // find the parent
  }
  heap_size = heap_size + 1;
}

int main() {
  int N, *A;
  scanf("%d", &N);
  A = (int *)malloc(N * sizeof(int));
  for (int i = 0; i < N; i++) {
    int key = 0;
    scanf("%d", &key);
    max_heap_insert(A, key, i);
  }
  for (int i = 0; i < N; i++) {
    printf("%d ", A[i]);
  }
  printf("\n");
  free(A);
  return 0;
}
