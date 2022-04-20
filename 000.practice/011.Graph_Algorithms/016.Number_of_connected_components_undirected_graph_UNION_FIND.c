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
    323. Number of Connected Components in an Undirected Graph

    Problem:
    Given n nodes labeled from 0 to n - 1 and a list of undirected edges (each edge is a pair of nodes),
    write a function to find the number of connected components in an undirected graph.

    Example 1:
            0          3
            |          |
            1 --- 2    4
    Given n = 5 and edges = [[0, 1], [1, 2], [3, 4]], return 2.

    Example 2:
            0           4
            |           |
            1 --- 2 --- 3
    Given n = 5 and edges = [[0, 1], [1, 2], [2, 3], [3, 4]], return 1.

    Note:
    You can assume that no duplicate edges will appear in edges.
    Since all edges are undirected, [0, 1] is the same as [1, 0] and
    thus will not appear together in edges.
*/

#define MAX 25

// This struct represents a undirected graph using adjacency list representation
typedef struct Graph
{
    int V; // No. of vertices

    // Pointer to an array containing adjacency lists
    int adj[MAX][MAX];
}GRAPH;

// Initializes adjacency list and degree vector
void GraphInit(GRAPH *g, int V)
{
    g->V = V;
}

// addEdge method adds vertex to adjacency list and increases degree by 1
void graphAddEdge(GRAPH *g, int v, int w)
{
    g->adj[v][w] = 1;    // Add w to v’s list
    //g->adj[w][v] = 1;    // Add v to w’s list
}

void printParent(int parent[], int n){
    printf("\r\n");
    for (int i = 0; i < n; i++) {
        printf("%4d", parent[i]);
    }
    printf("\r\n\r\n");
}

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


//  Method to return roots which gives minimum height to tree
void connectedComponents(GRAPH *g)
{
    int count = g->V;
    int x, y;

    // Mark all the vertices as not visited
    int parent[g->V];
    MakeSet(parent, g->V);

    for (int i = 0; i < g->V; i++) {
        for (int j = 0; j < g->V; j++) {
            if (0 != g->adj[i][j]) {
                x = Find(parent, i);
                y = Find(parent, j);
                if (x != y) {
                    count--;
                    Union(parent, x, y);
                }
                printf("processing Edge : [%d,%d]", i, j);
                printParent(parent, g->V);
            }
        }
    }

    printf("Total number of connected components : %d\r\n", count);
}

//  Driver code
int main()
{
    GRAPH g;
    memset(&g, 0, sizeof(g));
    GraphInit(&g, 5);
    graphAddEdge(&g, 1, 0);
    graphAddEdge(&g, 2, 3);
    graphAddEdge(&g, 3, 4);
    connectedComponents(&g);
}