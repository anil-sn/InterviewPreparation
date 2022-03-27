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
    Minimum Swaps for Bracket Balancing
    You are given a string of 2N characters consisting of N ‘[‘ brackets and N ‘]’ brackets.
    A string is considered balanced if it can be represented in the for S2[S1] where S1 and S2
    are balanced strings. We can make an unbalanced string balanced by swapping adjacent characters.
    Calculate the minimum number of swaps necessary to make a string balanced.

    Examples:

        Input  : []][][
            Output : 2
        First swap: Position 3 and 4
            [][]][
        Second swap: Position 5 and 6
            [][][]

        Input  : [[][]]
            Output : 0
        The string is already balanced.
*/
int swapCount(char chars[], int n)
{

    // Stores total number of Left and
    // Right brackets encountered
    int countLeft = 0, countRight = 0;

    // swap stores the number of swaps
    // required imbalance maintains
    // the number of imbalance pair
    int swap = 0 , imbalance = 0;

    for(int i = 0; i < n; i++)
    {
        if (chars[i] == '[')
        {

            // Increment count of Left bracket
            countLeft++;

            if (imbalance > 0)
            {

                // swaps count is last swap count + total
                // number imbalanced brackets
                swap += imbalance;

                // imbalance decremented by 1 as it solved
                // only one imbalance of Left and Right
                imbalance--;
            }
        }
        else if(chars[i] == ']' )
        {

            // Increment count of Right bracket
            countRight++;

            // imbalance is reset to current difference
            // between Left and Right brackets
            imbalance = (countRight - countLeft);
        }
    }
    return swap;
}

// Driver code
int main()
{
    char s[] = "[]][][";
    printf("Swap Count : %d\r\n", swapCount(s, strlen(s)));

    char s1[] = "[[][]]";
    printf("Swap Count : %d\r\n", swapCount(s1, strlen(s1)));

    return 0;
}