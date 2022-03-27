#include "stdio.h"
#include "math.h"
#include "limits.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "sys/types.h"
#include "assert.h"
#include "stdbool.h"


// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    We are given an array of n points in the plane, and the problem is to find out the
    closest pair of points in the array. This problem arises in a number of applications.
    For example, in air-traffic control, you may want to monitor planes that come
    too close together, since this may indicate a possible collision.
    Recall the following formula for distance between two points p and q.

    || pq || = sqrt{(px - qx)^2 + (py - qy)^2}

    We have discussed a divide and conquer solution for this problem.
    The time complexity of the implementation provided in the previous post is O(n (Logn)^2).
    In this post, we discuss implementation with time complexity as O(nLogn).

    Following is a recap of the algorithm discussed in the previous post.
    1) We sort all points according to x coordinates.
    2) Divide all points in two halves.
    3) Recursively find the smallest distances in both subarrays.
    4) Take the minimum of two smallest distances. Let the minimum be d.
    5) Create an array strip[] that stores all points which are at most d distance away
        from the middle line dividing the two sets.
    6) Find the smallest distance in strip[].
    7) Return the minimum of d and the smallest distance calculated in above step 6.

        The great thing about the above approach is, if the array strip[] is sorted according
        to y coordinate, then we can find the smallest distance in strip[] in O(n) time.
        In the implementation discussed in the previous post, strip[] was explicitly sorted in
        every recursive call that made the time complexity O(n (Logn)^2), assuming that
        the sorting step takes O(nLogn) time.
        In this post, we discuss an implementation where the time complexity is O(nLogn).
        The idea is to presort all points according to y coordinates.
        Let the sorted array be Py[]. When we make recursive calls,
        we need to divide points of Py[] also according to the vertical line.
        We can do that by simply processing every point and comparing
        its x coordinate with x coordinate of the middle line.
    Following is C++ implementation of O(nLogn) approach.
*/


// A structure to represent a Point in 2D plane
typedef struct Point_
{
    int x, y;
}Point;


/* Following two functions are needed for library function qsort().
   Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

// Needed to sort array of points according to X coordinate
int compareX(const void* a, const void* b)
{
    Point *p1 = (Point *)a,  *p2 = (Point *)b;
    return (p1->x != p2->x) ? (p1->x - p2->x) : (p1->y - p2->y);
}
// Needed to sort array of points according to Y coordinate
int compareY(const void* a, const void* b)
{
    Point *p1 = (Point *)a,   *p2 = (Point *)b;
    return (p1->y != p2->y) ? (p1->y - p2->y) : (p1->x - p2->x);
}

// A utility function to find the distance between two points
long double dist(Point p1, Point p2)
{
    long double res = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    return res;
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
long double bruteForce(Point P[], int n)
{
    long double min = INT_MAX;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);

    return min;
}


// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
long double stripClosest(Point strip[], int size, long double d)
{
    long double min = d;  // Initialize the minimum distance as d

    // Pick all points one by one and try the next points till the difference
    // between y coordinates is smaller than d.
    // This is a proven fact that this loop runs at most 6 times
    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i],strip[j]) < min)
                min = dist(strip[i], strip[j]);

    return min;
}

// A recursive function to find the smallest distance. The array Px contains
// all points sorted according to x coordinates and Py contains all points
// sorted according to y coordinates
long double closestUtil(Point Px[], Point Py[], int n)
{
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(Px, n);

    // Find the middle point
    int mid = n/2;
    Point midPoint = Px[mid];


    // Divide points in y sorted array around the vertical line.
    // Assumption: All x coordinates are distinct.
    Point Pyl[mid];   // y sorted points on left of vertical line
    Point Pyr[n-mid];  // y sorted points on right of vertical line
    int li = 0, ri = 0;  // indexes of left and right subarrays
    for (int i = 0; i < n; i++)
    {
      if ((Py[i].x < midPoint.x || (Py[i].x == midPoint.x && Py[i].y < midPoint.y)) && li<mid)
         Pyl[li++] = Py[i];
      else
         Pyr[ri++] = Py[i];
    }

    // Consider the vertical line passing through the middle point
    // calculate the smallest distance dl on left of middle point and
    // dr on right side
    long double dl = closestUtil(Px, Pyl, mid);
    long double dr = closestUtil(Px + mid, Pyr, n-mid);

    // Find the smaller of two distances
    long double d = min(dl, dr);

    // Build an array strip[] that contains points close (closer than d)
    // to the line passing through the middle point
    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(Py[i].x - midPoint.x) < d)
            strip[j] = Py[i], j++;

    // Find the closest points in strip.  Return the minimum of d and closest
    // distance is strip[]
    return stripClosest(strip, j, d);
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
long double closest(Point P[], int n)
{
    Point Px[n];
    Point Py[n];
    for (int i = 0; i < n; i++)
    {
        Px[i] = P[i];
        Py[i] = P[i];
    }

    qsort(Px, n, sizeof(Point), compareX);
    qsort(Py, n, sizeof(Point), compareY);

    // Use recursive function closestUtil() to find the smallest distance
    return closestUtil(Px, Py, n);
}

// Driver program to test above functions
int main()
{
    Point P[] = {{  2,   3},
                 { 12,  30},
                 { 40,  50},
                 {  5,   1},
                 { 12,  10},
                 {  3,   4}};
    int n = sizeof(P) / sizeof(P[0]);
    printf("The smallest distance is %Lf \r\n", closest(P, n));

    return 0;
}