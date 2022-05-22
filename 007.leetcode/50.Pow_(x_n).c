#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

double
myPow (double x, int n)
{
    if (n == 0) {
        return 1;
    }

    int originalN = n;
    n = abs (n);

    if ((n % 2) == 0) {
        double temp = myPow (x, n / 2);
        return (originalN > 0) ? (temp * temp) : (1 / (temp * temp));
    } else {
        double temp = myPow (x, (n - 1) / 2);
        return (originalN > 0) ? (x * temp * temp) : (1 / (x * temp * temp));
    }
}