#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int
cmpfunc (int **a, int **b)
{
    return (*a)[0] - (*b)[0];
}

int
minimumLines (int **stockPrices, int stockPricesSize, int *stockPricesColSize)
{
    qsort (stockPrices, stockPricesSize, sizeof (int *), cmpfunc); //** sorted daywise

    if (stockPricesSize <= 1) {
        return 0;
    }

    int lines = 1;

    for (int i = 1; i < stockPricesSize - 1; i++) {
        long double x1 = stockPrices[i - 1][0];
        long double y1 = stockPrices[i - 1][1];
        long double x2 = stockPrices[i][0];
        long double y2 = stockPrices[i][1];
        long double x3 = stockPrices[i + 1][0];
        long double y3 = stockPrices[i + 1][1];
        long double a = (long double) (y2 - y1) / (x2 - x1); // slope of line joining pts i , i-1
        long  double b = (long double) (y3 - y2) / (x3 - x2); // slope of line joining pts i+1,i

        if (a != b) {
            // since ith point is common , if slope a and b are equal then it is a straight line otherwise its not
            lines ++;
        }
    }

    return lines;
}
