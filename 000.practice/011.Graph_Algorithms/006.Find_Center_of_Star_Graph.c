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
    https://leetcode.com/problems/find-center-of-star-graph/

    There is an undirected star graph consisting of n nodes labeled from 1 to n.
    A star graph is a graph where there is one center node and exactly n - 1 edges
    that connect the center node with every other node.

    You are given a 2D integer array edges where each edges[i] = [ui, vi] indicates that
    there is an edge between the nodes ui and vi. Return the center of the given star graph.

    Example 1:
            ┌─────┐
            │  4  │
            └──┬──┘
                │
            ┌──┴──┐
    ┌───────┤  2  ├─────┐
    │       └─────┘     │
    ┌─┴───┐            ┌──┴──┐
    │  1  │            │  3  │
    └─────┘            └─────┘

    Input: edges = [[1,2],[2,3],[4,2]]
    Output: 2
    Explanation: As shown in the figure above,
                node 2 is connected to every other node, so 2 is the center.


    Example 2:

    Input: edges = [[1,2],[5,1],[1,3],[1,4]]
    Output: 1
    Constraints:

    3 <= n <= 105
    edges.length == n - 1
    edges[i].length == 2
    1 <= ui, vi <= n
    ui != vi
    The given edges represent a valid star graph.
    Explanation
    Find the vertice which occurs the same as the number of edges.

    Time Complexity: O(N).
    Space Complexity: O(N).
*/

// function to initialise the adjacency matrix
void init(int adj[][4], int n)
{
    for(int i=1; i<=n;i++)
        adj[i][i]=1;
}

// Function to add edge between nodes
void addEdge(int adj[][4], int a,int b)
{
    adj[a][b]=1;
    adj[b][a]=1;
}

int findCenter (int adj[][4], int n)
{
    int vertex[n+1];
    memset(vertex, 0, sizeof(vertex));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (0 != adj[i][j]) {
                vertex[j]++;
            }
        }
    }

    int center = -1;
    for (int i = 0; i < n; i++ ) {
        if (n-1 == vertex[i]) {
            center = i;
            break;
        }
    }

    return center;
}


int main () {
    int n = 4;
    int adj[n][n];
    memset(adj, 0, sizeof(adj));

    init(adj, n);
    addEdge(adj, 1,2);
    addEdge(adj, 2,3);
    addEdge(adj, 4,2);

    int center = findCenter (adj, 4);
    printf ("Center of the graph is %d \r\n", center);
}