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
    Page Faults in LRU

    In operating systems that use paging for memory management, page replacement algorithm
    is needed to decide which page needs to be replaced when the new page comes in.
    Whenever a new page is referred and is not present in memory,
    the page fault occurs and Operating System replaces one of the existing pages with a
    newly needed page.

    Given a sequence of pages in an array pages[] of length N and memory capacity C,
    find the number of page faults using Least Recently Used (LRU) Algorithm.

    Note:- Before solving this example revising the OS LRU cache mechanism is recommended.

    Example 1:

    Input: N = 9, C = 4
    pages = {5, 0, 1, 3, 2, 4, 1, 0, 5}
    Output: 8

    Explaination: memory allocated with 4 pages 5, 0, 1, 3: page fault = 4
    page number 2 is required, replaces LRU 5:
    page fault = 4+1 = 5
    page number 4 is required, replaces LRU 0:
    page fault = 5 + 1 = 6
    page number 1 is required which is already present:
    page fault = 6 + 0 = 6
    page number 0 is required which replaces LRU 3:
    page fault = 6 + 1 = 7
    page number 5 is required which replaces LRU 2:
    page fault = 7 + 1  = 8.

    Algorithm :
    step-1 : Initialize count as 0.
    step-2 : Create a array of size equal to memory capacity.
    step-3 : Traverse elements of pages[]
    step-4 : In each traversal:
            if(element is present in memory):
                remove the element and push the element at the end
            else:
                if(memory is full) remove the first element
                Increment count
                push the element at the end
*/
bool find_in_cache (int faults[], int c, int key){
    for (int i = 0; i< c; i++) {
        if (faults[i] == key) {
            printf("[%d] found in cache @ %d\r\n", key, i);
            return true;
        }
    }

    return false;
}

void printCache (int faults[], int c){
    for (int i = 0; i< c; i++) {
        printf("%d ", faults[i]);
    }
    printf("\r\n");
    return;
}

int pageFaults(int n, int c, int pages[])
{
    int total = 0;
    int faults[c];
    int pos = 0;
    memset(faults, -1, sizeof(faults));

    for (int i = 0; i<n; i++) {
        printCache(faults, c);
        printf("Received page : %d, next pos in cache: %d\r\n", pages[i], pos);
        bool inCache = find_in_cache(faults, c, pages[i]);
        if (false == inCache)  {
            total ++;
            if (pos < c-1) {
                faults[pos] = pages[i];
                pos ++;
            } else if (pos == c-1) {
                faults[pos] = pages[i];
                pos = 0;
            }
        }
    }

    return total;
}

/* Driver program to test pageFaults function*/
int main()
{
    int pages[] = { 5, 0, 1, 3, 2, 4, 1, 0, 5};
    int n = 9, c = 4;

    printf("Page Faults = %d\r\n",pageFaults(n, c, pages));
    return 0;
}