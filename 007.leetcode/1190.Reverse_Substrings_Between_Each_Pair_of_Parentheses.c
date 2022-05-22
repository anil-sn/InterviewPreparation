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
Explanation
In the first pass,
use a stack to find all paired parentheses,
I assume you can do this.

Now just imgine that all parentheses are wormholes.
As you can see in the diagram,
the paired parentheses are connected to each other.

Green->(<-Red <-Green)Red->

First it follows the left green arrrow,
go into the left wormhole and get out from the right wormhole.
Then it iterates the whole content between parentheses.
Finally it follows the right arrow,

go into the left wormhole,
get out from the right wormhole and finish the whole trip.

So in the second pass of our solution,
it traverses through the paired parentheses
and generate the result string res.

i is the index of current position.
d is the direction of traversing.
*/

#define MAX 10000
typedef struct stack_ {
    int stack[MAX];
    int top;
} STACK;

void
initializeStack (STACK *s)
{
    s->top = -1;
}

int
top (STACK *s)
{
    return (s->stack[s->top]);
}

bool
isStackEmpty (STACK *s)
{
    if (s->top == -1) {
        return true;
    } else {
        return false;
    }
}

bool
isStackFull (STACK *s)
{
    if (s->top == MAX - 1) {
        return true;
    } else {
        return false;
    }
}

void
push (STACK *s, int x)
{
    if (isStackFull (s)) {
        printf ("Stack Overflow\n");
        return;
    }

    s->top = s->top + 1;
    s->stack[s->top] = x;
}

int
pop (STACK *s)
{
    int x;

    if (isStackEmpty (s)) {
        printf ("Stack Underflow\n");
        return -1;
    }

    x = s->stack[s->top];
    s->top = s->top - 1;
    return x;
}
void
reverse (char *str, int l, int h)
{
    int n = h - l;
    char temp;

    // Swap character starting from two corners
    for (int i = 0; i < n / 2; i++) {
        temp = str[i + l];
        str[i + l] = str[n - i - 1 + l];
        str[n - i - 1 + l] = temp;
    }
}

char *
reverseParentheses (char *s)
{
    STACK st;
    initializeStack (&st);
    int len = strlen (s);
    char *res = malloc (len + 1);
    res[len] = '\0';

    for (int i = 0; i < len; i ++) {
        if (s[i] == '(') {
            push (&st, i);
        } else if (s[i] == ')') {
            int top = pop (&st);
            reverse (s, top + 1, i);
        }
    }

    int pos = 0;

    for (int i = 0; i < len; i ++) {
        char it = s[i];

        if (it != '(' && it != ')') {
            res[pos++] = it;
        }
    }

    res[pos] = '\0';
    return res;
}