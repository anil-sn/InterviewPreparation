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

int binsearch(const int x, const int arr[], const int n)
{
	int low = 0;
	int high = n - 1;
	int middle;

	while (low <= high)
	{
		middle = (low + high) / 2;

		int elem = arr[middle];

		if (x < elem)
			high = middle - 1;
		else if (x > elem)
			low = middle + 1;
		else
			return middle;
	}

	return -1;
}

int main()
{
	int arr[] = { 1,2,3,4,5,6,7,8,9,14,34,546,4566,24545 };
	int x = 6;

	int pos = binsearch(x, arr, sizeof arr/ sizeof arr[0]);

	printf("Element found in Pos : %d\r\n", pos);
}
