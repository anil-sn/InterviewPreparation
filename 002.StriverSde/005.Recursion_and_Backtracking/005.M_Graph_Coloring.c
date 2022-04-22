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
    M – Coloring Problem
    Problem Statement: Given an undirected graph and a number m, determine if the graph can be colored with
    at most m colors such that no two adjacent vertices of the graph are colored with the same color.

    Examples:

        Example 1:
        Input:
            N = 4
            E = 5
            M = 3
        Edges[] = { (   0,  1   ),
                    (   1,  2   ),
                    (   2,  3   ),
                    (   3,  0   ),
                    (   0,  2   )}

        Output: 1
        Explanation: It is possible to colour the given graph using 3 colours.

    Example 2:

    Input:
        N = 3
        E = 3
        M = 2
    Edges[] = { (0, 1),
                (1, 2),
                (0, 2) }
    Output: 0
    Explanation: It is not possible to color.
*/

typedef struct graph_{
    int graph[100][100];
    int numOfNodes;
    int numOfColors;
    int color[100];
} GRAPH;

bool isSafe(int node, GRAPH *g, int col)
{
    for (int k = 0; k < g->numOfNodes; k++)  {
        if (k != node &&
            g->graph[k][node] == 1 &&
            g->color[k] == col) {

            return false;
        }
    }

    return true;
}

bool solve(int node, GRAPH *g)
{
    if (node == g->numOfNodes) {
        return true;
    }

    for (int i = 1; i <= g->numOfColors; i++) {
        if (isSafe(node, g, i)) {
            g->color[node] = i;
            if (solve(node + 1, g)) {
                 return true;
            }
            g->color[node] = 0;
        }
    }

    return false;
}

//Function to determine if graph can be coloured with at most M colours such
//that no two adjacent vertices of graph are coloured with same colour.
bool graphColoring(GRAPH *g)
{
    bool result = false;

    result = solve(0, g);

    return result;
}

// function to initialise the adjacency matrix
void init(GRAPH *g)
{
    memset(g, 0, sizeof(GRAPH));
    for(int i = 1; i <= g->numOfNodes; i++) {
        g->graph[i][i]=1;
    }
}

// Function to add edge between nodes
void addEdge(GRAPH *g, int a,int b)
{
    g->graph[a][b]=1;
    g->graph[b][a]=1;
}

int main()
{
    GRAPH g;
    init(&g);

    g.numOfNodes = 4;
    g.numOfColors = 3;

    addEdge(&g, 0,1);
    addEdge(&g, 1,2);
    addEdge(&g, 2,3);
    addEdge(&g, 3,0);
    addEdge(&g, 0,2);

    printf("Adjecency Matrix : \r\n");
    for (int i = 0; i < g.numOfNodes; i++) {
        for (int j = 0; j < g.numOfNodes; j++) {
            printf("%3d", g.graph[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    bool result = graphColoring(&g);
    if (result) {
        printf("The graph can be colored with M colors \r\n");
        for (int i = 0; i < g.numOfNodes; i++) {
            printf("\tNode : %d [ %d ]\r\n", i, g.color[i]);
        }
    } else {
        printf("The graph can not be colored with M colors \r\n");
    }

    return 0;
}