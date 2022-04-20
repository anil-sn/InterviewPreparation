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
    Given a cost matrix cost[][] and a position (m, n) in cost[][],
    write a function that returns cost of minimum cost path to reach (m, n)
    from (0, 0). Each cell of the matrix represents a cost to traverse through
    that cell. The total cost of a path to reach (m, n) is the sum of all the
    costs on that path (including both source and destination). You can only
    traverse down, right and diagonally lower cells from a given cell, i.e.,
    from a given cell (i, j), cells (i+1, j), (i, j+1), and (i+1, j+1) can be
    traversed. You may assume that all costs are positive integers.

    For example, in the following figure, what is the minimum cost path to (2, 2)?

    1  2  3
    4  8  2
    1  5  3

    The path with minimum cost is highlighted in the following figure.
    The path is (0, 0) –> (0, 1) –> (1, 2) –> (2, 2).
    The cost of the path is 8 (1 + 2 + 2 + 3).

    1) Optimal Substructure
        The path to reach (m, n) must be through one of the 3 cells:
        (m-1, n-1) or (m-1, n) or (m, n-1).
        So minimum cost to reach (m, n) can be written as “minimum of the 3 cells plus cost[m][n]”.
        minCost(m, n) = min (minCost(m-1, n-1), minCost(m-1, n), minCost(m, n-1)) + cost[m][n]

    2) Overlapping Subproblems
        Following is a simple recursive implementation of the MCP (Minimum Cost Path) problem.
        The implementation simply follows the recursive structure mentioned above.

    Time Complexity: O(m * n)

    It should be noted that the above function computes the same subproblems again and again.
    See the following recursion tree, there are many nodes which appear more than once.
    The time complexity of this naive recursive solution is exponential and it is terribly slow.

    mC refers to minCost()
                                        mC(2, 2)
                                 /            |           \
                                /             |            \
                        mC(1, 1)           mC(1, 2)             mC(2, 1)
                    /     |     \       /     |     \           /     |     \
                    /      |      \     /      |      \         /      |       \
            mC(0,0) mC(0,1) mC(1,0) mC(0,1) mC(0,2) mC(1,1) mC(1,0) mC(1,1) mC(2,0)
*/

#define R 3
#define C 3

int min(int x, int y, int z);

/* Returns cost of minimum cost path from (0,0) to (m, n) in mat[R][C]*/
int minCost(int cost[R][C], int m, int n)
{
   if (n < 0 || m < 0)
      return INT_MAX;
   else if (m == 0 && n == 0)
      return cost[m][n];
   else
      return cost[m][n] + min( minCost(cost, m-1, n-1),
                               minCost(cost, m-1, n),
                               minCost(cost, m, n-1) );
}

/* A utility function that returns minimum of 3 integers */
int min(int x, int y, int z)
{
   if (x < y)
      return (x < z)? x : z;
   else
      return (y < z)? y : z;
}

/* Driver program to test above functions */
int main()
{
   int cost[R][C] = { {1, 2, 3},
                      {4, 8, 2},
                      {1, 5, 3} };
    printf("1\t2\t3\r\n4\t8\t2\r\n1\t5\t3\r\n");
    printf("Minimum cost path from (0,0) to (2, 2) is equal to %d \r\n", minCost(cost, 2, 2));
   return 0;
}