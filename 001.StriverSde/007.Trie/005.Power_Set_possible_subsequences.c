#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    Power Set: Print all the possible subsequences of the String
    Problem Statement: Given a string, find all the possible subsequences of the string.

    Examples:
        Example 1:
            Input: str = "abc"
            Output: a ab abc ac b bc c
            Explanation: Printing all the 7 subsequence for the string "abc".

        Example 2:
            Input: str = "aa"
            Output: a a aa
            Explanation: Printing all the 3 subsequences for the string "aa"
*/

void AllPossibleStrings(char s[])
{
	int n = strlen(s);
    char sub[100] = {'\0'};
    int pos = 0;

	for (int num = 0; num < (1 << n); num++) {
        memset(sub, '\0', sizeof(sub));
        pos = 0;
		for (int i = 0; i < n; i++) {
			//check if the ith bit is set or not
			if (num & (1 << i)) {
				sub[pos++] = s[i];
			}
		}

		if (pos > 0) {
			printf("%s \r\n", sub);
		}
	}

    return;
}

void solve(int i, char s[], char sub[], int pos) {
	if (i == strlen(s)) {
		for(int j = 0; j < pos; j++) {
            printf("%c", sub[j]);
        }
        printf("\r\n");

		return;
	}

	//picking
    sub[pos++] = s[i];
	solve(i + 1, s, sub, pos);
	//poping out while backtracking
	sub[pos--] = '\0';
	solve(i + 1, s, sub, pos);

    return;
}

void AllPossibleStringsRecursion(char s[])
{
    char sub[100] = {'\0'};
    int pos = 0;
    solve(0, s, sub, pos);
}

int main()
{
    char s[] ="abc";
	AllPossibleStrings(s);
    printf("----------------------------------\r\n");
    AllPossibleStringsRecursion(s);
}