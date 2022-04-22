#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int fib(int N){

    if (N < 2) {
        return N;
    }

    int memory[N+1];
    memset(memory, 0, sizeof(memory));

    memory[0] = 0;
    memory[1] = 1;

    for (int i = 2; i < N+1; i++){
        memory[i] = memory[i-1] + memory[i-2];
    }

    return memory[N];
}

/*
    Problem Statement:
        Write a function to calculate nth Fibonacci number.

        Fibonacci numbers are a series of numbers in which each number is the sum of 2 preceding numbers.

        First Few Fibonacci are : 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ………

        Complexity:

            Time: O(N) - At max we need to solve n sub-problems as they are not repeating
            Space: O(N) - Needed to store the results of the sub-problem.
*/

// Driver program to test above function
int main()
{
    int N = 6;
    printf("Fibonacci value of %d numbers is %d\r\n", N, fib(N));
    return 0;
}