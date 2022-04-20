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
    277. Find the Celebrity

    Suppose you are at a party withnpeople (labeled from 0 to n - 1) and among them,
    there may exist one celebrity.
    The definition of a celebrity is that all the other n - 1 people know him/her
    but he/she does not know any of them.

    Now you want to find out who the celebrity is or verify that there is not one.
    The only thing you are allowed to do is to ask questions like: "Hi, A. Do you know B?"
    to get information of whether A knows B. You need to find out the celebrity (or verify there is not one)
    by asking as few questions as possible (in the asymptotic sense).

    You are given a helper functionbool knows(a, b)which tells you whether A knows B.
    Implement a functionint findCelebrity(n), your function should minimize the number of calls toknows.

    Note: There will be exactly one celebrity if he/she is in the party.
    Return the celebrity's label if there is a celebrity in the party.
    If there is no celebrity, return-1.

    Thoughs:

    1. O(3n): Explanation from scy_usc in here
        a. :The first loop will stop to an candidate i who doesn't know anyone from i+1 to n-1.
            For any previous x<i either know sb else or is not known by sb else.
        b. The second loop will check whether i knows anyone from 0 to i-1.
        c. The third loop is gonna check whether all party participants know x or not.
    2. O(2n): Explanation from czonzhu in here
        a. The first pass is to pick out the candidate. If candidate knows i, then switch candidate.
        b. The second pass is to check whether the candidate is real.

*/

// Max # of persons in the party
#define N 8

// Person with 2 is celebrity
bool MATRIX[N][N] = {{0, 0, 1, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 0},
                     {0, 0, 1, 0}};

bool knows(int a, int b)
{
    return MATRIX[a][b];
}

// Returns -1 if celebrity is not present.
// If present, returns id (value from 0 to n-1).
int findCelebrity(int n)
{
    //the graph needs not be constructed as the edges can be found by using knows function

    //degree array;
    int indegree[n];
    int outdegree[n];
    memset(indegree, 0, sizeof(indegree));
    memset(outdegree, 0, sizeof(outdegree));

    //query for all edges
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

// Driver code
int main()
{
    int n = 4;
    int id = findCelebrity(n);
    id == -1 ? printf ("No celebrity\r\n") : printf("Celebrity ID %d\r\n", id);
    return 0;
}