#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void spiralOrder(int row, int col, int A[row][col]) {
    
    int top         = 0;
    int bottom      = row-1;
    int left        = 0;
    int right       = col -1;
    int direction   = 0;

    int i;
    int ans[100];
    int pos = 0;
    
    while(top <= bottom && left <= right)
    {
        if(direction==0)
        {
            for(i=left;i<=right;i++)
                ans[pos++] = (A[top][i]);
            top++;
        }
        else if(direction==1)
        {
            for(i=top;i<=bottom;i++)
                ans[pos++] = (A[i][right]);
            right--;
        }
        else if(direction==2)
        {
            for(i=right;i>=left;i--)
                ans[pos++] = (A[bottom][i]);
            bottom--;
        }
        else if(direction==3)
        {
            for(i=bottom;i>=top;i--)
                ans[pos++] = (A[i][left]);
            left++;
        }

        direction = (direction+1)%4;
    }

    for (int x = 0; x < pos; x++) {
        printf("%d ", ans[x]);
    }
    printf("\r\n");

    return;
}

int main (void) 
{
    int row = 5;
    int col = 6;
    int matrix [5][6] = {
                            { 1,     2,      3,      4,      5,      6},
                            { 7,     8,      9,     10,     11,     12},
                            {13,    14,     15,     16,     17,     18},
                            {19,    20,     21,     22,     23,     24},
                            {25,    26,     27,     28,     29,     30}
                        };

    spiralOrder(row, col, matrix);
}