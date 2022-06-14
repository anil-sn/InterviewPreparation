#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

void reverse(int low, int high, char arr[])
{
    char temp;
    while (low < high) {
        temp = arr[low];
        arr[low] = arr[high];
        arr[high] = temp;
        low++;
        high--;
    }

    return;
}

void solve(int n, char input[n]) {

    reverse(0, n - 1, input);
    printf("%s \r\n", input);

    int s = 0;
    for (int i = 0; i < n; i++) {
        if (input[i] == ' ') {
            int e = i - 1;
            reverse(s, e, input);

            s = i + 1;
        }
    }

    reverse(s, n - 1, input);
    return;
}

int main(void)
{
    char input[] = {"My Name is Aadhvi"};
    int n = strlen(input);

    printf("%s \r\n", input);
    solve(n, input);
    printf("%s \r\n", input);
    
    return 0;
}