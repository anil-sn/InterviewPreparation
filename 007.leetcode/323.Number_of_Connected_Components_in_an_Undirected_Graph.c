#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


typedef struct _GNode {
    int data;
    struct _GNode *next;
} GNode;

typedef struct _Graph {
    int v;
    GNode **List;
} Graph;

Graph *
initGraph (int v)
{
    Graph *g = (Graph *)malloc (sizeof (Graph));
    g->v = v;
    g->List = (GNode **)malloc (sizeof (GNode *) * v);

    for (int i = 0; i < v; i ++) {
        g->List[i] = NULL;
    }

    return g;
}

void
addEdge (Graph *g, int src, int dest)
{
    GNode *node = (GNode *)malloc (sizeof (GNode));
    node->data = dest;
    node->next = g->List[src];
    g->List[src] = node;
    GNode *node1 = (GNode *)malloc (sizeof (GNode));
    node1->data = src;
    node1->next = g->List[dest];
    g->List[dest] = node1;
}

void
buildAdjList (Graph *g, int **Arr, int edgesRowSize)
{
    for (int i = 0; i < edgesRowSize; i ++) {
        int src = Arr[i][0];
        int dest = Arr[i][1];
        addEdge (g, src, dest);
    }

    return;
}
void
printGraph (Graph *g)
{
    for (int i = 0; i < g->v; i++) {
        printf ("[%d] ", i);
        GNode *node = g->List[i];

        while (node != NULL) {
            printf ("%d ->", node->data);
            node = node->next;
        }

        printf ("NULL\n");
    }
}
void
dfsUtil (Graph *g, int v, bool Arr[])
{
    Arr[v] = true;
    GNode *node = g->List[v];

    while (node != NULL) {
        if (!Arr[node->data]) {
            dfsUtil (g, node->data, Arr);
        }

        node = node->next;
    }
}
int
dfs (Graph *g)
{
    if (g == NULL) {
        return true;
    }

    bool Arr[g->v];
    memset (Arr, false, sizeof (Arr));
    int count = 0;

    for (int i = 0; i < g->v; i ++) {
        if (!Arr[i]) {
            dfsUtil (g, i, Arr);
            count ++;
        }
    }

    return count;
}

int
countComponents (int n, int **edges, int edgesRowSize, int edgesColSize)
{
    if (n == 0 || edges == NULL) {
        return 0;
    }

    Graph *g = initGraph (n);
    buildAdjList (g, edges, edgesRowSize);
    //printGraph(g);
    return dfs (g);
}