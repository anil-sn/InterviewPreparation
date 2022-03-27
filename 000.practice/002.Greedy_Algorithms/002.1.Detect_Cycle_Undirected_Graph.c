#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

/*
    A disjoint-set data structure is a data structure that keeps track of a set of elements
    partitioned into a number of disjoint (non-overlapping) subsets. A union-find algorithm
    is an algorithm that performs two useful operations on such a data structure:

    Find: Determine which subset a particular element is in. This can be used for
    determining if two elements are in the same subset.

    Union: Join two subsets into a single subset. Here first we have to check if the two
    subsets belong to same set. If no, then we cannot perform union.
*/

// a structure to represent an edge in graph
typedef struct Edge {
    int src;
    int dest;
} EDGE;

// a structure to represent a graph
typedef struct Graph {

    // V-> Number of vertices, E-> Number of edges
    int V, E;

    // graph is represented as an array of edges
    EDGE* edge;
} GRAPH;

// Creates a graph with V vertices and E edges
GRAPH* createGraph(int V, int E) {
    GRAPH* graph = (GRAPH*) malloc( sizeof(GRAPH) );
    graph->V = V;
    graph->E = E;

    graph->edge = (EDGE*) malloc(graph->E * sizeof(EDGE) );

    return graph;
}

// A utility function to find the subset of an element i
int find(int parent[], int i)
{
    if (parent[i] == -1) {
        return i;
    }
    return find(parent, parent[i]);
}

// A utility function to do union of two subsets
void Union(int parent[], int x, int y) {
    parent[x] = y;
}

// The main function to check whether a given graph contains
// cycle or not
int isCycle( GRAPH* graph )
{
    // Allocate memory for creating V subsets
    int *parent = (int*) malloc( graph->V * sizeof(int) );

    // Initialize all subsets as single element sets
    memset(parent, -1, sizeof(int) * graph->V);

    // Iterate through all edges of graph, find subset of both
    // vertices of every edge, if both subsets are same, then
    // there is cycle in graph.
    for(int i = 0; i < graph->E; ++i)
    {
        int x = find(parent, graph->edge[i].src);
        int y = find(parent, graph->edge[i].dest);

        if (x == y)
            return 1;

        Union(parent, x, y);
    }

    return 0;
}

// Driver program to test above functions
int main()
{
    /* Let us create the following graph
        0
        | \
        |  \
        1---2
    */

    int V = 3, E = 3;
    GRAPH* graph = createGraph(V, E);

    // add edge 0-1
    graph->edge[0].src = 0;
    graph->edge[0].dest = 1;

    // add edge 1-2
    graph->edge[1].src = 1;
    graph->edge[1].dest = 2;

    // add edge 0-2
    graph->edge[2].src = 0;
    graph->edge[2].dest = 2;

    if (isCycle(graph))
        printf( "graph contains cycle\r\n" );
    else
        printf( "graph doesn't contain cycle\r\n" );

    return 0;
}