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
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*
    There are N stations on route of a train. The train goes from station 0 to N-1. The ticket cost for all pair of stations (i, j) is given where j is greater than i. Find the minimum cost to reach the destination.
    Consider the following example:


    Input:
    cost[N][N] = { {0, 15, 80, 90},
                {INF, 0, 40, 50},
                {INF, INF, 0, 70},
                {INF, INF, INF, 0}
                };
    There are 4 stations and cost[i][j] indicates cost to reach j
    from i. The entries where j < i are meaningless.

    Output:
    The minimum cost is 65
    The minimum cost can be obtained by first going to station 1
    from 0. Then from station 1 to station 3.

    The minimum cost to reach N-1 from 0 can be recursively written as following:

    minCost(0, N-1) = MIN { cost[0][n-1],
                            cost[0][1] + minCost(1, N-1),
                            minCost(0, 2) + minCost(2, N-1),
                            ........,
                            minCost(0, N-2) + cost[N-2][n-1] }
*/
// infinite value
#define INF INT_MAX

// Number of stations
#define N 4

// A C++ recursive function to find the shortest path from
// source 's' to destination 'd'.
int minCostRec(int cost[][N], int s, int d)
{
    // If source is same as destination or destination is next to source
    if (s == d || s+1 == d) {
      return cost[s][d];
    }

    // Initialize min cost as direct ticket from
    // source 's' to destination 'd'.
    int min = cost[s][d];

    // Try every intermediate vertex to find minimum
    for (int i = s+1; i<d; i++)
    {
        int c = minCostRec(cost, s, i) + minCostRec(cost, i, d);
        if (c < min) {
           min = c;
        }
    }
    return min;
}

// This function returns the smallest possible cost to
// reach station N-1 from station 0. This function mainly
// uses minCostRec().
int minCost(int cost[][N])
{
    return minCostRec(cost, 0, N-1);
}

// Driver program to test above function
int main()
{
    int cost[N][N] = {{0,   15,  80,  90},
                      {INF, 0,   40,  50},
                      {INF, INF, 0,   70},
                      {INF, INF, INF, 0}};
    printf("The Minimum cost to reach station %d is %d\r\n", N, minCost(cost));
    return 0;
}