//RECURSIVE:-

// A C++ program to count number of partitions 
// of a set with n elements into k subsets 
#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


// Returns count of different partitions of n 
// elements in k subsets 
int countP(int n, int k) 
{ 
    // Base cases 
    if (n == 0 || k == 0 || k > n) 
        return 0; 
    if (k == 1 || k == n) 
        return 1; 

    // S(n+1, k) = k*S(n, k) + S(n, k-1) 
    return k*countP(n-1, k) + countP(n-1, k-1); 
} 

// Driver program 
int main() 
{ 
    int n = 3;
    int k = 2;
    printf("Number of ways a set n(%d) elements in k(%d) sets: %d \r\n", n, k, countP(n, k)); 
    return 0; 
} 