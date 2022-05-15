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
    743. Network Delay Time
    https://leetcode.com/problems/network-delay-time/

    You are given a network of n nodes, labeled from 1 to n. You are also given times,
    a list of travel times as directed edges times[i] = (ui, vi, wi), where ui is
    the source node, vi is the target node, and wi is the time it takes for a signal
    to travel from source to target.
    We will send a signal from a given node k. Return the time it takes for all the
    n nodes to receive the signal. If it is impossible for all the n nodes to receive
    the signal, return -1.

    Input: times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
    Output: 2
    Example 2:

    Input: times = [[1,2,1]], n = 2, k = 1
    Output: 1
    Example 3:

    Input: times = [[1,2,1]], n = 2, k = 2
    Output: -1
*/

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

#define MAX 100
typedef struct graph {
    int noOfVertex;
    int noOfEdges;
    int startPoint;
    int M[MAX][MAX];
    // dist[i] will hold the shortest distance from src to i
    int dist[MAX];
    int parent[MAX];
    // sptSet[i] will be true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized
    bool sptSet[MAX];
}GRAPH;

void printMatrix(GRAPH *g)
{
    printf("\r\nAdjcency Matrix : \r\n");
    for (int u = 0; u < g->noOfVertex; u++) {
        for (int v = 0; v < g->noOfVertex; v++) {
            printf("%3d", g->M[u][v]);
        }
        printf("\r\n");
    }
    printf("\r\n");
}

// A utility function to find the vertex with minimum distance value,
// from the set of vertices not yet included in shortest path tree
int minDistance(GRAPH *g)
{
    // Initialize min value
    int min = INT_MAX;
    int min_index;

    for (int v = 0; v < g->noOfVertex; v++) {
        if (g->sptSet[v] == false && g->dist[v] <= min) {
            min = g->dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// A utility function to print the constructed distance array
void printSolution(GRAPH *g)
{
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < g->noOfVertex; i++)
        printf("%d \t\t %d\n", i, g->dist[i]);

    printf("\r\nVertex \t\t Its Parent\n");
    for (int i = 0; i < g->noOfVertex; i++)
        printf("%d \t\t %d\n", i, g->parent[i]);

    printf("\r\nTime taken by the signal to reach all the nodes is : ");
    int m = INT_MIN;
    for (int i = 1; i < g->noOfVertex; i++)
        m = max(m, g->dist[i]);
    printf("%d \r\n", m);
}


// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(GRAPH *g)
{
    // Distance of source vertex from itself is always 0
    g->dist[g->startPoint] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < (g->noOfVertex - 1); count++) {

        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(g);

        // Mark the picked vertex as processed
        g->sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < g->noOfVertex; v++)

            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if ((false == g->sptSet[v])
                && (0 != g->M[u][v])
                && (g->dist[u] != INT_MAX)
                && (g->dist[u] + g->M[u][v] < g->dist[v])) {

                g->dist[v] = g->dist[u] + g->M[u][v];
                g->parent[v] = u;
            }
    }

    // print the constructed distance array
    printSolution(g);
}

int networkDelayTime(GRAPH *g) {
    return 0;
}

void addDirectedWightEdge(GRAPH *g, int u,int v, int w)
{
    g->M[u][v] = w;
}

int main () {
    GRAPH g;
    memset(&g, 0, sizeof(g));
    int n = 6;
    int k = 1;
    int times[][3] = {{  1,  2, 1 },
                      {  1,  3, 2 },
                      {  2,  4, 4 },
                      {  2,  5, 5 },
                      {  3,  5, 3 }};

    g.noOfVertex = n;
    g.startPoint = k;
    g.noOfEdges = 5;

    printf("\r\nProcessing Edges : \r\n");
    for (int i =0; i < g.noOfEdges; i++){
        addDirectedWightEdge(&g, times[i][0], times[i][1], times[i][2]);
        printf ("%3d   -> %3d with weight : %3d \r\n", times[i][0], times[i][1], times[i][2]);
    }

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < g.noOfVertex; i++){
        g.dist[i] = INT_MAX;
        g.parent[i] = i;
        g.sptSet[i] = false;
    }

    printMatrix(&g);
    dijkstra(&g);
    networkDelayTime(&g);
}