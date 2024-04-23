#include <stdio.h>

int countTrees(char pre[], char post[], int pre_start, int pre_end,
               int post_start, int post_end) {
  if (pre_start > pre_end) {
    return 1;
  }
  if (pre_start == pre_end) {
    return 1;
  }
  int total_trees = 0;
  int i = pre_start + 1;
  while (i <= pre_end) {
    char left_root = pre[i];
    int j = post_start;
    while (j <= post_end) {
      if (post[j] == left_root) {
        int left_count = countTrees(pre, post, pre_start + 1, i, post_start, j);
        int right_count =
            countTrees(pre, post, i + 1, pre_end, j + 1, post_end - 1);
        total_trees += left_count * right_count;
        break;
      }
      j++;
    }
    i++;
  }
  return total_trees;
}

int main() {
  char pre[] = {'A', 'B', 'E', 'C', 'D'};
  char post[] = {'E', 'D', 'C', 'B', 'A'};
  int n = sizeof(pre) / sizeof(pre[0]);
  int tree_count = countTrees(pre, post, 0, n - 1, 0, n - 1);
  printf("Total possible trees: %d\n", tree_count);
  return 0;
}
