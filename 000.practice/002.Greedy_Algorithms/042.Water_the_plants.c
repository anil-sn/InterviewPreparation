#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    Water the plants

    A gallery with plants is divided into n parts, numbered : 0,1,2,3...n-1.
    There are provisions for attaching water sprinklers at every partition.
    A sprinkler with range x at partition i can water all partitions from i-x to i+x.
    Given an array gallery[ ] consisting of n integers, where gallery[i] is the range of sprinkler
    at partition i (power==-1 indicates no sprinkler attached), return the minimum number of sprinklers
    that need to be turned on to water the complete gallery.
    If there is no possible way to water the full length using the given sprinklers, print -1.

    Example 1:
    Input:
        n = 6
        gallery[ ] = {-1, 2, 2, -1, 0, 0}
    Output:
        2
    Explanation: Sprinklers at index 2 and 5 can water thefull gallery,
        span sprinkler at index 2 = [0,4] and span of sprinkler at index 5 = [5,5].

    Example 2:
    Input:
        n = 9
        gallery[ ] = {2, 3, 4, -1, 2, 0, 0, -1, 0}
    Output:
        -1
    Explanation: No sprinkler can throw water at index 7.
        Hence all plants cannot be watered.

    Example 3:
    Input:
        n = 9
        gallery[ ] = {2, 3, 4, -1, 0, 0, 0, 0, 0}
    Output:
        3
    Explanation: Sprinkler at indexes 2, 7 and 8 together can water all plants.

    Your task:
        complete the function min_sprinklers() which takes the array gallery[ ] and the integer n as
        input parameters and returns the value to be printed.

Approach: The above problem can be solved using the greedy technique. The idea is to first sort the range by left boundary and then traversing ranges from left and in each iteration select the rightmost boundary a sprinkler can cover having the left boundary in the current range. Follow the steps below to solve the problem:

    1. Initialize a vector<pair<int, int>> say sprinkler to store the range of every sprinkler as a pair.
    2. Traverse the array arr[] and if arr[i] is not equal to -1 then
        push the pair (i-arr[i], i+arr[i]) in the vector sprinkler.
    3. Sort the vector of pairs in ascending order by the first element.
    4. Initialize 2 variables say res, and target to store the minimum sprinklers to be turned on and
        to store the rightmost boundary of an array.
    5. Initialize a variable say i as 0 to iterate over the sprinkler.
    6. Iterate until target is less than N and perform the following steps:
    7. If i is equal to sprinkler.size() or sprinkler[i].first is greater than target then print -1 and return.
    8. Store the right boundary of the current sprinkler in the variable say max_range.
    9. Now iterate until i+1 is less than sprinkler.size() and sprinkler[i+1].first is less than or equal
        to target then in each iteration increment i by 1 and
        update max_range as max_range = min(max_range, sprinkler[i].second).
    10. If max_range is less than the target then print -1 and return.
    11. Update target as target = max_range+1 then Increment res and i by 1.
    12. Finally, after completing the above step, print the res as the answer.
*/
typedef struct pair {
    int left;
    int right;
} PAIR;

int cmp (const void *x, const void *y){
    PAIR *a = (PAIR *)x;
    PAIR *b = (PAIR *)y;

    return (a->left > b->right);
}

// Function to find minimum number of
// sprinkler to be turned on
int minSprinklers(int arr[], int N)
{
    // Stores the leftmost and rightmost point of every sprinklers
    PAIR sprinkler[100];
    int pos = 0;

    // Traverse the array arr[]
    for (int i = 0; i < N; i++) {
        if (arr[i] > -1) {
            sprinkler[pos].left = i - arr[i];
            sprinkler[pos].right = i + arr[i];
            pos++;
        }
    }

    for (int i = 0; i < pos; i++){
        printf ("Left Range: %d, Right Range : %d\r\n", sprinkler[i].left, sprinkler[i].right);
    }
    printf("\r\n");

    // Sort the array sprinklers in ascending order by first element
    qsort(sprinkler, pos, sizeof(sprinkler[0]), cmp);

    for (int i = 0; i < pos; i++){
        printf ("Left Range: %d, Right Range : %d\r\n", sprinkler[i].left, sprinkler[i].right);
    }
    printf("\r\n");

    // Stores the rightmost range of a sprinkler
    int target = 0;

    // Stores minimum sprinklers to be turned on
    int res = 0;

    int i = 0;

    // Iterate until target is less than N
    while (target < N) {

        // If i is equal to sprinkler.size() or sprinkler[i].first is greater target
        if (i == pos || sprinkler[i].left > target) {
            return -1;
        }

        // Stores the rightmost boundary of current sprinkler
        int max_range = sprinkler[i].right;

        // Iterate until i+1 is less
        // than sprinkler.size() and sprinkler[i+1].first
        // is less than or equal to target
        while (i + 1 < pos && sprinkler[i + 1].left <= target) {

            // Increment i by 1
            i++;

            // Update max_range
            max_range = max(max_range, sprinkler[i].right);
        }

        // If max_range is less than the target
        if (max_range < target) {
            // Return -1
            return -1;
        }
        // Increment res by 1
        res++;

        // Update target
        target = max_range + 1;

        // Increment i by 1
        i++;
    }
    // Return res as answer
    return res;
}

// Drive code.
int main()
{
    // Input
    int arr[] = { -1, 2, 2, -1, 0, 0 };
    int N = sizeof(arr) / sizeof(arr[0]);

    // Function call
    printf("%d\r\n", minSprinklers(arr, N));
}