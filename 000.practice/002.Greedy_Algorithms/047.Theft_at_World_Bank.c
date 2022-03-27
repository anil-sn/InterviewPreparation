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
    The world’s most successful thief Albert Spaggiari was planning for his next heist on
    the world bank. He decides to carry a sack with him, which can carry a maximum weight
    of C kgs. Inside the world bank there were N large blocks of gold. All the blocks have
    some profit value associated with them i.e. if he steals ith block of weight w[i] then
    he will have p[i] profit. As blocks were heavy he decided to steal some part of them
    by cutting them with his cutter.
    The thief does not like symmetry, hence, he wishes to not take blocks or parts of
    them whose weight is a perfect square. Now, you need to find out the maximum profit
    that he can earn given that he can only carry blocks of gold in his sack.
    Note: The answer should be precise upto 3 decimal places.

    Example 1 :
        N = 3, C = 10
        w[] = {4, 5, 7}
        p[] = {8, 5, 4)
    Output:
        7.857
    Explanation: As first block’s weight is 4 which is a perfect square, he will not use
        this block. Now with the remaining blocks the most optimal way is to use 2nd block
        completely and cut 5kg piece from the 3rd block to get a total profit of 5 + 2.857
        = 7.857

    Approach: The given problem can be solved using a greedy algorithm known as
    fractional Knapsack. Follow the steps below to solve the problem:

    1. Initialize a vector of pairs say V to store the pairs formed by taking array
        elements from the array B[] as the first element and the array elements from
        array B[] as the second element present at the same index.
    2. Iterate over the range [0, N] and if A[i] is not a, perfect square then push
        the pair {B[i], A[i]} into the vector V.
    3. Sort the vector V in descending order by a custom comparator by the ratios of the pairs.
    4. Initialize a variable say profit as 0 to store the maximum profit.
    5. Traverse the vector V and perform the following steps in each iteration:
        5.1. If the second element of the current pair is less than or equal to W,
             then increment profit by the first element of the pair, i.e taking the
             whole block of gold and then decrease W by the second element of the
             current pair.
        5.2. Otherwise, take an amount equal to the ratio of W and the second element
            of the current pair and store it in a variable P. Then increment profit by
            P*first element of the current pair and then break.
    6. After completing the above steps, print the value obtained in profit as the answer.
*/

typedef struct pair {
    long long weight;
    long long profit;
}PAIR;

// Custom comparator
int comp(const void *p1, const void *p2)
{
    PAIR *a = (PAIR *)p1;
    PAIR *b = (PAIR *)p2;

    long double r1 = a->profit / a->weight;
    long double r2 = b->profit / b->weight;

    return r1 > r2;
}

// Function to find the maximum profit
long double maximumProfit(long double W[], long double P[], int N, long long C)
{
    // Stores the pairs of elements of P and W at the same index
    PAIR V[N+1];
    int pairPos = 0;

    // Iterate over the range [0, N]
    for (int i = 0; i < N; i++) {

        long double temp = sqrt(W[i]);

        // If current integer is perfect square
        if (temp * temp == W[i])
            continue;

        // Push the pair of B[i] and W[i] in vector V
        V[pairPos].weight = W[i];
        V[pairPos].profit = P[i];
        pairPos++;
    }

    // Sorts the vector using the custom comparator
    qsort(V, pairPos, sizeof(V[0]), comp);

    // Stores the maximum profit
    long double profit = 0.00;

    // Traverse the vector V
    for (int i = 0; i < pairPos; i++) {

        // If V[i].second is less than C
        if (V[i].weight <= C) {

            // Increment profit by V[i].profit
            profit += V[i].profit;

            // Decrement V[i].second
            // from W
            C -= V[i].weight;
        } else {
            // Update profit
            profit += V[i].profit * ((long double)C / V[i].weight);
            break;
        }
    }
    // Return the value of profit
    return profit;
}

// Driver Code
int main()
{
    int N = 3;
    long long C = 10;
    long double W[] = { 4, 5, 7 };
    long double P[] = { 8, 5, 4 };

    printf("Maximum Profit : %Lf\r\n", maximumProfit(W, P, N, C));
    return 0;
}