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
    N meetings in one room
    Problem Statement: There is one meeting room in a firm. You are given two arrays,
    start and end each of size N.For an index ‘i’, start[i] denotes the starting time of
    the ith meeting while end[i]  will denote the ending time of the ith meeting.
    Find the maximum number of meetings that can be accommodated if only one meeting can
    happen in the room at a  particular time. Print the order in which these meetings will be performed.

    Example:

    Input:  N = 6,  start[] = {1,3,0,5,8,5},
                     end[] =  {2,4,5,7,9,9}
    Output: 1 2 4 5
    Explanation: See the figure for a better understanding.
*/

typedef struct meeting_
{
    int slno;
    int start;
    int end;
}MEETING;

int cmp (const void *a, const void *b)
{
    MEETING *x = (MEETING *)a;
    MEETING *y = (MEETING *)b;
    return (x->end - y->end);
}

void printListOfMeeting(MEETING schedule[], int slots)
{
    // Sort the meeting as ending time because only ending time can
    // gurantee if we can start next meeting after that time.
    qsort(schedule, slots, sizeof(MEETING), cmp);
    for (int i = 0; i < slots; i++){
        printf("Meeting %d \r\n\t\tstarts : %4d \t\t Ends : %4d \r\n", schedule[i].slno,
                                                                       schedule[i].start ,
                                                                       schedule[i].end );
    }

    printf("\r\n\r\n Accepted Meeting are :\r\n");
    printf("Meeting %d \r\n\t\tstarts : %4d \t\t Ends : %4d \r\n", schedule[0].slno,
                                                                   schedule[0].start,
                                                                   schedule[0].end);
    for (int i = 1; i < slots; i++) {
        if (schedule[i-1].end <= schedule[i].start) {
            printf("Meeting %d \r\n\t\tstarts : %4d \t\t Ends : %4d \r\n", schedule[i].slno,
                                                                           schedule[i].start,
                                                                           schedule[i].end);
        }
    }

    return;
}


int main ()
{
    MEETING schedule[] =    {{ 1, 1, 2},
                             { 2, 3, 4},
                             { 3, 0, 5},
                             { 4, 5, 7},
                             { 5, 8, 9},
                             { 6, 5, 9}};
    int slots = sizeof(schedule)/sizeof(schedule[0]);

    printListOfMeeting(schedule, slots);

    return;
}