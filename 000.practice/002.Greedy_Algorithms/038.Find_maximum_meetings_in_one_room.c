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
    Find maximum meetings in one room

    There is one meeting room in a firm.
    There are N meetings in the form of (S[i], F[i]) where S[i] is the start time of meeting i
    and F[i] is finish time of meeting i. The task is to find the maximum number of meetings that
    can be accommodated in the meeting room.
    Print all meeting numbers

    Examples:

    Input : s[] = {1, 3, 0, 5, 8, 5}, f[] = {2, 4, 6, 7, 9, 9}
    Output : 1 2 4 5
    First meeting [1, 2]
    Second meeting [3, 4]
    Fourth meeting [5, 7]
    Fifth meeting [8, 9]

    Input : s[] = {75250, 50074, 43659, 8931, 11273, 27545, 50879, 77924},
    f[] = {112960, 114515, 81825, 93424, 54316, 35533, 73383, 160252 }
    Output : 6 7 1

    Approach:
    Idea is to solve the problem using the greedy approach which is the same as Activity
    Selection Problem.

    Sort all pairs(Meetings) in increasing order of second number(Finish time) of each pair.
    Select first meeting of sorted pair as the first Meeting in the room and push it into
    result vector and set a variable time_limit(say) with the second value(Finishing time)
    of the first selected meeting.
    Iterate from the second pair to last pair of the array and if the value of the
    first element(Starting time of meeting) of the current pair is greater then previously
    selected pair finish time (time_limit) then select the current pair and
    update the result vector (push selected meeting number into vector) and
    variable time_limit. Print the Order of meeting from vector.

*/
typedef struct meet {
    int start;
    int finish;
    int id;
} MEET;

int cmp (const void *x, const void *y){
    MEET *a = (MEET *)x;
    MEET *b = (MEET *)y;

    return (a->finish > b->finish);
}

// Function for finding maximum meeting in one room
int maxMeetings(int s[], int f[], int n)
{
    MEET T[n];
    int i = 0;
    int j = 0;
    for (i = 0; i < n; i++) {
        T[i].start = s[i];
        T[i].finish = f[i];
        T[i].id = i;
    }

    // Sorting of meeting according to their finish time.
    qsort(T, n, sizeof(T[0]), cmp);
    for(i = 0; i < n; i++) {
        printf("%d: Start: %d, Finish: %d \r\n", T[i].id, T[i].start, T[i].finish);
    }

    int possible_meetings = 0;  // To store the number of possible events
    int previous_meeting = 0;
    int meetings[n];            // This is used to store the meeting numbers that can be attended.
    memset(meetings, -1, sizeof(meetings));
    for(j = 0; j < n; j++) {
        printf("%3d, ", meetings[j]);
    }
    printf ("\r\n");

    //Taking the first task
    meetings[possible_meetings++] = T[0].id;


    /* Select the task if it is compatable with the previously selected task*/
    for(i=1; i < n; i++)
    {
        for(int j = 0; j < n; j++) {
            printf("%3d, ", meetings[j]);
        }
        printf ("\r\n");

        if(T[i].start >= T[previous_meeting].finish)
        {
            meetings[possible_meetings++] = T[i].id;
            previous_meeting = i;
        }
    }

    printf("Maximum possible Meeting that can be scheduled are %d. They are : ",(possible_meetings - 1));
    for(i=1; i < n; i++)
    {
        if (-1 != meetings[i]) {
            printf("%d ",meetings[i]);
        }
    }
    printf ("\r\n\r\n");

    return (possible_meetings - 1);
}

// Driver code
int main()
{
    // Starting time
    int s[] = { 1, 3, 0, 5, 8, 5 };

    // Finish time
    int f[] = { 2, 4, 6, 7, 9, 9 };

    // Number of meetings.
    int n = sizeof(s) / sizeof(s[0]);

    // Function call
    maxMeetings(s, f, n);

    return 0;
}