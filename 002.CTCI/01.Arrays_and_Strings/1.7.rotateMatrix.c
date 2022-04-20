#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
/*
    Rotate Matrix: Given an image represented by an NxN matrix, where each pixel in the image is 4
    bytes, write a method to rotate the image by 90 degrees. Can you do this in place?

    SOLUTION
    Because we're rotating the matrix by 90 degrees, the easiest way to do this is to implement the rotation in
    layers. We perform a circular rotation on each layer, moving the top edge to the right edge, the right edge
    to the bottom edge, the bottom edge to the left edge, and the left edge to the top edge.

    How do we perform this four-way edge swap? One option is to copy the top edge to an array, and then
    move the left to the top, the bottom to the left, and so on. This requires O(N) memory, which is actually
    unnecessary.

    A better way to do this is to implement the swap index by index. In this case, we do the following:
    for i = 0 to n
        temp= top[i];
        top[i] = left[i]
        left[i] = bottom[i]
        bottom[i] = right[i]
        right[i] = temp

    We perform such a swap on each layer, starting from the outermost layer and working our way inwards.
    (Alternatively, we could start from the inner layer and work outwards.)

*/

void displayMatrix(u_int32_t p[4][4], u_int32_t n)
{
    u_int32_t row, col;

    for (row = 0; row < n; row++)
    {
        for (col = 0; col < n; col++) {
            printf("%d\t", p[row][col]);
        }

        printf("\n");
    }

    printf("\n\n");
}

void rotate(u_int32_t pSrc[4][4], u_int32_t n)
{
    u_int32_t r, c;
    u_int32_t temp[4] = {0,0,0,0};

    {
        int start = 0, end = n - 1;
        int temp = 0;
        //save left vertical line to temp and rotate 90 degree line by line
        while( start < end ){
            for( int i = 0; i < end - start; i++ )
            {
                temp = pSrc[ start ][ start + i ];
                pSrc[ start ][ start + i ] = pSrc[ end - i ][ start ];
                pSrc[ end - i ][ start ] = pSrc[ end ][ end - i ];
                pSrc[ end ][ end - i ] = pSrc[ start + i ][ end ];
                pSrc[ start + i ][ end ] = temp;
            }
            start++;
            end--;
        }
    }

}

int main()
{
    u_int32_t image[4][4] = {{1,2,3,4},
                            {5,6,7,8},
                            {9,10,11,12},
                            {13,14,15,16}};
    u_int32_t *pSrc;
    u_int32_t *pDst;
    u_int32_t n;

    // setting initial values and memory allocation
    n = 4, pSrc = (u_int32_t *)image;

    // process each buffer
    printf("Input Matrix : \r\n");
    displayMatrix(pSrc, n);

    rotate(pSrc, n);

    printf("90 Degrees Rotated Matrix : \r\n");
    displayMatrix(pSrc, n);

    return 0;
}
