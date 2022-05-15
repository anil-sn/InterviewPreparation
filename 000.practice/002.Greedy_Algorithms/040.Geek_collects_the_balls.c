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
    Geek collects the balls
        There are two parallel roads, each containing N and M buckets, respectively.
        Each bucket may contain some balls. The balls in first road are given in an array a and
        balls in the second road in an array b. The buckets on both roads are kept in such a way that they
        are sorted according to the number of balls in them. Geek starts from the end of the road which has
        the bucket with a lower number of balls (i.e. if buckets are sorted in increasing order, then geek
        will start from the left side of the road).

        Geek can change the road only at a point of intersection i.e. a point where buckets have the
        same number of balls on two roads. Help Geek collect the maximum number of balls.

    Example 1:
        Input:
            N = 5, M = 5
            a[] = {1, 4, 5, 6, 8}
            b[] = {2, 3, 4, 6, 9}
        Output: 29

        Explanation: The optimal way to get the maximum number of balls is to start from road 2.
        Get 2+3. Then switch at intersection
        point 4. Get 4+5+6. Then switch at intersection
        point 6. Get 9. Total = 2+3+4+5+6+9 = 29.

    Example 2:
        Input:
        N = 3, M = 3
        a[] = {1, 2, 3}
        b[] = {4, 5, 6}
        Output: 15

    Your Task:
        Your task is to complete the function maxBalls() which takes N, M, a[] and b[] as input parameters
        and returns the maximum number of balls that can be collected.
*/
int maxBalls(int n, int m, int a[], int b[])
{
    int asum = 0;
    int bsum = 0;
    int ans = 0;
    int i = 0;
    int j = 0;

    while (i < n && j < m) {

        // Repeated size bucket in Road A
        while (i+1 < n && a[i] == a[i+1]) {
            printf(">> Collecting %d balls from Road A \r\n", a[i]);
            asum += a[i++];
        }

        // Repeated size bucket in Road B
        while (j+1 < m && b[j] == b[j+1]) {
            printf(">> Collecting %d balls from Road B \r\n", b[j]);
            bsum += b[j++];
        }

        if (a[i] == b[j]) {
            printf("We have collected %d balls from Road A \r\n", asum);
            printf("We have collected %d balls from Road B \r\n", bsum);
            printf ("We can crossover to other side now as current bucket has same number of balls : %d\r\n", a[i]);
            ans += max(asum, bsum) + a[i];
            printf("We have collected %d balls from as of now \r\n", ans);
            asum = 0;
            bsum = 0;
            i++;
            j++;
        } else if (a[i] < b[j]) {
            printf(">> Collecting %d balls from Road A \r\n", a[i]);
            asum += a[i++];
        } else {
            bsum += b[j++];
            printf(">> Collecting %d balls from Road B %d \r\n", b[j]);
        }
    }

    while (i < n ) {
        printf("Collecting Leftover %d balls from Road A \r\n", a[i]);
        asum += a[i++];
    }

    while (j < m ) {
        printf("Collecting Leftover %d balls from Road B \r\n", b[j]);
        bsum += b[j++];
    }

    printf("We have collected %d balls from as of now from Road A \r\n", asum);
    printf("We have collected %d balls from as of now from Road B \r\n", bsum);
    ans += max(asum, bsum);
    printf("So Total we have collected %d balls from as of now \r\n", ans);

    return ans;
}

int main ()
{
    int N = 5;
    int M = 5;
    int a[] = {1, 4, 5, 6, 8};
    int b[] = {2, 3, 4, 6, 9};

    printf ("Road A: [1, 4, 5, 6, 8] \r\n");
    printf ("Road B: [2, 3, 4, 6, 9] \r\n");
    printf ("Maximum balls collected is : %d\r\n", maxBalls(N, M, a, b));

    return 0;
}