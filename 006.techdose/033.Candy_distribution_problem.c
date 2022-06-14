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
    Assuming current position is i and there are 3 cases for each i as following:

    ratings[i - 1] < ratings[i]
    ratings[i - 1] == ratings[i]
    ratings[i - 1] > ratings[i]
    For case 1, it's the simplest. If the previous child gets n candies, we give the i-th child n + 1 candies.

    For case 2, you can give any number but 0 to the i-th child and this is still compliant to the rules. But to minimize the number, you have to give the i-th child a single candy.

    For case 3, it's a little bit complicated and let's discuss it by a simple example.

    Given an input array [2, 3, 4, 3, 2, 1] and then we go through the process step by step.

    i = 0, child[0] = 1

    i = 1, child[1] = 2

    i = 2, child[2] = 3

    i = 3, child[3] = 1 (to minimize the number, we give the child a single candy)

    i = 4, child[4] = 1

    At this point, child[3] == child[4]. Therefore, we have to go back to increase child[3] by 1, i.e, child[3] = 2.

    i = 5, child[5] = 1

    At this point, child[4] == child[5]. We increase child[4] by 1.

    However, the action affects child[2] and child[3] and we have to give them more candies.

    Now let's look at the final optimal candy sequence carefully and we can see the subsequence after the third child is a reversed continuous series starting from 1.

    [1 2 4 3 2 1]

    Now let's reverse the subsequence and get a series starting from 1. At this point, we know we only need to count the number of lower ratings (compared to the rating of the third child) [defined as lo] and add it to the original amount of candy we gave away.

    [1 2 4 1 2 3]

    We record the number of candies as hi when we encounter higher or the same rating and reset lo to 0.

    When the number of lower ratings lo is equal to hi, we increase hi by 1.

    Here is the concise C code.
*/
#define max(a,b) (a>b?a:b)

int candy(int* ratings, int size) {

	 if(size<=1)
		 return size;

    int num[size];
	 for (int i = 0; i < size; i++)
	 {  
        num[i] = 1;
		printf("%d ", num[i]);
	 }
     printf("\r\n");

	 for (int i = 1; i < size; i++)
	 {
		 if(ratings[i]>ratings[i-1])
			 num[i]=num[i-1]+1;
	 }

	 for (int i= size-1; i>0 ; i--)
	 {
		 if(ratings[i-1]>ratings[i])
			 num[i-1]=max(num[i]+1,num[i-1]);
	 }

	 int result=0;
	 for (int i = 0; i < size; i++)
	 {
		 result+=num[i];
	 }

	 return result;
}
#if 0
int candy(int* ratings, int ratingsSize) {
    int i, sum = 1, cur = 1, lo = 0, hi = 1;
    if (0 >= ratingsSize || !ratings) {
        return 0;
    }
    for (i = 1; i < ratingsSize; ++i) {
        if (ratings[i] > ratings[i - 1]) {
            lo = 0;
            sum += ++cur;
            hi = cur;
        } else if (ratings[i] == ratings[i - 1]) {
            lo = 0;
            cur = 1;
            sum += cur;
            hi = cur;
        } else {
            ++lo;
            if (lo == hi) {
                hi++;
                sum += lo + 1;
            } else {
                sum += lo;
            }
            cur = 1;
        }
    }
    return sum;
}
#endif

int main (void)
{
    int childrenRating[] = {1, 0, 2};
    int n = 3;

    printf("total number of candies %d \r\n", candy(childrenRating, n));
    return 0;
}