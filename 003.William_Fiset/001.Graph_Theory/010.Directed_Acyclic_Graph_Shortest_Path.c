#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAXIMUM 100
#define SIZEOF(array_) (sizeof((array_))/sizeof((array_)[0]))
#define MAX(a_,b_) (a_ > b_ ? a_ : b_)
#define MIN(a_,b_) (a_ < b_ ? a_ : b_)
#define INF 999

typedef struct Queue_ {
    int queue[MAXIMUM];
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
    queue->maxSize = MAXIMUM;
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

typedef struct Edge {
    int from;
    int to;
    int weight;
    struct Edge *prev;
    struct Edge *next;
} EDGE;

typedef struct graph{
    EDGE *nodes[MAXIMUM];
    bool visited[MAXIMUM];
    int  inDegree[MAXIMUM];
    int  ordering[MAXIMUM];
    int  distance[MAXIMUM];
    int  numOfNodes;
} GRAPH;

void refreshGraph(GRAPH * graph)
{
    memset(graph->visited, false, sizeof(graph->visited));
    memset(graph->inDegree, 0, sizeof(graph->inDegree));
    memset(graph->ordering, 0, sizeof(graph->ordering));
    memset(graph->distance, 0, sizeof(graph->distance));

    return;
}

GRAPH * initGraph(int numOfNodes)
{
    GRAPH *g = calloc(1, sizeof(GRAPH));
    g->numOfNodes = numOfNodes;
    refreshGraph(g);
    return g;
}

void updEdge(EDGE *e, int u, int v, int w)
{
    e->from = u;
    e->to = v;
    e->weight = w;
}

EDGE* newEdge(int u, int v, int weight)
{
    EDGE *new = calloc(1, sizeof(EDGE));

    updEdge(new, u, v, weight);
    new->prev = NULL;
    new->next = NULL;

    return new;
}

void addEdgeToGraph(GRAPH *g, int u, int v, int uWeight, int vWeight, bool isUnDirected)
{
    EDGE* uhead = g->nodes[u];
    EDGE* vhead = g->nodes[v];
    EDGE* uAdj = NULL;
    EDGE* vAdj = NULL;

    if (false == isUnDirected) {
        if (NULL == uhead) {
            g->nodes[u] = newEdge(u, v, uWeight);
        } else {
            uAdj = newEdge(u, v, uWeight);
            uAdj->prev = uhead;
            uAdj->next = uhead->next;
            uhead->next = uAdj;
        }
        return;
    } else {

        if (NULL == uhead) {
            g->nodes[u] = newEdge(u, v, uWeight);
        } else {
            uAdj = newEdge(u, v, uWeight);
            uAdj->prev = uhead;
            uAdj->next = uhead->next;
            uhead->next = uAdj;
        }

        if (NULL == vhead) {
            g->nodes[v] = newEdge(v, u, vWeight);
        } else {
            vAdj = newEdge(v, u, vWeight);
            vAdj->prev = vhead;
            vAdj->next = vhead->next;
            vhead->next = vAdj;
        }
        return;
    }

    uAdj = newEdge(u, v, uWeight);
    uAdj->prev = uhead;
    uAdj->next = uhead->next;
    uhead->next = uAdj;

    if (true == isUnDirected) {
        vAdj = newEdge(v, u, vWeight);
        vAdj->prev = vhead;
        vAdj->next = vhead->next;
        vhead->next = vAdj;
    }

    return;
}

int dfs(int i, int at, GRAPH * graph)
{
    graph->visited[at] = true;

    EDGE *head = graph->nodes[at];
    while (NULL != head){
        if ( false == graph->visited[head->to]) {
            i = dfs(i, head->to, graph);
        }
        head = head->next;
    }

    graph->ordering[i] = at;
    return i - 1;
}

void topologicalSort(GRAPH * graph)
{
    refreshGraph(graph);

    int i = graph->numOfNodes - 1;
    for (int at = 0; at < graph->numOfNodes; at++) {
        if ( false == graph->visited[at]){
            i = dfs(i, at, graph);
        }
    }

    return;
}

void kahnsTopologicalSort(GRAPH * graph)
{
    refreshGraph(graph);

    EDGE *head = NULL;
    for (int i = 0; i < graph->numOfNodes; i++){
        head = graph->nodes[i];
        while (NULL != head) {

            graph->inDegree[head->to]++;

            head = head->next;
        }
    }

    QUEUE *q = newQ();
    // Find all start nodes.
    for (int i = 0; i < graph->numOfNodes; i++){
        if (graph->inDegree[i] == 0) {
            addQ(q, i);
        }
    }

    int index = 0;

    while (false == isQEmpty(q)) {

        int at = delQ(q);
        graph->ordering[index++] = at;
        head = graph->nodes[at];
        while (NULL != head) {

            graph->inDegree[head->to]--;
            if (graph->inDegree[head->to] == 0) {
                addQ(q, head->to);
            }
            head = head->next;
        }
    }

    if (index != graph->numOfNodes) {
        printf("Graph is not acyclic! Detected a cycle.\r\n");
    }

    return;
}

void printEdgeList(int node, EDGE *head)
{
    printf("[%3d]:->\t", node);

    if (NULL == head){
        printf("  NIL");
    }

    while (NULL != head){
        printf("%3d->%3d [%3d]\t", head->from, head->to, head->weight);
        head = head->next;
    }

    printf("\r\n");
}

void printGraph(GRAPH *g)
{
    printf("Graph : \r\n");
    for (int i = 0; i < g->numOfNodes; i++){
        printEdgeList(i, g->nodes[i]);
    }
    printf("\r\n");
}

void dagShortestPath(GRAPH * graph, int start, int numNodes)
{
    refreshGraph(graph);
    topologicalSort(graph);

    graph->distance[start] = 0;
    for (int i = 0; i < numNodes; i++) {

        int nodeIndex = graph->ordering[i];

        EDGE *head = graph->nodes[nodeIndex];
        while (NULL != head) {

            int newDist = graph->distance[nodeIndex] + head->weight;
            if (graph->distance[head->to] == 0) {
                graph->distance[head->to] = newDist;
            } else {
                graph->distance[head->to] = MIN(graph->distance[head->to], newDist);
            }

            head = head->next;
        }
    }

    return;
}

// Example usage of topological sort
int main(void) {

    // Graph setup
    int N = 6;
    GRAPH * graph = initGraph(N);

    addEdgeToGraph(graph, 0, 1, 3 , 0, false);
    addEdgeToGraph(graph, 0, 2, 2 , 0, false);
    addEdgeToGraph(graph, 0, 5, 3 , 0, false);
    addEdgeToGraph(graph, 1, 3, 1 , 0, false);
    addEdgeToGraph(graph, 1, 2, 6 , 0, false);
    addEdgeToGraph(graph, 2, 3, 1 , 0, false);
    addEdgeToGraph(graph, 2, 4, 10, 0, false);
    addEdgeToGraph(graph, 3, 4, 5 , 0, false);
    addEdgeToGraph(graph, 5, 4, 7 , 0, false);

    printGraph(graph);
    topologicalSort(graph);

    printf("[DFS] Topological Ordering   : ");
    for (int i = 0; i < N; i++){
        printf("%3d", graph->ordering[i]);
    }
    printf("\r\n");

    kahnsTopologicalSort(graph);
    printf("[kahns] Topological Ordering : ");
    for (int i = 0; i < N; i++){
        printf("%3d", graph->ordering[i]);
    }
    printf("\r\n");

    dagShortestPath(graph, 0, N);
    printf("[DAG] All node shortest path : ");
    for (int i = 0; i < N; i++){
        printf("[%d]%1d  ", i, graph->distance[i]);
    }
    printf("\r\n");

    return 0;
}