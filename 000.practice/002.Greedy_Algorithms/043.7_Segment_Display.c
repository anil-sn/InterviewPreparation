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
    7 Segment Display

    Light Emitting Diode (LED) is the most widely used semiconductor which emits either visible light
    or invisible infrared light when forward biased. Remote controls generate invisible light.
    A Light emitting diode (LED) is an optical electrical energy into light energy when voltage is
    applied.

    Seven Segment Displays:
    Seven segment displays are the output display device that provide a way to display information
    in the form of image or text or decimal numbers which is an alternative to the more complex dot
    matrix displays. It is widely used in digital clocks, basic calculators, electronic meters, and
    other electronic devices that display numerical information. It consists of seven segments of
    light emitting diodes (LEDs) which is assembled like numerical 8.

    Using seven segment display, you can write down any digit from 0 to 9 using at most seven
    segments. Given a positive number N and then a string S representing a number of N digits.
    You have to rearrange the segments in this N digit number to get the smallest possible
    N digit number. This number can have leading zeros. You can not waste any segment or
    add any segment from your own. You have to use all of the segments of the given N digits.

    Example 1:
        Input:
            N = 6
            S = "234567"
        Output:
            000011
        Explanation:
            234567 has a total of 28 segments and
            we can rearrange them to form 000011
            which has 28 segments too. 000011 is the
            smallest possible number which can be
            formed on rearrangement.
    Example 2:
        Input:
            N = 3
            S = "011"
        Output:
            011
        Explanation:
            011 has a total of 10 segments and this
            is the smallest number with 10 segments.

    Your Task:
    Complete the function sevenSegments() which takes an Integer N and a String S of length N
    as input and returns the smallest possible N digit number which can be made using all of
    the segments given.
*/
                   //   0  1  2  3  4  5  6  7  8  9
int noOfSegments[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 5};
#define MAX_SEGMENT 7

void sevenSegments(char S[], int N) {

    int sum = 0;
    for(int i=0; i<N; i++) {
        sum += noOfSegments[S[i]-'0']; // 5+5+...
    }

    /* 2 segments is the minimum so for all digits give two segments */
    sum -= 2*N;
    //output ->2 2 2 2 2 2
    //         1 1 1 1 1 1
    int output[N][2];
    for (int i=0; i < N; i++){
        output[i][0] = 2;
        output[i][1] = 1;
    }
    /* try adding segments by making MSP as 0 which has second highest segments */
    int i =0;
    while(i<N && sum>=4)
    {
        sum = sum - 4;
        output[i][0] = 6;
        output[i][1] = 0;
        i++;
    }

    // Case S = "34782"
    // sum = 2
    // ouput 6 6 6 2 2
    //       0 0 0 1 1
    // We have sum remaining
    i = N-1;
    while(i >= 0 && sum)
    {
        /* Start adding 8 at the end which has max 7 segments used up */
        if(sum > MAX_SEGMENT - output[i][0])
        {
            sum -= MAX_SEGMENT - output[i][0]; // 1
            output[i][0] = 7;
            output[i][1] = 8;
            i--;
        } else {
            output[i][0] += sum;
            if (sum == 2) {
                output[i][1] = 1;
            } else if (sum == 3) {
                output[i][1] = 7;
            } else if (sum == 4) {
                output[i][1] = 4;
            } else if (sum == 5) {
                output[i][1] = 2;
            } else if (sum == 6) {
                output[i][1] = 0;
            }
            sum=0;
        }
    }
    for (int i=0; i < N; i++){
        printf("%d", output[i][1]);
    }
    printf("\r\n");
}
int main () {
    int N = 6;
    char S[] = "234567";
    sevenSegments(S, N);

    return 0;
}