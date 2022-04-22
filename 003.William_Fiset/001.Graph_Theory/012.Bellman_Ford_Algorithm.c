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
#define INF 0xFFFF

typedef struct Edge {
    int from;
    int to;
    double weight;
    struct Edge *prev;
    struct Edge *next;
} EDGE;

typedef struct graph{
    EDGE *nodes[MAXIMUM];
    bool visited[MAXIMUM];
    int  parent[MAXIMUM];
    double  distance[MAXIMUM];
    int  numOfNodes;
} GRAPH;

void refreshGraph(GRAPH * graph)
{
    memset(graph->visited, false, sizeof(graph->visited));
    memset(graph->parent, 0, sizeof(graph->parent));
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

void printEdgeList(int node, EDGE *head)
{
    printf("[%3d]:->\t", node);

    if (NULL == head){
        printf("  NIL");
    }

    while (NULL != head){
        printf("%3d->%3d [%3f]\t", head->from, head->to, head->weight);
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

void bellmanFord(GRAPH *g, int start) {

    for (int i = 0; i <= g->numOfNodes; i++){
        g->distance[i] = INFINITY;
    }

    g->distance[start] = 0;

    // For each vertex, apply relaxation for all the edges
    for (int i = 0; i < g->numOfNodes; i++){
        EDGE *edge = g->nodes[i];
        while (NULL != edge) {

            if (g->distance[edge->from] + edge->weight < g->distance[edge->to]) {
                g->distance[edge->to] = g->distance[edge->from] + edge->weight;
            }
            edge = edge->next;
        }
    }

#if 1
    // Run algorithm a second time to detect which nodes are part
    // of a negative cycle. A negative cycle has occurred if we
    // can find a better path beyond the optimal solution.
    for (int i = 0; i <= g->numOfNodes; i++){
        EDGE *edge = g->nodes[i];
        while (NULL != edge) {

            if (g->distance[edge->from] + edge->weight < g->distance[edge->to]) {
                g->distance[edge->to] = INFINITY;
            }
            edge = edge->next;
        }
    }
#endif

    // Return the array containing the shortest distance to every node
    return;
}

// Example usage of topological sort
int main(void) {

    // Graph setup
    int N = 9;
    GRAPH * graph = initGraph(N);

    addEdgeToGraph(graph, 0, 1,  4 , 4, true);
    addEdgeToGraph(graph, 0, 7,  8 , 8, true);
    addEdgeToGraph(graph, 1, 7, 11, 11, true);
    addEdgeToGraph(graph, 1, 2,  8 , 8, true);
    addEdgeToGraph(graph, 2, 8,  2 , 2, true);
    addEdgeToGraph(graph, 2, 5,  4 , 4, true);
    addEdgeToGraph(graph, 2, 3,  7,  7, true);
    addEdgeToGraph(graph, 3, 4,  9,  9, true);
    addEdgeToGraph(graph, 3, 5, 14, 14, true);
    addEdgeToGraph(graph, 4, 5, 10, 10, true);
    addEdgeToGraph(graph, 5, 6,  2,  2, true);
    addEdgeToGraph(graph, 6, 7,  1,  1, true);
    addEdgeToGraph(graph, 6, 8,  6,  6, true);
    addEdgeToGraph(graph, 7, 8,  7,  7, true);

    printGraph(graph);
    bellmanFord(graph, 0);

    printf("The Shortest path from 0 to all nodes : \r\n");
    for (int i = 0; i < graph->numOfNodes; i++){
        printf("%2d ", i);
    }
    printf("\r\n");
    for (int i = 0; i < graph->numOfNodes; i++){
        printf("%2f ", graph->distance[i]);
    }
    printf("\r\n");

    return 0;
}