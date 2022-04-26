#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

bool canJump(int jump[], int n)
{
    int goalPost = jump[n-1];
    for (int i = n-1; i >=0; i--){
        if (i + jump[i] >= goalPost) {
            goalPost = i;
        }
    }

    if (0 == goalPost){
        return true;
    }
    return false;
}


int main (void)
{
    int jump1[] = {2,3,1,1,4};
    int jump2[] = {3,2,1,0,4};
    int n = 5;

    bool result  = false;
    result = canJump(jump1, n);
    printf("I can %s Jump \r\n", result ? "True" : "False");

    result = canJump(jump2, n);
    printf("I can %s Jump \r\n", result ? "True" : "False");

    return 0;
}