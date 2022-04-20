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
    261. Graph Valid Tree
    Given n nodes labeled from 0 to n - 1 and a list of undirected edges
    (each edge is a pair of nodes), write a function to check whether
    these edges make up a valid tree.

    For example:

    Given n = 5 and edges = [[0, 1], [0, 2], [0, 3], [1, 4]], return true.

    Given n = 5 and edges = [[0, 1], [1, 2], [2, 3], [1, 3], [1, 4]], return false.

    Note: you can assume that no duplicate edges will appear in edges.
    Since all edges are undirected, [0, 1] is the same as [1, 0] and
    thus will not appear together in edges.

    This is an undirected map, so in-degree methods is not very suitable. Using bfs,
    since in a valid tree traversal, each not should be only visited once,
    if there is a loop, you end up visit an already visited node, this is not a valid tree.
    using a queue, every time, if node is not visited, mark it as visited,
    then traverse its neighbors, if is not visited, enque it, if there is any cycle in the graph,
    your queue ends up containing multiple instance of certain numbers.

    https://tenderleo.gitbooks.io/leetcode-solutions-/content/GoogleMedium/261.html

    https://www.geeksforgeeks.org/union-find/
    Disjoint Set (Or Union-Find)  (Detect Cycle in an Undirected Graph)
    A disjoint-set data structure is a data structure that keeps track of a set of elements
    partitioned into a number of disjoint (non-overlapping) subsets.
    A union-find algorithm is an algorithm that performs two useful operations on such a
    data structure:

    Find: Determine which subset a particular element is in.
    This can be used for determining if two elements are in the same subset.
    Union: Join two subsets into a single subset.
    Here first we have to check if the two subsets belong to same set.
    If no, then we cannot perform union.
*/

#define MAX 6

// Data structure to store a graph edge
typedef struct Edge {
    int src, dest;
} EDGE;

typedef struct Graph {
    int adj[MAX][MAX];
}GRAPH;

// Find the root of the set in which element `k` belongs
int Find(int parent[], int k)
{
    // if `k` is root
    if (parent[k] == k) {
        return k;
    }

    // recur for the parent until we find the root
    return Find(parent, parent[k]);
}

// Perform Union of two subsets
void Union(int parent[], int a, int b)
{
    // find the root of the sets in which elements `x` and `y` belongs
    int x = Find(parent, a);
    int y = Find(parent, b);

    parent[y] = x;
}

void MakeSet(int parent[], int n)
{
    // create `n` disjoint sets (one for each vertex)
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

void printParent(int parent[], int n){
    printf("\r\n");
    for (int i = 0; i < n; i++) {
        printf("%4d", parent[i]);
    }
    printf("\r\n\r\n");
}

void printAdjecency(GRAPH *graph, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", graph->adj[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");
}

// Returns true if the graph has a cycle
bool findCycle(GRAPH *graph, int n)
{
    // initialize Main class
    int parent[n];
    int x, y;
    memset(parent, 0, sizeof(parent));

    // create a singleton set for each element of the universe
    MakeSet(parent, n);
    printParent(parent, n);

    printAdjecency(graph, n);

    // Recur for all adjacent vertices
    for (int i =0; i < n; i++){
        printf ("Processing Edge : %d -> %d \r\n", graph->adj[i][0], graph->adj[i][1]);
        // find the root of the sets to which elements `u` and `v` belongs
        x = Find(parent, graph->adj[i][0]);
        y = Find(parent, graph->adj[i][1]);

        printf("Parent of %d is %d & Parent of %d is %d  \r\n", graph->adj[i][0], x, graph->adj[i][1], y);

        // if both `u` and `v` have the same parent, the cycle is found
        if (x == y) {
            printParent(parent, n);
            return true;
        }
        else {
            Union(parent, x, y);
            printParent(parent, n);
        }
    }

    printParent(parent, n);
    return false;
}

void addUnDirectedEdge(GRAPH *g, int v,int w)
{
    g->adj[w][v] = 1;
    g->adj[v][w] = 1;
}

// Union–find algorithm for cycle detection in a graph
int main()
{
    // vector of graph edges
    int edges[][2] = {{  0,  1 },
                      {  1,  2 },
                      {  2,  3 },
                      {  2,  5 },
                      {  3,  4 },
                      {  0,  4 }
    };
    /*
                           ┌────┐
                  ┌────────┤ 0  ├────┐
                  │        └────┘    │
                  │                  │
                ┌─┴──┐               │
                │ 1  │             ┌─┴─┐
                └──┬─┘             │ 4 │
                   │               └─┬─┘
                   │                 │
                 ┌─┴──┐      ┌────┐  │
                 │  2 ├──────┤ 3  ├──┘
                 └─┬──┘      └────┘
                   │
                   │
                 ┌─┴──┐
                 │ 5  │
                 └────┘
    */

    GRAPH g;
    memset(&g, 0, sizeof(g));
    for (int i =0; i < MAX; i++){
        addUnDirectedEdge(&g, edges[i][0], edges[i][1]);
        printf ("%d -> %d \r\n", edges[i][0], edges[i][1]);
    }

    // total number of nodes in the graph (labelled from 0 to 11)
    int n = MAX;


    if (findCycle(&g, n)) {
        printf("Cycle Found \r\n\r\n");
    }
    else {
        printf("No Cycle Found \r\n\r\n");
    }

    // vector of graph edges
    int edges2[][2] = {{  0,  1 },
                      {  0,  3 },
                      {  1,  5 },
                      {  1,  2 },
                      {  3,  4 }
    };
    /*
                            ┌────┐
                   ┌────────┤ 0  │
                   │        └──┬─┘
                   │           │
                 ┌─┴──┐      ┌─┴──┐
            ┌────┤ 1  │      │  3 │
            │    └──┬─┘      └─┬──┘
            │       │          │
            │       │          │
        ┌───┴─┐   ┌─┴──┐     ┌─┴───┐
        │  5  │   │  2 │     │  4  │
        └─────┘   └────┘     └─────┘
    */
    memset(&g, 0, sizeof(g));
    for (int i =0; i < MAX-1; i++){
        addUnDirectedEdge(&g, edges2[i][0], edges2[i][1]);
        printf ("%d -> %d \r\n", edges2[i][0], edges2[i][1]);
    }

    // total number of nodes in the graph (labelled from 0 to 11)
    n = MAX;
    if (findCycle(&g, n)) {
        printf("Cycle Found \r\n");
    }
    else {
        printf("No Cycle Found \r\n");
    }
    return 0;
}