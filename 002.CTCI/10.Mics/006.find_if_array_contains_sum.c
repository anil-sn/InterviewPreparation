
// Question is to find two elements in an array that
// their sum is equal to another given element
// for example if the array is [7, 3, 49, -8, 6, -11, 10]
// and if the number is 16
// then, there exists a sum that is equal to 16
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int cmpfunc(const void* a, const void* b) {
	return ((*((int*)a)) - (*((int*)b)));
}

int contains_sum(int arr[], size_t size, int elem)
{
	qsort(arr, size, sizeof(int), cmpfunc);

	// We will pick two anchors here One from left, and one from right
	int left = 0;
	int right = (int)size - 1;

	printf("sorted Array : ");
	for(int i = 0; i<size; i++){
		printf ("%d ", arr[i]);
	}
	printf("\r\n\r\n");

	while (left < right)
	{
		int sum = arr[left] + arr[right];
		printf ("Elem %d Sum %d, left %d, right %d, arry[left] %d, array[right %d\r\n",
				elem, sum, left, right, arr[left], arr[right]);
		if (sum < elem)
			left++;
		else if (sum > elem)
			right--;
		else
			return 1;
	}

	return 0;
}

int main()
{
	int arr[] = {7, 3, 49, -8, 6, -11, 10};
	int elem = 16;
	size_t size = sizeof(arr) / sizeof(arr[0]);

	int result = contains_sum(arr, size, elem);

	printf("\r\n  [7, 3, 49, -8, 6, -11, 10] %s\r\n", result ? "Contains 16" : "Does not contain 16");
}
