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
    int  degree[MAX];
    int  numOfNodes;
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

int countAdjList(ADJ *head)
{
    int count = 0;
    if (NULL == head){
        return count;
    }

    while (NULL != head){
        count++;
        head = head->next;
    }

    return count;
}

int findRootGraph(GRAPH *g)
{
    int leaves[MAX] = {0};
    int pos = 0;
    int newLeaves[MAX] = {0};
    int newPos = 0;

    printf("Finding center of the tree : \r\n");
    for (int i = 0; i < g->numOfNodes; i++){
        g->degree[i] = countAdjList(g->nodes[i]);
        printf("Initial Degree of node %d is %d \r\n", i, g->degree[i]);
    }
    printf("\r\n");

    for (int i = 0; i < g->numOfNodes; i++){
        if ((0 == g->degree[i]) || (1 == g->degree[i])) {
            g->degree[i] = 0;
            leaves[pos++] = i;
        }
    }

    printf("\r\n Leaves : ");
    for (int i = 0; i < pos; i++){
        printf("%3d ", leaves[i]);
    }
    printf("\r\n");

    printf("\r\n Updated Degree of node : ");
    for (int i = 0; i < g->numOfNodes; i++){
        printf("[%1d]->%1d    ", i, g->degree[i]);
    }
    printf("\r\n");

    int count = pos;

    while (count < g->numOfNodes) {

        newPos = 0;
        for (int i = 0; i < pos; i++) {

            int leaf = leaves[i];
            ADJ *head = g->nodes[leaf];
            while (NULL != head){
                g->degree[head->v] = g->degree[head->v] -1;
                if ((0 == g->degree[head->v]) || (1 == g->degree[head->v])){
                    newLeaves[newPos++] = head->v;
                }
                head = head->next;
            }
            g->degree[leaf] = 0;
        }
        count += newPos;
        for (int i = 0; i < newPos; i++){
            leaves[i] = newLeaves[i];
        }

        pos  = newPos;
    }

    for (int i = 0; i < pos; i++){
        printf("Center Node : %d \r\n", leaves[i]);
    }

    return leaves[pos - 1];
}

int main (void)
{
    int numOfNodes = 12;

    GRAPH *g = initGraph(numOfNodes);

    addAdjToGraph (g,  0,  9, 0, 0, true);
    addAdjToGraph (g,  3,  2, 0, 0, true);
    addAdjToGraph (g,  3,  4, 0, 0, true);
    addAdjToGraph (g,  3,  5, 0, 0, true);
    addAdjToGraph (g,  6,  7, 0, 0, true);
    addAdjToGraph (g,  7,  3, 0, 0, true);
    addAdjToGraph (g,  7, 10, 0, 0, true);
    addAdjToGraph (g,  8,  1, 0, 0, true);
    addAdjToGraph (g,  8,  7, 0, 0, true);
    addAdjToGraph (g,  9,  8, 0, 0, true);
    addAdjToGraph (g, 11,  7, 0, 0, true);

    printGraph(g);
    printf("Selecting center of the graph as %d \r\n", findRootGraph(g));
    return 0;
}
