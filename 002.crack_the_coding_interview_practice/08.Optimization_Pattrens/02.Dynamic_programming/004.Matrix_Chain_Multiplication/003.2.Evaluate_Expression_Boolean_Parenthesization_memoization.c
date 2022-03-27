#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    Given a boolean expression with following symbols.

    Symbols
        'T' ---> true
        'F' ---> false
    And following operators filled between symbols

    Operators
        &   ---> boolean AND
        |   ---> boolean OR
        ^   ---> boolean XOR
    Count the number of ways we can parenthesize the expression so that the value of expression evaluates to true.
    Let the input be in form of two arrays one contains the symbols (T and F) in order and the other contains operators (&, | and ^}

    Examples:

        Input: symbol[]    = {T, F, T}
                operator[]  = {^, &}

        Output: 2
        The given expression is "T ^ F & T", it evaluates true
        in two ways "((T ^ F) & T)" and "(T ^ (F & T))"

        Input: symbol[]    = {T, F, F}
            operator[]  = {^, |}

        Output: 2
        The given expression is "T ^ F | F", it evaluates true
        in two ways "( (T ^ F) | F )" and "( T ^ (F | F) )".

        Input: symbol[]    = {T, T, F, T}
            operator[]  = {|, &, ^}

        Output: 4
        The given expression is "T | T & F ^ T", it evaluates true
        in 4 ways ((T|T)&(F^T)), (T|(T&(F^T))), (((T|T)&F)^T)
        and (T|((T&F)^T)).
*/

int solve(char s[],int i,int j,int isTrue)
{
    int k = 0;
    int ans=0;

    // Base condition when expression is empty
    if(i>j) {
        return true;
    }

    // Base condition when only character exists
    if (i == j) {
        if (isTrue == true) {
            return s[i] == 'T';
        } else {
            return s[i] == 'F';
        }
    }

    for(k = i+1; k < j; k+=2)
    {
        int lt=solve(s, i, k-1,true);
        int lf=solve(s, i, k-1,false);

        int rt=solve(s, k+1, j,true);
        int rf=solve(s, k+1, j,false);

        if(s[k]=='&')
        {
            if(isTrue == true) {
                ans += lt*rt;
            } else {
                ans += (lf*rt) + (lt*rf) + (lf*rf);
            }
        } else if(s[k] == '|') {
            if(isTrue == true) {
                ans += lt*rt + lf*rt + rf*lt;
            } else {
                ans += lf*rf;
            }
        } else if(s[k] == '^') {
            if(isTrue == true) {
                ans += lt*rf + lf*rt;
            } else {
                ans += lt*rt + lf*rf;
            }
        }
    }

    return ans;
}
int main()
{
    char s[] = "T|T&F^T";
    int size = strlen(s);
    int i=0,j=size-1;
    int isTrue = 1;
    int isFalse = 0;

    printf("The number of ways for [T|T&F^T] evaluate as TRUE: %d\r\n",solve(s,i,j,isTrue));
    printf("The number of ways for [T|T&F^T] evaluate as FALSE: %d\r\n",solve(s,i,j,isFalse));
}