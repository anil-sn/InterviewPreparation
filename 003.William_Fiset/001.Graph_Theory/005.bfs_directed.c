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

typedef struct Queue_ {
    int queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

QUEUE* newQ()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = MAX;
    return queue;
}

bool isQFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

void addQ(QUEUE *q, int val)
{
    if (isQFull(q)) {
        printf("Queue is full\r\nExiting...\r\n");
        return;
    }

    if (q->end == -1) {
        q->end = 0;
        q->start = 0;
    } else {
        q->end = (q->end + 1) % q->maxSize;
    }

    q->queue[q->end] = val;
    //printf("Element %d is enqued \r\n", val);
    q->currSize++;
}

int delQ(QUEUE *q)
{
    int val;
    if (isQEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
        return -1;
    }

    val = q->queue[q->start];
    if (q->currSize == 1) {
        q->end = -1;
        q->start = -1;
    } else {
        q->start = (q->start + 1) % q->maxSize;
    }

    q->currSize--;
    return val;
}

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
    int parent[MAX];
    int numOfNodes;
} GRAPH;

GRAPH * initGraph(int numOfNodes)
{
    GRAPH *g = calloc(1, sizeof(GRAPH));
    g->numOfNodes = numOfNodes;
    memset(g->parent, -1, sizeof(g->parent));
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

void bfsVisit(GRAPH *g, int visit)
{
    int node = 0;

    QUEUE *q = newQ();
    addQ(q, visit);

    while (false == isQEmpty(q)){
        node = delQ(q);
        if (true == g->visited[node]){
            continue;
        }

        g->visited[node] = true;
        printf("%3d, ", node);

        ADJ* uhead = g->nodes[node];
        while (NULL != uhead) {
            if (false == g->visited[uhead->v]){
                addQ(q, uhead->v);
                g->parent[uhead->v] = node;
            }
            uhead = uhead->next;
        }
    }

    return;
}

void bfs(GRAPH *g)
{
    for (int startPoint = 0; startPoint < g->numOfNodes; startPoint++) {
        if (false == g->visited[startPoint]) {
            printf("\r\nStart Point : %3d \r\n", startPoint);
            bfsVisit(g, startPoint);
        }
    }

    printf("\r\nBFS Travelsal Parent List \r\n");

    for (int node = 0; node < g->numOfNodes; node++) {
        printf("Parent of %d is %d \r\n", node, g->parent[node]);
    }

    printf("\r\nBFS Travelsal is Compeleted \r\n");

    return;
}

int main (void)
{
    int numOfNodes = 13;

    GRAPH *g = initGraph(numOfNodes);

    addAdjToGraph (g,  0,  9,  0, 0, false);
    addAdjToGraph (g,  1,  0,  0, 0, false);
    addAdjToGraph (g,  3,  2,  0, 0, false);
    addAdjToGraph (g,  3,  4,  0, 0, false);
    addAdjToGraph (g,  3,  5,  0, 0, false);
    addAdjToGraph (g,  5,  6,  0, 0, false);
    addAdjToGraph (g,  6,  7,  0, 0, false);
    addAdjToGraph (g,  7,  3,  0, 0, false);
    addAdjToGraph (g,  7, 10,  0, 0, false);
    addAdjToGraph (g,  8,  1,  0, 0, false);
    addAdjToGraph (g,  8,  7,  0, 0, false);
    addAdjToGraph (g,  9,  8,  0, 0, false);
    addAdjToGraph (g, 10, 11,  0, 0, false);
    addAdjToGraph (g, 11,  7,  0, 0, false);

    printGraph(g);
    bfs(g);

    return 0;
}
