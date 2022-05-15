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
    There is a bi-directional graph with n vertices, where each vertex is labeled from 0 to n - 1 (inclusive).
    The edges in the graph are represented as a 2D integer array edges, where each edges[i] = [ui, vi]
    denotes a bi-directional edge between vertex ui and vertex vi. Every vertex pair is connected by at most one edge,
    and no vertex has an edge to itself.

    You want to determine if there is a valid path that exists from vertex source to vertex destination.

    Given edges and the integers n, source, and destination, return true if there is a valid path from source to destination,
    or false otherwise.

    Example 1:
    Input: n = 3, edges = [[0,1],[1,2],[2,0]], source = 0, destination = 2
    Output: true
    Explanation: There are two paths from vertex 0 to vertex 2:
    - 0 → 1 → 2
    - 0 → 2

    Example 2:
    Input: n = 6, edges = [[0,1],[0,2],[3,5],[5,4],[4,3]], source = 0, destination = 5
    Output: false
    Explanation: There is no path from vertex 0 to vertex 5.


    Constraints:
    1 <= n <= 2 * 105
    0 <= edges.length <= 2 * 105
    edges[i].length == 2

    0 <= ui, vi <= n - 1
    ui != vi
    0 <= source, destination <= n - 1

    There are no duplicate edges.
    There are no self edges.

    https://leetcode.com/problems/find-if-path-exists-in-graph/
*/

int adj[100][100];

// function to initialise the adjacency matrix
void init(int n)
{
    for(int i=1; i<=n;i++)
        adj[i][i]=1;
}

// Function to add edge between nodes
void addEdge(int a,int b)
{
    adj[a][b]=1;
    adj[b][a]=1;
}

// Function to compute the path
void computePaths(int n)
{
    // Use Floyd Warshall algorithm to detect if a path exists
    for(int k = 1; k <= n; k++)
    {
        // Try every vertex as an intermediate vertex to check if a path exists
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                adj[i][j] = adj[i][j] | (adj[i][k] && adj[k][j]);
    }
}

// Function to check if nodes are reachable
bool isReachable(int s, int d)
{
    if (adj[s][d] == 1)
        return true;
    else
        return false;
}

int main () {
    int n = 4;
    int source = 4;
    int destination = 3;

    init(n);
    addEdge(1,2);
    addEdge(2,3);
    addEdge(1,4);

    computePaths(n);

    bool result = isReachable(source,destination);
    printf("Path %s b/w Source %d to Destination %d \r\n", result ? "Exists" : "Does not exists", source, destination);
}