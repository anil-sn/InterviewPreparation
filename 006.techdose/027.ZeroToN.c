#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int main()
{

    int n = 25;
    int count = 0;

    while(n)
    {
        if(n%2)
            n-=1;
        else
            n/=2;
        count+=1;
    }
    printf("Count : %d \r\n", count);
}