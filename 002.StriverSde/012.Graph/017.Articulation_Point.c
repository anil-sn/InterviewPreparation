#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define MIN(a_, b_) (a_ < b_ ? a_ : b_)

typedef struct graph_ {
    int noOfNodes;
    bool visited[MAX];
    int timeStamp[MAX];
    int low[MAX];
    int adj[MAX][MAX];
} GRAPH;

GRAPH * initGraph(int noOfNodes)
{
    GRAPH *g = calloc(1, sizeof(GRAPH));
    g->noOfNodes = noOfNodes;

    return g;
}

void dfsHelper(int node, int parent, int *timer, GRAPH *g)
{
    int child = 0;

    //printf("VIST %2d from Parent %2d\r\n", node, parent);
    g->visited[node] = true;
    g->timeStamp[node] =  *timer;
    g->low[node] = *timer;
    (*timer)++;

    for (int i = 0; i < g->noOfNodes; i++)
    {
        if ((false == g->adj[node][i]) ||
            (node == i) ||
            (parent == i)) {
            continue;
        }

        if (true == g->visited[i]) {
            g->low[node] = MIN(g->low[node], g->timeStamp[i]);
        } else {
            dfsHelper(i, node, timer, g);
            g->low[node] = MIN(g->low[node], g->low[i]);
            child ++;
            if(g->low[i] > g->timeStamp[node]) {
                printf("Cut Edge : %d - %d \r\n", node, i);
            }

            if((g->low[i] >= g->timeStamp[node]) && (parent != -1)) {
                printf("Articulation Node : %d \r\n", node);
            }
        }
    }

    if(parent == -1 && child > 1) {
        printf("Articulation Node : %d \r\n", node);
    }

    //printf("RETURN %2d \r\n", node);
}

void dfs(GRAPH *g)
{
    int timer = 0;
    for (int i = 0; i < g->noOfNodes; i++)
    {
        if (false == g->visited[i])
        {
            dfsHelper(i, -1, &timer, g);
        }
    }

    for (int i = 0; i < g->noOfNodes; i++)
    {
        printf("Node %2d [TimeStamp : %d, Component : %d] \r\n", i, g->timeStamp[i], g->low[i]);
    }
}

void addEdge(GRAPH *g, int u, int v)
{
    g->adj[u][v] = true;
    g->adj[v][u] = true;
}

int main (void)
{
    GRAPH *g = initGraph(13);

    addEdge(g, 0, 2 );
    addEdge(g, 0, 4 );
    addEdge(g, 1, 2 );
    addEdge(g, 1, 4 );
    addEdge(g, 3, 2 );
    addEdge(g, 3, 4 );
    addEdge(g, 4, 5 );
    addEdge(g, 5, 6 );
    addEdge(g, 6, 7 );
    addEdge(g, 6, 9 );
    addEdge(g, 7, 8 );
    addEdge(g, 8, 9 );
    addEdge(g, 8, 10 );
    addEdge(g, 10, 11 );
    addEdge(g, 10, 12 );
    addEdge(g, 11, 12 );

    dfs(g);

    return 0;
}