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
    Area of largest rectangle in Histogram

    Problem Statement: Given an array of integers heights representing the histogram’s
        bar height where the width of each bar is 1  return the area of the largest rectangle
        in histogram.

    Example:
        Input: N =6, heights[] = {2,1,5,6,2,3}
        Output: 10

        ────┐           ┌───┐
          6 │           │   │
        ────┤       ┌───┼───┤
          5 │       │XXX│XXX│
        ────┤       │XXX│XXX│
          4 │       │XXX│XXX│
        ────┤       │XXX│XXX│   ┌───┐
          3 │       │XXX│XXX│   │   │
        ────┼───┐   │XXX│XXX├───┤   │
          2 │   │   │XXX│XXX│   │   │
        ────┤   ├───┤XXX│XXX│   │   │
          1 │   │   │XXX│XXX│   │   │
        ────┼───┼───┼───┼───┼───┼───┼──
            │   │   │   │   │   │   │
            │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │
*/

#define MIN(a_, b_) (a_ < b_ ? a_ : b_)
#define MAX(a_, b_) (a_ > b_ ? a_ : b_)

// Brute Force Approach to find largest rectangle area in Histogram
int largestAreaBruteForce(int arr[], int n)
{
    int maxArea = 0;
    for (int i = 0; i < n; i++) {

        int minHeight = INT_MAX;
        for (int j = i; j < n; j++) {

            minHeight = MIN(minHeight, arr[j]);
            maxArea = MAX(maxArea, minHeight * (j - i + 1));
        }
    }

    return maxArea;
}

#define STACK_MAX 100
int stack[STACK_MAX];
int top = -1;

bool isStackEmpty()
{
    if (top < 0) {
        return true;
    }

    return false;
}

bool isStackFull()
{
    if (top >= STACK_MAX) {
        return true;
    }

    return false;
}

void push(int val)
{
    if (isStackFull()) {
        return;
    }

    stack[++top] = val;
    return;
}

int pop()
{
    if (isStackEmpty()) {
        return -1;
    }

    return (stack[top--]);
}

int peek()
{
    return (stack[top]);
}

int largestAreaExtSpace(int heights[], int n)
{
    int maxArea = 0;
    int top = -1;
    int leftsmall[n];
    int rightsmall[n];

    for (int i = 0; i < n; i++) {

        /* Making sure stack entries to be smaller then current */
        while (false == isStackEmpty() && heights[peek()] >= heights[i]) {
            (void) pop();
        }

        if (true == isStackEmpty()) {
            leftsmall[i] = 0;
        } else {
            leftsmall[i] = peek() + 1;
        }

        push(i);
    }

    // clear the stack to be re-used
    while (false == isStackEmpty()) {
        (void) pop();
    }

    for (int i = n - 1; i >= 0; i--) {
        while (false == isStackEmpty() && heights[peek()] >= heights[i]) {
            (void) pop();
        }

        if (true == isStackEmpty()) {
            rightsmall[i] = n - 1;
        } else {
            rightsmall[i] = peek() - 1;
        }

        push(i);
    }

    for (int i = 0; i < n; i++) {
        maxArea = MAX(maxArea, heights[i] * (rightsmall[i] - leftsmall[i] + 1));
    }

    // clear the stack to be re-used
    while (false == isStackEmpty()) {
        (void) pop();
    }

    return maxArea;
}

int largestRectangleAreaIntution(int histo[], int n)
{

    int maxArea = 0;

    for (int i = 0; i <= n; i++) {
        while (false == isStackEmpty() && (i == n || histo[peek()] >= histo[i])) {

            int height = histo[peek()];
            (void) pop();

            int width;
            if (isStackEmpty()) {
                width = i;
            } else {
                width = i - peek() - 1;
            }

            maxArea = MAX(maxArea, width * height);
        }

        push(i);
    }

    return maxArea;
}

int main()
{
  int arr[] = {2, 1, 5, 6, 2, 3, 1};
  int n = 7;

  // Printing the largest rectangle area
  printf("[BruteForce] The largest area in the histogram is %d \r\n", largestAreaBruteForce(arr, n));
  printf("[Optimization With Extra Space] The largest area in the histogram is %d \r\n", largestAreaExtSpace(arr, n));
  printf("[Optimization Best] The largest area in the histogram is %d \r\n", largestRectangleAreaIntution(arr, n));
  return 0;
}