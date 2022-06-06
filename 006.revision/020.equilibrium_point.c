#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int equilibrium(int arr[], int n) 
{ 
	int sum = 0; // initialize sum of whole array 
	int leftsum = 0; // initialize leftsum 

	/* Find sum of the whole array */
	for (int i = 0; i < n; ++i) 
		sum += arr[i]; 

	for (int i = 0; i < n; ++i) 
	{ 
		sum -= arr[i]; // sum is now right sum for index i 

		if (leftsum == sum) 
			return i; 

		leftsum += arr[i]; 
	} 

	/* If no equilibrium index found, then return 0 */
	return -1; 
} 

// Driver code 
int main() 
{ 
	int arr[] = { -7, 1, 5, 2, -4, 3, 0 }; 
	int arr_size = sizeof(arr) / sizeof(arr[0]); 
	printf("First equilibrium index is %d \r\n", equilibrium(arr, arr_size)); 
	return 0; 
} 