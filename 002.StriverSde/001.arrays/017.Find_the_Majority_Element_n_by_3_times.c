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
    Majority Elements(>N/3 times) | Find the elements that appears more than N/3 times
    in the array
    Problem Statement: Given an array of N integers. Find the elements that appears
    more than N/3 times in the array. If no such element exists, return an empty vector.

    Example 1:
    Input: N = 5, array[] = {1,2,2,3,2}
    Ouput: 2

    Explanation: Here we can see that the Count(1) = 1, Count(2) = 3 and Count(3) = 1.T
    herefore, the count of 2 is greater than N/3 times. Hence, 2 is the answer.

    Example 2:
    Input:  N = 6, array[] = {11,33,33,11,33,11}
    Output: 11 33

    Explanation: Here we can see that the Count(11) = 3 and Count(33) = 3.
    Therefore, the count of both 11 and 33 is greater than N/3 times.
    Hence, 11 and 33 is the answer.
*/

/* Moore’s Voting Algorithm */
void majorityElement(int nums[], int len) {

    int number1 = -1;
    int number2 = -1;
    int count1 = 0;
    int count2 = 0;

    for (int i = 0; i < len; i++) {
        if (nums[i] == number1)
            count1++;
        else if (nums[i] == number2)
            count2++;
        else if (count1 == 0) {
            number1 = nums[i];
            count1 = 1;
        } else if (count2 == 0) {
            number2 = nums[i];
            count2 = 1;
        } else {
            count1--;
            count2--;
        }
    }

    count1 = 0;
    count2 = 0;
    for (int i = 0; i < len; i++) {
        if (nums[i] == number1)
            count1++;
        else if (nums[i] == number2)
            count2++;
    }

    if (count1 > len / 3)
        printf("First Number is : %d \r\n", number1);
    if (count2 > len / 3)
        printf("First Number is : %d \r\n", number2);

    return;
}

int main ()
{
    int list[] = {4,4,2,3,3,4,4,3,3,4};
    int count = 10;

    majorityElement(list, count);
    return;
}