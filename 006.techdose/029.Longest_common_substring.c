#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

int main() {

    char s1[] = "ABCDGH";
    char s2[] = "ACDGHR";
    int X = strlen(s1);
    int Y = strlen(s2);
    
    int dp[X+1][Y+1];
    int lcs = 0;
    
    for(int i=0;i<=X;++i)
        dp[i][0]=0;
    for(int i=0;i<=Y;++i)
        dp[0][i]=0;
    
    //Fill dp[][]
    for(int i=1;i<=X;++i)
    {
        for(int j=1;j<=Y;++j)
        {
            if(s1[i-1]==s2[j-1])
            {
                dp[i][j] = 1 + dp[i-1][j-1];
                if(lcs < dp[i][j])
                {
                    lcs = dp[i][j];
                }
            }
            else
                dp[i][j]=0;
        }
    }

    printf("lcs len: %d \r\n", lcs);

	return 0;
}