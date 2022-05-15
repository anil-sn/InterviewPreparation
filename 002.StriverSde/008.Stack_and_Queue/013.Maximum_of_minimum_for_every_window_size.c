#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Problem Statement
        You are given an array of ‘N’ integers, you need to find the maximum of minimum for every window size.
        The size of the window should vary from 1 to ‘N’ only.

        For example:
        ARR = [1,2,3,4]
        Minimums of window size 1 = min(1), min(2), min(3), min(4) = 1,2,3,4
        Maximum among (1,2,3,4)  is 4

        Minimums of window size 2 = min(1,2), min(2,3),   min(3,4) = 1,2,3
        Maximum among (1,2,3) is 3

        Minimums of window size 3 = min(1,2,3), min(2,3,4) = 1,2
        Maximum among (1,2) is 2

        Minimums of window size 4 = min(1,2,3,4) = 1
        Maximum among them is 1
*/

#define STACK_MAX 100

typedef struct stack_ {
    int stack[STACK_MAX];
    int top;
    int capacity;
} STACK;

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = STACK_MAX;
    return stack;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

int pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    return (s->stack[s->top--]);
}

void push(STACK *s, int value) {
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    printf("Element %d is pushed to stack \r\n", value);
    s->stack[++s->top] = value;
    return;
}

int peek(STACK *s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

/*
    Monotonic Stack

    1) We will follow some reverse strategy for building our final solution instead of finding minimums for every possible window.
       We will find how many windows our current element can be the answer.

    2) To find that we will calculate the indexes of the next smaller and previous smaller element for every element given in
       the array. The next smaller element is some number that is smaller than the current element and lies nearest on
       the right-hand side of the current element. Similarly, the previous smaller is some number that is smaller than
       the current element and lies nearest on the left-hand side of the current element.

    3) If there is no next smaller element for the current number then we will assume its index having value N and if
       there is no previous smaller element for the current number then we will assume its index having value -1.

    4) The above two arrays of the next smaller and previous smaller can be done in linear time with the help of Monotonic Stack.

    5) Suppose ‘next[i]’ = index of next smaller element, ‘prev[i]’ = index of previous smaller element.
       Now, we know the ‘ARR[i]’ will be the minimum of the window of size -> size = next[i] - prev[i] + 1.
       So, we will directly use the value of ‘ARR[i]’ for updating the answer of window having size = next[i] - prev[i] + 1.

    6) After doing this for every element we can update our answer for windows of some different lengths i.e,
       we are still left with some window sizes for which the answer is not calculated yet.
       So to fill the remaining entries we will use some observations listed below:

        6.1) The answer for some window having size ‘L’ would always be greater than or equal to
            the answer for a window having a length greater than L i.e, L+1, L+2 .... so on.
        6.2) Hence, we will update the remaining uncalculated answer for some windows having length ‘L’ with
            the maximum suffix starting from length ‘L+1’.
*/

// This function will return an array.
// Each ith position contains the index of previous smaller elements in the original array.
void previousSmaller(int arr[], int n, int prev[])
{
    STACK *s = initStack();

    for (int i = 0; i < n; i++) {

        while (false == isStackEmpty(s) && arr[peek(s)] >= arr[i]) {
            (void)pop(s);
        }

        if (true == isStackEmpty(s)) {
            prev[i] = -1;
        } else {
            prev[i] = peek(s);
        }

        push(s, i);
    }

    return;
}

// This function will return an array.
// Each ith position contains the index of next smaller elements in the original array.
void nextSmaller(int arr[], int n, int next[])
{
    STACK *s = initStack();

    for (int i = n - 1; i >= 0; i--) {

        while (false == isStackEmpty(s) && arr[peek(s)] >= arr[i]) {
            (void)pop(s);
        }

        if (true == isStackEmpty(s)) {
            next[i] = n;
        } else {
            next[i] = peek(s);
        }

        push(s, i);
    }

    return;
}

#define MAX(a_, b_) (a_ > b_ ? a_ : b_)

void maxMinWindow(int arr[], int n, int answer[])
{
    int next[n];
    int prev[n];
    memset(next, 0, sizeof(next));
    memset(prev, 0, sizeof(prev));

    for (int i = 0; i < n; ++i) {
        answer[i] = INT_MIN;
    }

    // Definition: next[i] will store the index of next smaller element which lie on
    // the right hand side of 'i'.
    nextSmaller(arr, n, next);
    printf("Next Smaller : \r\n");
    for (int i = 0; i < n; i++) {
        printf("%3d", next[i]);
    }
    printf("\r\n");

    // Definition: prev[i] will store the index of previous smaller element which lie on
    // the left hand side of 'i'.
    previousSmaller(arr, n, prev);
    printf("Previous Smaller : \r\n");
    for (int i = 0; i < n; i++) {
        printf("%3d", prev[i]);
    }
    printf("\r\n");

    for (int i = 0; i < n; i++)
    {
        // Length of the window in which a[i] is minimum
        int length = next[i] - prev[i] - 1;

        // Update the answer[length-1] ( 0 based indexing )  with a[i]
        answer[length - 1] = MAX(answer[length - 1], arr[i]);
    }

    // Some entries in answer[] may not be filled yet.
    // We fill them by taking maximum element from suffix starting from 'i'.
    for (int i = n - 2; i >= 0; i--)
    {
        answer[i] = MAX(answer[i], answer[i + 1]);
    }

    return;
}

int main ()
{
    int arr[] = {3, 3, 4, 2, 4 };
    int len = sizeof(arr)/sizeof(arr[0]);
    int answer[len];
    memset(answer, 0, sizeof(answer));

    printf("Input : \r\n");
    for (int i = 0; i < len; i++) {
        printf("%3d", arr[i]);
    }
    printf("\r\n");

    maxMinWindow(arr, len, answer);

    printf("Answer : \r\n");
    for (int i = 0; i < len; i++) {
        printf("%3d", answer[i]);
    }
    printf("\r\n");

    return;
}