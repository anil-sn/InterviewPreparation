# Problem: Merge Overlapping Intervals

```
“You are given an array of intervals where each interval is represented as a pair of integers
[start, end]. Merge all overlapping intervals and return the non-overlapping intervals that
cover all the intervals in the input.”

Example:
Input: intervals = [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]
Explanation: Since intervals [1,3] and [2,6] overlap, they are merged into [1,6].

Step-by-Step Approach

Sort the intervals based on the starting value. This ensures that overlapping intervals are adjacent.
Iterate through the sorted intervals and merge them if they overlap.
Store the merged intervals in a result array.

Explanation of Key Concepts

Sorting is crucial to ensure that we only need a single pass to merge overlapping intervals.
Merging Logic: If the current interval starts before or at the end of the last merged interval, we merge them by extending the end.
Space Complexity: O(n) for the merged array.
Time Complexity: O(n log n) due to sorting, and O(n) for the merge pass.

function mergeIntervals(intervals):
  if intervals is empty:
    return empty array
    
  sort intervals by start time
  initialize result array with first interval
  
  for each current interval in intervals (starting from second):
    last interval = last interval in result array
    
    if current interval's start <= last interval's end:
      merge by updating last interval's end to max(last interval's end, current interval's end)
    else:
      add current interval to result
      
  return result
  ```

```c
#include <stdio.h>
#include <stdlib.h>

// Structure to represent an interval
typedef struct {
    int start;
    int end;
} Interval;

// Comparator function for qsort to sort intervals by start time
int compareIntervals(const void *a, const void *b) {
    Interval *intA = (Interval *)a;
    Interval *intB = (Interval *)b;
    return intA->start - intB->start;
}

// Function to merge overlapping intervals
int mergeIntervals(Interval *intervals, int intervalsSize, Interval *merged) {
    if (intervalsSize == 0) return 0;

    // Step 1: Sort the intervals by start time
    qsort(intervals, intervalsSize, sizeof(Interval), compareIntervals);

    int index = 0; // Index for merged intervals

    // Step 2: Iterate and merge
    merged[0] = intervals[0];
    for (int i = 1; i < intervalsSize; i++) {
        if (intervals[i].start <= merged[index].end) {
            // Overlapping intervals, merge them
            if (intervals[i].end > merged[index].end) {
                merged[index].end = intervals[i].end;
            }
        } else {
            // No overlap, move to next interval
            index++;
            merged[index] = intervals[i];
        }
    }

    return index + 1; // Number of merged intervals
}

// Helper function to print intervals
void printIntervals(Interval *intervals, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("[%d,%d]", intervals[i].start, intervals[i].end);
        if (i != size - 1) printf(",");
    }
    printf("]\n");
}

int main() {
    Interval intervals[] = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
    int n = sizeof(intervals) / sizeof(intervals[0]);

    Interval merged[n]; // Output array (at most n intervals)

    int mergedSize = mergeIntervals(intervals, n, merged);

    printf("Merged Intervals: ");
    printIntervals(merged, mergedSize);

    return 0;
}
```