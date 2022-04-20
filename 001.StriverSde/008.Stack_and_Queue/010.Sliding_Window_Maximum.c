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
    Sliding Window Maximum
        Problem Statement: Given an array of integers arr, there is a sliding window of size k
        which is moving from the very left of the array to the very right. You can only see the k
        numbers in the window. Each time the sliding window moves right by one position.
        Return the max sliding window.

    Examples:

    Example 1:

    Input: arr = [4,0,-1,3,5,3,6,8], k = 3
    Output: [4,3,5,5,6,8]
    Explanation:
        Window position                   Max
        ------------------------         -----
        [4  0  -1] 3  5  3  6  8           4
         4 [0  -1  3] 5  3  6  8           3
         4  0 [-1  3  5] 3  6  8           5
         4  0  -1 [3  5  3] 6  8           5
         4  0  -1  3 [5  3  6] 8           6
         4  0  -1  3  5 [3  6  8]          8

        For each window of size k=3, we find the maximum element in the window and add it to our output array.

    Example 2:
        Input: arr= [20,25], k = 2
        Output: [25]
        Explanation: There’s just one window is size 2 that is possible and the maximum of the two elements is our answer.
*/
#include <stdio.h>

#define QUEUE_MAX 12

int queue[QUEUE_MAX];
int front = -1;
int rear = -1;

int count()
{
  int c = 0, i;

    for (i = 0; i < QUEUE_MAX; i++) {
        if (queue[i] != 0) {
            c++;
        }
    }

    return c;
}

bool deQueueEmpty ()
{
    if (front == -1) {
        return true;
    }

    return false;
}

bool deQueueFull ()
{
    if (front == 0 && rear == QUEUE_MAX - 1) {
        return true;
    }

    return false;
}

int deQueueFront()
{
    return queue[front];
}

int deQueueRear()
{
    return queue[rear];
}

void addFront(int item)
{
    int i, k, c;

    if (front == 0 && rear == QUEUE_MAX - 1) {
        printf("\nDeque is full.\n");
        return;
    }

    if (front == -1) {

        front = rear = 0;
        queue[front] = item;
        return;
    }

    if (rear != QUEUE_MAX - 1) {

        c = count(queue);
        k = rear + 1;
        for (i = 1; i <= c; i++) {
            queue[k] = queue[k - 1];
            k--;
        }

        queue[k] = item;
        front = k;
        (rear)++;
    } else {
        (front)--;
        queue[front] = item;
    }
}

void addRear(int item) {
  int i, k;

  if (front == 0 && rear == QUEUE_MAX - 1) {
    printf("\nDeque is full.\n");
    return;
  }

  if (front == -1) {
    rear = front = 0;
    queue[rear] = item;
    return;
  }

  if (rear == QUEUE_MAX - 1) {
    k = front - 1;
    for (i = front - 1; i < rear; i++) {
      k = i;
      if (k == QUEUE_MAX - 1)
        queue[k] = 0;
      else
        queue[k] = queue[i + 1];
    }
    (rear)--;
    (front)--;
  }
  (rear)++;
  queue[rear] = item;
}

int delFront()
{
    int item;

    if (front == -1) {
        printf("\nDeque is empty.\n");
        return 0;
    }

    item = queue[front];
    queue[front] = 0;

    if (front == rear)
        front = rear = -1;
    else
        (front)++;

    return item;
}

int delRear() {
    int item;

    if (front == -1) {
        printf("\nDeque is empty.\n");
        return 0;
    }

    item = queue[rear];
    queue[rear] = 0;
    (rear)--;
    if (rear == -1)
        front = -1;
    return item;
}

void display() {
    int i;

    printf("\n front:  ");
    for (i = 0; i < QUEUE_MAX; i++)
        printf("  %d", queue[i]);
    printf("  :rear");
}


#define MAX(a_, b_) (a_ > b_ ? a_ : b_)

void GetMax(int nums[], int left, int right, int ans[], int *pos)
{
    int i;
    int maxi = INT_MIN;

    for (i = left; i <= right; i++) {
        maxi = MAX(maxi, nums[i]);
    }

    ans[(*pos)++] = maxi;
}

void maxSlidingWindow(int arr[], int n, int k, int ans[])
{
    int left = 0;
    int right = 0;
    int pos = 0;
    int i, j;

    while (right < k - 1) {
        right++;
    }

    while (right < n) {
        GetMax(arr, left, right, ans, &pos);
        left++;
        right++;
    }

    return;
}

void maxSlidingWindowDeQueue(int nums[], int n, int k, int ans[])
{
    int pos = 0;
    for (int i = 0; i < n; i++) {
        //display();
        if ((false == deQueueEmpty()) && (deQueueFront() == i - k)) {
            (void) delFront();
        }

        while ((false == deQueueEmpty()) && nums[deQueueRear()] < nums[i]) {
            (void )delRear();
        }

        addRear(i);

        if (i >= k - 1) {
            ans[pos++] = nums[deQueueFront()];
        }
    }

    return;
}

int main()
{
    int k = 3;
    int arr[] = {4,0,-1,3,5,3,6,8};
    int n = sizeof(arr)/sizeof(arr[0]);
    int ans[n];

    printf("Input : \r\n");
    for (int i = 0; i < n; i++) {
        printf("%4d", arr[i]);
    }
    printf("\r\n");

    maxSlidingWindow(arr, n, k, ans);
    printf("Maximum element in every %d window : \r\n",k );
    for (int i = 0; i < n; i++) {
        printf("%4d", ans[i]);
    }
    printf("\r\n");

    memset(ans, 0, sizeof(ans));
    maxSlidingWindowDeQueue(arr, n, k, ans);
    printf("\r\nOptimized Maximum element in every %d window : \r\n",k );
    for (int i = 0; i < n; i++) {
        printf("%4d", ans[i]);
    }
    printf("\r\n");

    return 0;
}