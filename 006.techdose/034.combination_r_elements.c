#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/* HANDLING DUPLICATES:
    // Since the elements are sorted, 
       all occurrences of an element must be together
        while (arr[i] == arr[i+1])
             i++; 
*/


/* arr[] ---> Input Array 
data[] ---> Temporary array to 
store current combination 
start & end ---> Staring and 
Ending indexes in arr[] 
index ---> Current index in data[] 
r ---> Size of a combination to be printed */
void combinationUtil(int arr[], int data[], 
					int start, int end, 
					int index, int r) 
{ 
	// Current combination is ready 
	// to be printed, print it 
	if (index == r) 
	{ 
		for (int j = 0; j < r; j++) 
			printf("%d ",data[j]);
		printf("\r\n");
		return; 
	} 

	// replace index with all possible 
	// elements. The condition "end-i+1 >= r-index" 
	// makes sure that including one element 
	// at index will make a combination with 
	// remaining elements at remaining positions 
	for (int i = start; i <= end && end - i + 1 >= r - index; i++) 
	{ 
		data[index] = arr[i]; 
		combinationUtil(arr, data, i+1, end, index+1, r); 
	} 
} 

// The main function that prints 
// all combinations of size r 
// in arr[] of size n. This function 
// mainly uses combinationUtil() 
void printCombination(int arr[], int n, int r) 
{ 
	// A temporary array to store 
	// all combination one by one 
	int data[r]; 

	// Print all combination using 
	// temprary array 'data[]' 
	combinationUtil(arr, data, 0, n-1, 0, r); 
} 
// Driver code 
int main() 
{ 
	int arr[] = {1, 2, 3, 4, 5}; 
	int r = 3; 
	int n = sizeof(arr)/sizeof(arr[0]); 
	printCombination(arr, n, r); 
}