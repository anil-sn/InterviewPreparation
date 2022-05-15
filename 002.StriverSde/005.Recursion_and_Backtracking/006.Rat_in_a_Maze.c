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
    Rat in a Maze

    Consider a rat placed at (0, 0) in a square matrix of order N * N.
    It has to reach the destination at (N – 1, N – 1).
    Find all possible paths that the rat can take to reach from source to destination.
    The directions in which the rat can move are ‘U'(up), ‘D'(down), ‘L’ (left), ‘R’ (right).
    Value 0 at a cell in the matrix represents that it is blocked and the rat cannot move to
    it while value 1 at a cell in the matrix represents that rat can travel through it.

    Note: In a path, no cell can be visited more than one time.
        Print the answer in lexicographical(sorted) order

    Examples:

        Example 1:
            Input:
            N = 4
            m[][] = {{1, 0, 0, 0},
                     {1, 1, 0, 1},
                     {1, 1, 0, 0},
                     {0, 1, 1, 1}}

            Output: DDRDRR DRDDRR

            Explanation: The rat can reach the destination at (3, 3) from (0, 0) by two paths
            - DRDDRR and DDRDRR, when printed in sorted order we get DDRDRR DRDDRR.

    Example 2:
        Input: N = 2
            m[][] = {{1, 0},
                     {1, 0}}
        Output:
        No path exists and the destination cell is blocked.
 */

char move[100];
int pos;

void solve(int i, int j, int a[4][4], int n, char move[], int pos, int vis[4][4])
{
    if (i == n - 1 && j == n - 1) {
      printf("%s \r\n", move);
      return;
    }

    // downward
    if (i + 1 < n &&
        vis[i + 1][j] == 0 &&
        a[i + 1][j] == 1) {

        vis[i][j] = 1;
        move[pos++] = 'D';

        solve(i + 1, j, a, n, move, pos, vis);

        move[pos--] = '\0';
        vis[i][j] = 0;
    }

    // left
    if (j - 1 >= 0 &&
        vis[i][j - 1] == 0
        && a[i][j - 1] == 1) {

        vis[i][j] = 1;
        move[pos++] = 'L';

        solve(i, j - 1, a, n, move, pos, vis);

        move[pos--] = '\0';
        vis[i][j] = 0;
    }

    // right
    if (j + 1 < n &&
        vis[i][j + 1] == 0 &&
        a[i][j + 1] == 1) {

        vis[i][j] = 1;
        move[pos++] = 'R';

        solve(i, j + 1, a, n, move, pos, vis);

        move[pos--] = '\0';
        vis[i][j] = 0;
    }

    // upward
    if (i - 1 >= 0 &&
        vis[i - 1][j] == 0 &&
        a[i - 1][j] == 1) {

        vis[i][j] = 1;
        move[pos++] = 'U';

        solve(i - 1, j, a, n, move, pos, vis);

        move[pos--] = '\0';
        vis[i][j] = 0;
    }

    return;
}

void findPath(int m[4][4], int n)
{
    int vis[4][4];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vis[i][j] = 0;
        }
    }

    memset(move, '\0', sizeof(move));
    pos = 0;

    if (m[0][0] == 1) {
        solve(0, 0, m, n, move, pos, vis);
    }

    return;
}

int main()
{
    int n = 4;
    int a[4][4] =   {{1,0,0,0},
                    {1,1,0,1},
                    {1,1,0,0},
                    {0,1,1,1}};

    findPath(a, n);
}
