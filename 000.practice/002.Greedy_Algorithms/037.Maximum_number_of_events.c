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
    Maximum number of events that can be attended

    There are N events in Geek's city. You are given two arrays start[] and end[] denoting starting
    and ending day of the events respectively. Event i starts at start[i] and ends at end[i].

    You can attend an event i at any day d between start[i] and end[i] (start[i] ≤ d ≤ end[i]).
    But you can attend only one event in a day.

    Find the maximum number of events you can attend.

    Example 1:
        Input:
            N = 3
            start[] = {1, 2, 1}
            end[] = {1, 2, 2}
        Output:
            2
        Explanation:
            You can attend a maximum of two events.
            You can attend 2 events by attending 1st event
            at Day 1 and 2nd event at Day 2.
    Example 2:
        Input:
            N = 3
            start[i] = {1, 2, 3}
            end[i] = {2, 3, 4}
        Output :
            3
        Explanation:
            You can attend all events by attending event 1
            at Day 1, event 2 at Day 2, and event 3 at Day 3.
*/
typedef struct event
{
        int start_day;
        int end_day;
        int event_number;
}event;

int cmp(const void *x, const void *y)
{
    event *e1 = (event *)x;
    event *e2 = (event *)y;

    return (*e1).end_day > (*e2).end_day;
}

int maxEvents(int start[], int end[], int n){

    event T[n];

    for (int i = 0; i < n; i++) {
        T[i].start_day  = start[i];
        T[i].end_day = end[i];
        T[i].event_number = i;
    }

    /* Sort the events according to their respective finish time. */
    qsort(T,n,sizeof(event),cmp);

    int events[n]; // This is used to store the event numbers that can be attended.
    int possible_events = 0; // To store the number of possible events
    int previous_event = 0;

    //Taking the first task
    events[possible_events++] = T[0].event_number;

    /* Select the task if it is compatable with the previously selected task*/
    for(int i=1; i < n; i++)
    {
        if(T[i].start_day >= T[previous_event].end_day)
        {
            events[possible_events++] = T[i].event_number;
            previous_event = i;
        }
    }

    printf("%d\n",possible_events);

    printf("Maximum possible events that can be attended are %d. They are \r\n",possible_events);
    for(int i=1; i < n; i++)
    {
        printf("%d\n",events[i]);
    }

    return possible_events;
}

int main ()
{
    int start[] = {1, 2, 3};
    int end[] = {2, 3, 4};
    int n = 3;

    printf("Maximum Number of events : %d", maxEvents(start, end, n));

    return 0;
}
