#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define SWAP(a_m, b_m)  (((a_m) ^= (b_m)), ((b_m) ^= (a_m)), ((a_m) ^= (b_m)))
#define MIN(a_m,b_m)    (b_m ^ ((a_m ^ b_m) & -(a_m < b_m)))
#define MAX(a_m,b_m)    (a_m ^ ((a_m ^ b_m) & -(a_m < b_m)))
#define SETBIT(flag_m, mask_m, val_m) (val_m = (val_m & ~mask_m) | (-flag_m & mask_m))
#define MERGE(a_m,b_m,r_m,mask_m) (r_m = a_m ^ ((a_m ^ b_m) & mask_m))

// Print a digit in binary
void bin(int n, char reason[]) {
    int i;
    printf("0");
    for (i = 1 << 30; i > 0; i = i / 2) {
        if((n & i) != 0) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("[ %d ]: %s ", n, reason);
    printf("\r\n");
}

/*
Method (Recursive)

    Approach:
Example:

0  -> 0000000     8  -> 001000      16  -> 010000    24 -> 011000

1  -> 0000001     9  -> 001001      17  -> 010001    25 -> 011001

2  -> 0000010    10  -> 001010     18 -> 010010     26 -> 011010

3  -> 0000011    11  -> 001011     19  -> 010010    27 -> 011011

4  -> 0000100    12  -> 001100     20  -> 010100    28 -> 011100

5  -> 0000101    13  -> 001101     21 -> 010101     29 -> 011101

6  -> 0000110    14  -> 001110     22 -> 010110     30 -> 011110

7  -> 0000111    15  -> 001111     23 -> 010111     31 -> 011111

Input: N = 4

Output: 5

Input: N = 17

Output: 35

Approach : Pattern recognition

    Let ‘N’ be any arbitrary number and consider indexing from right to left(rightmost being 1); then  nearestPow = pow(2,i).
    Now, when you write all numbers from 1 to N, you will observe the pattern mentioned below:
    For every index i, there are exactly nearestPow/2 continuous elements that are unset followed by nearestPow/2 elements that are set.
    Throughout the solution, i am going to use this concept.

    You can clearly observe above concept in the above table.
    The general formula that i came up with:
    a. addRemaining = mod – (nearestPos/2) + 1 iff mod >= nearestPow/2;
    b. totalSetBitCount = totalRep*(nearestPow/2) + addRemaining

    where totalRep -> total number of times the pattern repeats at index i
                addRemaining -> total number of set bits left to be added after the pattern is exhausted

    Eg:  let N = 17

        leftMostSetIndex = 5 (Left most set bit index, considering 1 based indexing)

        i = 1 => nearestPos = pow(2,1) = 2;   totalRep = (17+1)/2 = 9 (add 1 only for i=1)

                        mod = 17%2 = 1

                        addRemaining = 0 (only for base case)

                        totalSetBitCount = totalRep*(nearestPos/2) + addRemaining = 9*(2/2) + 0 = 9*1 + 0 = 9

        i = 2 => nearestPos = pow(2, 2)=4;       totalRep = 17/4 = 4

                        mod = 17%4 = 1

                        mod(1) < (4/2) => 1 < 2 => addRemaining = 0

                        totalSetBitCount  = 9 + 4*(2) + 0 = 9 + 8 = 17

        i = 3 => nearestPow = pow(2,3) = 8;    totalRep = 17/8 = 2

                        mod = 17%8 = 1

                        mod < 4 => addRemaining = 0

                        totalSetBitCount = 17 + 2*(4) + 0 = 17 + 8 + 0 = 25

        i = 4 => nearestPow = pow(2, 4) = 16; totalRep = 17/16 = 1

                    mod = 17%16 = 1

                    mod < 8 => addRemaining = 0

                    totalSetBitCount  = 25 + 1*(8) + 0 = 25 + 8 + 0 = 33

    We cannot simply operate on the next power(32) as 32>17. Also, as the first half bits will be 0s only, we need to find the distance of the given number(17) from the last power to directly get the number of 1s to be added

    i = 5 => nearestPow = (2, 5) = 32 (base case 2)

                    lastPow = pow(2, 4) = 16

                    mod = 17%16 = 1

                    totalSetBit = 33 + (mod+1) = 33 + 1 + 1 = 35

    Therefore, total num of set bits from 1 to 17 is 35
    Try iterating with N = 30, for better understanding of the solution.
*/

int GetLeftMostSetBit(int n){
    int pos = 0;

    while(n>0){
        pos++;
        n>>=1;
    }

    return pos;
}

int countSetBits(int n){
    int leftMostSetBitInd = GetLeftMostSetBit(n);

    int totalRep, mod;
    int nearestPow;
    int totalSetBitCount = 0;
    int addRemaining=0;

    int curr=0; // denotes the number of set bits at index i

    //cout<<"leftMostSetBitInd: "<<leftMostSetBitInd<<endl;

    for(int i=1; i<=leftMostSetBitInd; ++i){
        nearestPow = pow(2, i);
        if(nearestPow>n){
            int lastPow = pow(2, i-1);
            mod = n%lastPow;
            totalSetBitCount += mod+1;
        }
        else{
            if(i==1 && n%2==1){
                totalRep = (n+1)/nearestPow;
                mod = nearestPow%2;
                addRemaining =  0;
            }
            else{
                totalRep = n/nearestPow;
                mod = n%nearestPow;

                if(mod >= (nearestPow/2)){
                    addRemaining = mod - (nearestPow/2) + 1;
                }else{
                    addRemaining = 0;
                }
            }

            curr = totalRep*(nearestPow/2) + addRemaining;
            totalSetBitCount += curr;
        }
        // debug output at each iteration
        //cout<<i<<" "<<nearestPow<<" "<<totalRep<<" "<<mod<<" "<<totalSetBitCount<<" "<<curr<<endl;
    }

    return totalSetBitCount;
}

int main()
{
    int n = 17;
    printf("Number of bits set from 1 to %d is %d\r\n",n,countSetBits(n));
    return 0;
}