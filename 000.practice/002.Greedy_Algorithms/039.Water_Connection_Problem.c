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
    Water Connection Problem

    There are n houses and p water pipes in Geek Colony. Every house has at most one pipe going
    into it and at most one pipe going out of it. Geek needs to install pairs of tanks and taps
    in the colony accoDestinationing to the following guidelines.

    1. Every house with one outgoing pipe but no incoming pipe gets a tank on its roof.
    2. Every house with only one incoming and no outgoing pipe gets a tap.

    The Geek council has proposed a network of pipes where connections are denoted by three input values:
    ai, bi, di denoting the pipe of diameter di from house ai to house bi.
    Find a more efficient way for the construction of this network of pipes.
    Minimize the diameter of pipes wherever possible.

    Note: The generated output will have the following format.
    The first line will contain t,
    denoting the total number of pairs of tanks and taps installed.
    The next t lines contain three integers each: house number of tank,
    house number of tap, and the minimum diameter of pipe between them.


    Example 1:

    Input:
        n = 9, p = 6
        a[] = {7,5,4,2,9,3}
        b[] = {4,9,6,8,7,1}
        d[] = {98,72,10,22,17,66}
    Output:
        3
        2 8 22
        3 1 66
        5 6 10
    Explanation:
        Connected components are
        3->1, 5->9->7->4->6 and 2->8.
        Therefore, our answer is 3
        followed by 2 8 22, 3 1 66, 5 6 10.


    Your Task:
        Your task is to complete the function solve() which takes an integer n(the number of houses),
        p(the number of pipes),
        the array a[] , b[] and d[]
        (where d[i] denoting the diameter of the ith pipe from the house a[i] to house b[i])
        as input parameter and returns the array of pairs of tanks and taps installed
        i.e ith element of the array contains three integers:
        house number of tank, house number of tap and the minimum diameter of pipe between them.

    Approach:
        Perform DFS from appropriate houses to find all different connected components.
        The number of different connected components is our answer t.
        The next t lines of the output are the beginning of the connected component,
        end of the connected component and the minimum diameter from the start to the end of
        the connected component in each line.
        Since, tanks can be installed only on the houses having outgoing pipe and no incoming pipe,
        therefore these are appropriate houses to start DFS from i.e.
        perform DFS from such unvisited houses.
*/

#define MAX 1100

// number of houses and number
// of pipes
int n, p;

// Array Destination stores the ending vertex of pipe
int Destination[MAX];

// Array wd stores the value of diameters between two pipes
int Diameter[MAX];

// Array Source stores the starting end of pipe
int Source[MAX];

// Vector a, b, c are used to store the final output
int Src[MAX];
int Dst[MAX];
int Dia[MAX];
int srcPos;
int Dstpos;
int Diapos;

int minDiameter;

int dfs(int house)
{
    printf("House %d ", house);
    if (Source[house] == 0){
        printf(" -> No further house");
        return house;
    }

    if (Diameter[house] < minDiameter) {
        printf("---%d---> ", Diameter[house]);
        minDiameter = Diameter[house];
    }
    return dfs(Source[house]);
}

// Function performing calculations.
void solve(int arr[][3])
{
    int i = 0;

    while (i < p) {

        int src = arr[i][0];
        int Dst = arr[i][1];
        int Dia = arr[i][2];

        Source[src] = Dst;
        Diameter[src] = Dia;
        Destination[Dst] = src;
        i++;
    }

    memset(Src, -1, sizeof(Src));
    memset(Dst, -1, sizeof(Dst));
    memset(Dia, -1, sizeof(Dia));
    srcPos = 0;
    Dstpos = 0;
    Diapos = 0;

    for (int house = 1; house <= n; house++)

        /* If a pipe has no ending vertex but has starting vertex
           i.e is an outgoing pipe then we need to start DFS with this vertex.*/
        if (Destination[house] == 0 && Source[house]) {
            minDiameter = INT_MAX;
            printf ("\r\n[ First House : %d  ", house);
            int w = dfs(house);
            printf (" ] Last House connected : %d Minimum Diameter : %d \r\n\r\n", w, minDiameter);

            // We put the details of component in final output array
            Src[srcPos++] = house;
            Dst[Dstpos++] = w;
            Dia[Diapos++] = minDiameter;
        }

    printf("%3d\r\n", srcPos);
    for (int j = 0; j < srcPos; j++) {
        printf("%3d %3d %3d \r\n", Src[j], Dst[j], Dia[j]);
    }
}

// driver function
int main()
{
    n = 9, p = 6;

    memset(Destination, 0, sizeof(Destination));
    memset(Source, 0, sizeof(Source));
    memset(Diameter, 0, sizeof(Diameter));

    int arr[][3] = { { 7, 4, 98 },
                     { 5, 9, 72 },
                     { 4, 6, 10 },
                     { 2, 8, 22 },
                     { 9, 7, 17 },
                     { 3, 1, 66 } };

    printf("Water pipe is connections: \r\n\r\n");
    for (int i = 0; i < p; i++){
        printf("House %d  -- %d --> House %d\r\n", arr[i][0], arr[i][2], arr[i][1]);
    }
    solve(arr);
    return 0;
}