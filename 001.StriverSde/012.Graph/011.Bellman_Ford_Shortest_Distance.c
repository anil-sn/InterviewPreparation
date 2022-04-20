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

#ifdef INFINITY
#undef INFINITY
#define INFINITY 99999
#endif // INFINITY

typedef struct Edge {
    int u;
    int v;
    int w;
} EDGE;

typedef struct Graph {
    int V;
    int E;
    EDGE *edge;
    int *distance;
    int *predecessor;
} GRAPH;

void initGraphArrays(GRAPH *g)
{
    for (int i = 0; i < (g->V); i++) {
        g->distance[i] = INFINITY;
        g->predecessor[i] = 0;
    }

    return;
}

GRAPH * initGraph(int noVertex, int noEdges)
{
    GRAPH *g = malloc(sizeof(GRAPH));
    g->V = noVertex;
    g->E = noEdges;
    g->edge = (EDGE *)malloc(noEdges * sizeof(EDGE));
    g->distance = (int *)malloc(noVertex * sizeof(int));
    g->predecessor = (int *)malloc(noVertex * sizeof(int));

    initGraphArrays(g);

    return g;
}

void updateEdge(GRAPH *g, int u, int v, int w)
{
    static int pos = 0;

    g->edge[pos].u = u;
    g->edge[pos].v = v;
    g->edge[pos].w = w;

    pos++;
}

void display(int arr[], int size) {

    for (int i = 0; i < size; i++) {
        printf("[%d]%d ", i, arr[i]);
    }
    printf("\n");
}

void bellmanford(struct Graph *g, int source)
{
    int u = 0;
    int v = 0;
    int w = 0;

    //step 1: fill the distance array and predecessor array
    initGraphArrays(g);

    //mark the source vertex
    g->distance[source] = 0;

    printf("[ITOR : %d]Distance array   : ", 0);
    display(g->distance, (g->V));
    printf("[ITOR : %d]Predecessor array: ", 0);
    display(g->predecessor, (g->V));
    printf("\r\n");

    //step 2: relax edges |V| - 1 times
    for (int i = 0; i < ((g->V) - 1); i++) {
        for (int j = 0; j < (g->E); j++) {

            //get the edge data
            u = g->edge[j].u;
            v = g->edge[j].v;
            w = g->edge[j].w;

            if (g->distance[u] != INFINITY && g->distance[v] > g->distance[u] + w) {
                g->distance[v] = g->distance[u] + w;
                g->predecessor[v] = u;
            }
        }

        printf("[ITOR : %d]Distance array   : ", i+1);
        display(g->distance, (g->V));
        printf("[ITOR : %d]Predecessor array: ", i+1);
        display(g->predecessor, (g->V));
        printf("\r\n");
    }

    //step 3: detect negative cycle
    //if value changes then we have a negative cycle in the graph
    //and we cannot find the shortest distances
    for (int i = 0; i < (g->E); i++) {

        u = g->edge[i].u;
        v = g->edge[i].v;
        w = g->edge[i].w;

        if (g->distance[u] != INFINITY && g->distance[v] > g->distance[u] + w) {
            printf("Negative weight cycle detected!\n");
            return;
        }
    }

    //No negative weight cycle found!
    //print the distance and predecessor array
    printf("Distance array   : ");
    display(g->distance, (g->V));
    printf("Predecessor array: ");
    display(g->predecessor, (g->V));
}

int main(void)
{
    int source = 0;
    GRAPH *g =  initGraph(4, 5);

    updateEdge(g, 0, 1, 5);
    updateEdge(g, 0, 2, 4);
    updateEdge(g, 1, 3, 3);
    updateEdge(g, 2, 1, -6);
    //updateEdge(g, 2, 1, -3);
    //updateEdge(g, 2, 1, 6);
    updateEdge(g, 3, 2, 2);

    bellmanford(g, source);

    return 0;
}
