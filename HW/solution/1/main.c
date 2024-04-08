#include <stdbool.h>
#include <stdio.h>

int a[20];           // the given array 'a' in the problem
int ans[200000][20]; // record all the beautiful permutations
int cur[20];         // record the current sequence during recursion
int num = 0;         // record the number of beautiful permutations
bool used[20] =
    {}; // record whether the element in 'a' has been used during recursion

// Use recursion to find all the beautiful permutations
// 'idx' represents the current position during recursion
void solve(int idx, int n) {
  if (idx == n) { // The current sequence ('cur') is a beautiful permutation
    for (int i = 0; i < n; i++) {
      ans[num][i] = cur[i];
    }
    num++;
    return;
  }
  int pre = -1;
  for (int i = 0; i < n; i++) {
    // If 'a[i]' hasn't been used, try to put 'a[i]' at 'cur[idx]'
    if (!used[i]) {
      // Avoid repeated recursion (the same 'cur[0...idx]' should not be
      // recursed more than once)
      if (pre != -1 && a[pre] == a[i])
        continue;
      pre = i;

      if (idx < 2 ||
          ((long long)(a[i] - cur[idx - 1]) * (cur[idx - 1] - cur[idx - 2]) <
           0)) {
        // Continue recursion only when the current sequence ('cur') is not
        // beautiful
        cur[idx] = a[i];
        used[i] = true;
        solve(idx + 1, n);
        used[i] = false;
      }
    }
  }
}

int main() {

  int n;
  scanf("%d", &n);

  for (int i = 0; i < n; i++) {
    scanf("%d", &a[i]);
  }

  // Sort the array from small to large
  for (int i = 0; i < n; i++) {
    for (int j = 1; j < n; j++) {
      if (a[j - 1] > a[j]) {
        int tmp = a[j - 1];
        a[j - 1] = a[j];
        a[j] = tmp;
      }
    }
  }

  solve(0, n);
  // Output all the beautiful permutations in 'ans'
  printf("%d\n", num);
  for (int i = 0; i < num; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", ans[i][j]);
    }
    printf("\n");
  }

  return 0;
}
