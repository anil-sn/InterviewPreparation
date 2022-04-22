#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX 100

typedef struct adj{
    int u;
    int v;
    int weight;
    struct adj *prev;
    struct adj *next;
}ADJ;

typedef struct graph{
    ADJ *nodes[MAX];
    bool visited[MAX];
    int numOfNodes;
} GRAPH;

GRAPH * initGraph(int numOfNodes)
{
    GRAPH *g = calloc(1, sizeof(GRAPH));
    g->numOfNodes = numOfNodes;
    return g;
}

ADJ* newAdj(int u, int v, int weight)
{
    ADJ *new = calloc(1, sizeof(ADJ));
    new->u = u;
    new->v = v;
    new->weight = weight;
    new->prev = NULL;
    new->next = NULL;

    return new;
}

void addAdjToGraph(GRAPH *g, int u, int v, int uWeight, int vWeight, bool isUnDirected)
{
    ADJ* uhead = g->nodes[u];
    ADJ* vhead = g->nodes[v];
    ADJ* uAdj = NULL;
    ADJ* vAdj = NULL;

    if (false == isUnDirected) {
        if (NULL == uhead) {
            g->nodes[u] = newAdj(u, v, uWeight);
        } else {
            uAdj = newAdj(u, v, uWeight);
            uAdj->prev = uhead;
            uAdj->next = uhead->next;
            uhead->next = uAdj;
        }
        return;
    } else {

        if (NULL == uhead) {
            g->nodes[u] = newAdj(u, v, uWeight);
        } else {
            uAdj = newAdj(u, v, uWeight);
            uAdj->prev = uhead;
            uAdj->next = uhead->next;
            uhead->next = uAdj;
        }

        if (NULL == vhead) {
            g->nodes[v] = newAdj(v, u, vWeight);
        } else {
            vAdj = newAdj(v, u, vWeight);
            vAdj->prev = vhead;
            vAdj->next = vhead->next;
            vhead->next = vAdj;
        }
        return;
    }

    uAdj = newAdj(u, v, uWeight);
    uAdj->prev = uhead;
    uAdj->next = uhead->next;
    uhead->next = uAdj;

    if (true == isUnDirected) {
        vAdj = newAdj(v, u, vWeight);
        vAdj->prev = vhead;
        vAdj->next = vhead->next;
        vhead->next = vAdj;
    }

    return;
}

void printAdjList(int node, ADJ *head)
{
    printf("[%3d]:->\t", node);

    if (NULL == head){
        printf("  NIL");
    }

    while (NULL != head){
        printf("%3d->%3d [%3d]\t", head->u, head->v, head->weight);
        head = head->next;
    }

    printf("\r\n");
}

void printGraph(GRAPH *g)
{
    printf("Graph : \r\n");
    for (int i = 0; i < g->numOfNodes; i++){
        printAdjList(i, g->nodes[i]);
    }
    printf("\r\n");
}

int main (void)
{
    int numOfNodes = 13;

    GRAPH *g = initGraph(numOfNodes);

    addAdjToGraph (g,  0,  9, 0, 0, true);
    addAdjToGraph (g,  1,  0, 0, 0, true);
    addAdjToGraph (g,  3,  2, 0, 0, true);
    addAdjToGraph (g,  3,  4, 0, 0, true);
    addAdjToGraph (g,  3,  5, 0, 0, true);
    addAdjToGraph (g,  5,  6, 0, 0, true);
    addAdjToGraph (g,  6,  7, 0, 0, true);
    addAdjToGraph (g,  7,  3, 0, 0, true);
    addAdjToGraph (g,  7, 10, 0, 0, true);
    addAdjToGraph (g,  8,  1, 0, 0, true);
    addAdjToGraph (g,  8,  7, 0, 0, true);
    addAdjToGraph (g,  9,  8, 0, 0, true);
    addAdjToGraph (g, 10, 11, 0, 0, true);
    addAdjToGraph (g, 11,  7, 0, 0, true);

    printGraph(g);

    return 0;
}
