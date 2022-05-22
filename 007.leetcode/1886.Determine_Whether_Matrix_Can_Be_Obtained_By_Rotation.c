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
    Some explanation, suppose we are at index i, j, let us find a relation of this position with it's position in various rotations
    i. e
    0deg rotation : i, j will point to i, j
    90deg rotation : i, j will point to j, n - i - 1
    180deg rotation : i, j will point to n - i - 1, n - j - 1
    270deg rotation : i, j will point to n - j - 1, i

    We denote these rotations by boolean variables, initially we assume all these rotations are valid, so we set them all to true
    here c[0] denotes 0deg rotation, c[1] -> 90deg, c[2] -> 180 deg, c[3] -> 270deg

    Then, we loop over all elements and check if that particular rotation is valid or not. If not, we turn it to false.
    In the end, we simply check if any of the rotations is still possible or not.

    This same code can also be used to find out the valid rotation, i,e 0deg / 90deg / 180 deg/ 270 deg by checking the boolean value in c[0], c[1]. c[2], c[3] respectively.
*/

bool
findRotation (int **mat, int matSize, int *matColSize, int **target, int targetSize, int *targetColSize)
{
    bool c[4];
    memset (c, true, sizeof (c));
    int n = matSize;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (mat[i][j] != target[i]    [j]) {
                c[0] = false;    //   0deg rotation
            }

            if (mat[i][j] != target[n - j - 1][i]) {
                c[1] = false;    //  90deg rotation
            }

            if (mat[i][j] != target[n - i - 1][n - j - 1]) {
                c[2] = false;    // 180deg rotation
            }

            if (mat[i][j] != target[j]    [n - i - 1]) {
                c[3] = false;    // 270deg rotation
            }
        }
    }

    return c[0] || c[1] || c[2] || c[3];
}