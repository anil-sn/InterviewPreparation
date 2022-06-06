#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// C++ program to cover a sequence of points  in minimum steps in a given order. 
#define max(a,b) (a>b?a:b)

// function to give minimum steps to  move from point p1 to p2 
int shortestPath(int *p1, int *p2) 
{ 
	// dx is total horizontal distance to be covered 
	int dx = abs(p1[0] - p2[0]); 

	// dy is total vertical  distance to be covered 
	int dy = abs(p1[1] - p2[1]); 

	// required answer is maximum of these two 
	return max(dx, dy); 
} 

// Function to return the minimum steps 
int coverPoints(int sequence[][2], int size) 
{ 
	int stepCount = 0; 

	// finding steps for each consecutive point in the sequence 
	for (int i = 0; i < size - 1; i++) { 
		stepCount += shortestPath((int *)sequence[i],  (int *)sequence[i + 1]); 
	} 

	return stepCount; 
} 

// Driver code 
int main() 
{ 
	// arr stores sequence of points  that are to be visited 
	int arr[][2] = { { 4, 6 }, { 1, 2 }, { 4, 5 }, { 10, 12 } }; 

	int n = sizeof(arr) / sizeof(arr[0]); 
	printf("The minimum number of steps to cover all the points in the infinite grid : %d \r\n", coverPoints(arr, n)); 
} 