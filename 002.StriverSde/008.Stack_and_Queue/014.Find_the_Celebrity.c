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
    Suppose you are at a party with n people labeled from 0 to n - 1 and among them, there may exist one celebrity.

    The definition of a celebrity is that all the other n - 1 people know the celebrity,
    but the celebrity does not know any of them.
    Now you want to find out who the celebrity is or verify that there is not one.

    The only thing you are allowed to do is ask questions like: "Hi, A. Do you know B?" to get information about whether A knows B.
    You need to find out the celebrity (or verify there is not one) by asking as few questions as possible (in the asymptotic sense).

    You are given a helper function bool knows(a, b) that tells you whether A knows B.
    Implement a function int findCelebrity(n).
    There will be exactly one celebrity if they are at the party.
    Return the celebrity's label if there is a celebrity at the party.
    If there is no celebrity, return -1.

    Example 1:
        Input: graph = [[1,1,0],[0,1,0],[1,1,1]]
        Output: 1
        Explanation: There are three persons labeled with 0, 1 and 2.
                     graph[i][j] = 1 means person i knows person j,
                     otherwise graph[i][j] = 0 means person i does not know person j.
                     The celebrity is the person labeled as 1 because both 0 and 2 know him but 1 does not know anybody.
    Example 2:
        Input: graph = [[1,0,1],[1,1,0],[0,1,1]]
        Output: -1
        Explanation: There is no celebrity.
*/

#define MAX 100
typedef struct stack_ {
    int stack[MAX];
    int top;
    int capacity;
} STACK;

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX;
    return stack;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

void push(STACK *s, int value) {
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    printf("Element %d is pushed to stack \r\n", value);
    s->stack[++s->top] = value;
    return;
}

int peek(STACK *s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

int pop(STACK *s) {
    if (isStackEmpty(s)) {
        return -1;
    }

    return (s->stack[s->top--]);
}

int size(STACK *s) {
    return (s->top);
}

// Max # of persons in the party
#define N 4

// Person with 2 is celebrity
bool MATRIX[N][N] = {{0, 0, 1, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 0},
                     {0, 0, 1, 0}};

bool knows(int a, int b)
{
    return MATRIX[a][b];
}

/*
    Brilliant algorithm:

    The first loop is to find the candidate as the author explains.

    In detail, suppose the candidate after the first for loop is person k,
    it means 0 to k-1 cannot be the celebrity, because they know a previous or current candidate.

    Also, since k knows no one between k+1 and n-1, k+1 to n-1 can not be the celebrity either.
    Therefore, k is the only possible celebrity, if there exists one.

    The remaining job is to check if k indeed does not know any other persons and all other persons know k.

    To this point, I actually realize that we can further shrink the calling of knows method.
    For example, we don't need to check if k knows k+1 to n-1 in the second loop, because the first loop has already done that.
    The optimized code is as follows:
*/

int findCelebrityOptimzed(int n)
{
    int candidate = 0;
    for(int i = 1; i < n; i++)
    {
        if(knows(candidate, i)) {
            candidate = i;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (i < candidate &&
            knows(candidate, i) ||
            !knows(i, candidate))
        {
             return -1;
        }

        if (i > candidate &&
            !knows(i, candidate))
        {
            return -1;
        }
    }

    return candidate;
}

// Returns -1 if celebrity is not present. If present, returns id (value from 0 to n-1).
int findCelebrity(int n)
{
    int indegree[n];
    int outdegree[n];

    memset(indegree, 0, sizeof(indegree));
    memset(outdegree, 0, sizeof(outdegree));

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {

            int x = knows(i,j);

            //set the degrees
            outdegree[i]+=x;
            indegree[j]+=x;
        }
    }

    //find a person with indegree n-1 and out degree 0
    for(int i=0; i<n; i++) {
        if(indegree[i] == n-1 && outdegree[i] == 0) {
            return i;
        }
    }

    return -1;
}

int findCelebrityStack(int n)
{
    STACK *s = initStack();

    int C; // Celebrity

    for (int i = 0; i < n; i++) {
        push(s, i);
    }

    // Extract top 2
    int A = pop(s);
    int B = pop(s);

    // Find a potential celebrity
    while (size(s) > 1) {

        if (knows(A, B)) {
            A = pop(s);
        } else {
            B = pop(s);
        }
    }

    // Potential candidate?
    C = pop(s);

    // Last candidate was not examined, it leads one excess comparison (optimize)
    if (knows(C, B))
        C = B;

    if (knows(C, A))
        C = A;

    // Check if C is actually a celebrity or not
    for (int i = 0; i < n; i++) {

        // If any person doesn't know 'a' or 'a' doesn't know any person, return -1
        if ( (i != C) && (knows(C, i) || !knows(i, C)) ) {
            return -1;
        }
    }

    return C;
}

int main()
{
    int n = 4;
    int id = findCelebrity(n);
    id == -1 ? printf ("No celebrity \r\n") : printf("Celebrity ID %d\r\n", id);

    id = findCelebrityOptimzed(n);
    id == -1 ? printf ("No celebrity \r\n") : printf("Celebrity ID %d\r\n", id);

    id = findCelebrityStack(n);
    id == -1 ? printf ("No celebrity \r\n") : printf("Celebrity ID %d\r\n", id);

    return 0;
}