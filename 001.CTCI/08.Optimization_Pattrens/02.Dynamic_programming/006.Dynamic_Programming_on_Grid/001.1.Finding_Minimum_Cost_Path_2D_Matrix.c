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
    Problem Statement : Given a cost matrix Cost[][] where Cost[i][j] denotes the
        Cost of visiting cell with coordinates (i,j), find a min-cost path to reach
        a cell (x,y) from cell (0,0) under the condition that you can only travel one
        step right or one step down. (We assume that all costs are positive integers)

    Solution : It is very easy to note that if you reach a position (i,j) in the grid,
        you must have come from one cell higher, i.e. (i-1,j) or from one cell to your left,
        i.e. (i,j-1). This means that the cost of visiting cell (i,j) will come from the
        following recurrence relation:

    MinCost(i,j) = min(MinCost(i-1,j),MinCost(i,j-1)) + Cost[i][j]
        The above statement means that to reach cell (i,j) wit minimum cost,
        first reach either cell(i-1,j) or cell (i,j-1) in as minimum cost as possible.
        From there, jump to cell (i,j).

    This brings us to the two important conditions
    which need to be satisfied for a dynamic programming problem:

    Optimal Sub-structure:- Optimal solution to a problem involves optimal solutions to sub-problems.

    Overlapping Sub-problems:- Subproblems once computed can be stored in a table for
    further use. This saves the time needed to compute the same sub-problems again and again.

    The problem of finding the min-Cost Path is now almost solved.
    We now compute the values of the base cases: the topmost row and
    the leftmost column. For the topmost row, a cell can be reached only from
    the cell on the left of it. Assuming zero-based index,

    MinCost(0,j) = MinCost(0,j-1) + Cost[0][j]
    i.e. cost of reaching cell (0,j) = Cost of reaching cell (0,j-1) +
                                       Cost of visiting cell (0,j) Similarly,

    MinCost(i,0) = MinCost(i-1,0) + Cost[i][0]
    i.e. cost of reaching cell (i,0) = Cost of reaching cell (i-1,0) +
                                       Cost of visiting cell (i,0)
*/

#define F(i,a,b) for(int i = (int)(a); i <= (int)(b); i++)
#define RF(i,a,b) for(int i = (int)(a); i >= (int)(b); i--)

int main()
{
    int X,Y;    //X:number of rows, Y: number of columns
    X = Y = 10; //assuming 10X10 matrix
    int Cost[X][Y];

    F(i,0,X-1)
    {
        F(j,0,Y-1)
        {
            //Take input the cost of visiting cell (i,j)
            Cost[i][j] = (((i+j)%10 == 0) ? (i+1) : ((i+j)%10));
        }
    }

    F(i,0,X-1) {
        F(j,0,Y-1) {
            //Take input the cost of visiting cell (i,j)
            printf("\t%d", Cost[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    int MinCost[X][Y]; //declare the minCost matrix

    MinCost[0][0] = Cost[0][0];

    // initialize first row of MinCost matrix
    F(j,1,Y-1)
        MinCost[0][j] = MinCost[0][j-1] + Cost[0][j];

    //Initialize first column of MinCost Matrix
    F(i,1,X-1)
        MinCost[i][0] = MinCost[i-1][0] + Cost[i][0];

    //This bottom-up approach ensures that all the sub-problems needed
    // have already been calculated.
    F(i,1,X-1)
    {
        F(j,1,Y-1)
        {
            //Calculate cost of visiting (i,j) using the
            //recurrence relation discussed above
            MinCost[i][j] = min(MinCost[i-1][j],MinCost[i][j-1]) + Cost[i][j];
        }
    }

    printf("Minimum cost from (0,0) to (%d,%d) is %d\r\n",X, Y, MinCost[X-1][Y-1]);
    return 0;
}