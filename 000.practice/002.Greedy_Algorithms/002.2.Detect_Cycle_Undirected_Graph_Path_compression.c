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

    The above union() and find() are naive and the worst case time complexity is linear.
    The trees created to represent subsets can be skewed and can become like a linked list.
    Following is an example worst case scenario.

    Let there be 4 elements 0, 1, 2, 3

    Initially, all elements are single element subsets.
    0 1 2 3

    Do Union(0, 1)
       1   2   3
      /
     0

    Do Union(1, 2)
         2   3
        /
       1
     /
    0

    Do Union(2, 3)
             3
            /
          2
         /
       1
     /
    0

    The above operations can be optimized to O(Log n) in worst case.
    The idea is to always attach smaller depth tree under the root of the deeper tree.
    This technique is called union by rank.

    The term rank is preferred instead of height because if path compression technique
    (we have discussed it below) is used, then rank is not always equal to height. Also,
    size (in place of height) of trees can also be used as rank. Using size as rank also
    yields worst case time complexity as O(Logn).

    Let us see the above example with union by rank
    Initially, all elements are single element subsets.
    0 1 2 3

    Do Union(0, 1)
       1   2   3
      /
     0

    Do Union(1, 2)
       1    3
     /  \
    0    2

    Do Union(2, 3)
        1
     /  |  \
    0   2   3

    The second optimization to naive method is Path Compression.
    The idea is to flatten the tree when find() is called.
    When find() is called for an element x, root of the tree is returned.
    The find() operation traverses up from x to find root.
    The idea of path compression is to make the found root as parent of x
    so that we don’t have to traverse all intermediate nodes again.
    If x is root of a subtree, then path (to root) from all nodes under x also compresses.

    Let the subset {0, 1, .. 9} be represented as below and find() is called
    for element 3.

                 9
             /   |   \
            4    5    6
           /         /  \
          0         7    8
         /
        3
       / \
      1   2

    When find() is called for 3, we traverse up and find 9 as representative
    of this subset. With path compression, we also make 3 and 0 as the child of 9 so
    that when find() is called next time for 0, 1, 2 or 3, the path to root is reduced.

            --------9-------
          /   /    /  \      \
         0   4    5    6       3
                      /  \    /  \
                     7    8   1   2

    The two techniques complement each other. The time complexity of each operation becomes even
    smaller than O(Logn). In fact, amortized time complexity effectively becomes small constant.

    Following is union by rank and path compression based implementation to find a cycle in a graph.
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

typedef struct subset {
    int parent;
    int rank;
}SUBSET;

// Creates a graph with V vertices and E edges
GRAPH* createGraph(int V, int E) {
    GRAPH* graph = (GRAPH*) malloc( sizeof(GRAPH) );
    graph->V = V;
    graph->E = E;

    graph->edge = (EDGE*) malloc(graph->E * sizeof(EDGE) );

    return graph;
}

// A utility function to find set of an element i
// (uses path compression technique)
int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y (uses union by rank)
void Union(struct subset subsets[], int xroot, int yroot)
{

    // Attach smaller rank tree under root of high rank tree
    // (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and
    // increment its rank by one
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// The main function to check whether a given graph contains
// cycle or not
int isCycle( GRAPH* graph )
{
    int V = graph->V;
    int E = graph->E;

    // Allocate memory for creating V sets
    struct subset* subsets = (struct subset*)malloc(V * sizeof(struct subset));

    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Iterate through all edges of graph, find sets of both
    // vertices of every edge, if sets are same, then there
    // is cycle in graph.
    for (int e = 0; e < E; ++e) {
        int x = find(subsets, graph->edge[e].src);
        int y = find(subsets, graph->edge[e].dest);

        if (x == y)
            return 1;

        Union(subsets, x, y);
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