#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// https://takeuforward.org/dynamic-programming/striver-dp-series-dynamic-programming-problems/
// https://youtu.be/ekcwMsSIzVc

#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 9999
#define MAX_STRING_SIZE 40

bool isStringsChain(char *s1, char *s2)
{
    int n = strlen(s1);
    int m = strlen(s2);

    if (n != (m + 1)) {
        return false;
    }

    int first = 0;
    int second = 0;

    while (first < n) {
        if (s1[first] == s2[second]){
            first++;
            second++;
        } else {
            first++;
        }
    }

    if ((first == n) && (second == m)) {
        return true;
    }

    return false;
}

int lscIterative(int n, char arr[][MAX_STRING_SIZE])
{
    int dp[n];
    int hash[n];
    for (int i = 0; i < n; i++){
        dp[i] = 1;
        hash[i] = i;
    }

    int maxi = 1;
    int last = 0;
    for (int i = 0; i < n; i++){
        for (int prev = 0; prev < i; prev++) {
            if ((true == isStringsChain(arr[i], arr[prev])) &&
                (1 + dp[prev] > dp[i])) {

                dp[i] = 1 + dp[prev];
                hash[i] = prev;
            }
        }

        if (dp[i] > maxi){
            last = i;
            maxi = dp[i];
        }
    }

    char* lis[maxi];

    int pos = 0;
    lis[pos++] = arr[last];

    while (hash[last] != last){
        last = hash[last];
        lis[pos++] = arr[last];
    }

    printf("LIS : \r\n");
    for (int i = pos-1; i >= 0; i--){
        printf("%10s", lis[i]);
    }
    printf("\r\n");

    return maxi;
}

int cmp(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

int main ()
{
    char arr[][MAX_STRING_SIZE] = {"a",
                                   "bda",
                                   "bca",
                                   "b",
                                   "ba",
                                   "bdca"};
    int n = SIZEOF(arr);

    qsort(arr, n, sizeof(arr[0]), cmp);
    for (int i = 0; i < n; i++){
        printf("%10s", arr[i]);
    }
    printf("\r\n");

    int lsc = lscIterative(n, arr);
    printf("The length of longest string chain is : %d \r\n", lsc);
    return 0;
}