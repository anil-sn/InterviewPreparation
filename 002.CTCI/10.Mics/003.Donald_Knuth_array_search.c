
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * This code searches an element in an array
 * with two instructions executed in loop
 * Normally when we search for an element linearly,
 * we execute three instructions which are:
 * 1- compare the index with the size
 * 2- check if current element equals the element we are searching
 * 3- and increment the loop variable
 *
 * But in this implementation which is thought by Donald Knuth
 * we execute two instructions in a loop:
 * 1- compare current element with the element we are searching
 * 2- increment loop variable
 * when we are at end the condition becomes true and loop index
 * becomes the size
 *
 * We need to allocate one more element at the end to
 * implement this algorithm
 */

int main()
{
	int cnt = 10;
	int arr[10+1] = {1,2,3,4,5,6,7,8,9,10};

	int LookUpElement = 10;

	arr[cnt] = LookUpElement;

	int i = 0;
	for (; arr[i] != LookUpElement; i++);

	printf("%s %d\r\n", (i == cnt) ? "Array does not contain element."
									 : "Array contains element.", LookUpElement);
	return 0;
}
