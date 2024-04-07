#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4000

// Given an infix arithmetic expression that contains binary operators +, - , *,
// /, % as well as parentheses on non-negative integers, convert it to the
// postfix expression by

// Input: Each line contains a valid infix expression without any spaces.
// Output: For each infix expression in the input, output a line of
// [postfix]=[value] where [postfix] is the postifix expression without spaces,
// and [value] is the final value.

// precedence: '+' = '-' < '*' = '/' = '%'
int precedence(char c) {
  if (c == '+' || c == '-') {
    return 1;
  } else if (c == '*' || c == '/' || c == '%') {
    return 2;
  } else {
    return 0;
  }
}

int infix_to_postfix(char *infix, char *postfix, long long *numbers) {
  // S is an empty stack
  char *S = (char *)malloc(MAX * sizeof(char));
  int i = 0, j = 0, k = 0, c = 0;
  int top = -1;
  for (; infix[i] != '\0'; i++) {
    if (infix[i] >= '0' && infix[i] <= '9') {
      long long num = 0;
      postfix[j] = infix[i];
      j++;
      while (infix[c] >= '0' && infix[c] <= '9') {
        num = num * 10 + (infix[c] - '0');
        if (infix[c + 1] < '0' || infix[c + 1] > '9') {
          numbers[k++] = num;
        }
        c++;
      }
    } else {
      if (infix[i] == '(') {
        S[++top] = infix[i]; // push '(' to S
      } else if (infix[i] == ')') {
        while (top != -1 && S[top] != '(') {
          postfix[j++] = S[top--]; // pop and append to postfix
          numbers[k++] = S[top + 1] - '0';
        }
        top--; // pop '(' from S
      } else {
        while (top != -1 && precedence(S[top]) >= precedence(infix[i])) {
          postfix[j++] = S[top--]; // pop and append to postfix
          numbers[k++] = S[top + 1] - '0';
        }
        S[++top] = infix[i]; // push infix[i] to S
      }
      c++;
    }
  }
  while (top != -1) {
    postfix[j++] = S[top--]; // pop and append to postfix
    numbers[k++] = S[top + 1] - '0';
  }
  return k;
}

void postfix_eval(long long *numbers, int len) {
  long long *T = (long long *)malloc(MAX * sizeof(long long));
  memset(T, 0, MAX * sizeof(long long));
  int top = -1;
  for (int i = 0; i < len; i++) {
    if (numbers[i] >= 0) {
      T[++top] = numbers[i];
    } else {
      long long op2 = T[top--]; // pop the second operand from T
      long long op1 = T[top--]; // pop the first operand from T
      switch (numbers[i]) {
      case -5:
        T[++top] = op1 + op2;
        break;
      case -3:
        T[++top] = op1 - op2;
        break;
      case -6:
        T[++top] = op1 * op2;
        break;
      case -1:
        T[++top] = op1 / op2;
        break;
      case -11:
        T[++top] = op1 % op2;
        break;
      }
    }
  }
  printf("%lld", T[top]); // the final value
  free(T);
}

int main() {
  char *infix[3], *postfix[3];
  long long *numbers[3];
  int len[3] = {0, 0, 0};
  for (int i = 0; i < 3; i++) {
    infix[i] = (char *)malloc(MAX * sizeof(char));
    postfix[i] = (char *)malloc(MAX * sizeof(char));
    numbers[i] = (long long *)malloc(MAX * sizeof(long long));
    memset(numbers[i], 0, MAX * sizeof(long long));
  }
  for (int i = 0; i < 3; i++) {
    scanf("%s", infix[i]);
    len[i] = infix_to_postfix(infix[i], postfix[i], numbers[i]);
  }
  for (int i = 0; i < 3; i++) {
    printf("%s=", postfix[i]);
    postfix_eval(numbers[i], len[i]);
    printf("\n");
  }
  for (int i = 0; i < 3; i++) {
    free(infix[i]);
    free(postfix[i]);
    free(numbers[i]);
  }
  return 0;
}
