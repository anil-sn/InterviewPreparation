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

// This function checks if the given input string contains all digits 0-9

// Unlike the naive implementation which is O(N^2)
// This implementation's time complexity is O(N) and uses O(1) auxilliary space

#define MAX 256

int alldigits(const char str[])
{
	int present[MAX] = { 0 };

	for (int i = 0; str[i] != '\0'; ++i)
		if (isdigit(str[i]))
			present[str[i]] = 1;

	for (int i = '0'; i <= '9'; i++)
		if (!present[i])
			return 0;

	return 1;
}

int main()
{
	const char name[] = "0123456rtyudfdfgn789ji";

	printf("In %s All 0-9 digits present %s \r\n", name, alldigits(name) ? "TRUE" : "FALSE");
	return 0;
}
