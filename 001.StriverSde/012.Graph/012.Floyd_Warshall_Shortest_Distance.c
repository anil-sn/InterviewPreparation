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
#ifdef INFINITY
#undef INFINITY
#define INFINITY 99999
#endif // INFINITY

typedef struct Graph {
    int V;
    int E;
    int matrix[MAX][MAX];
    int *distance;
    int *predecessor;
} GRAPH;

void initGraphArrays(GRAPH *g)
{
    for (int i = 0; i < (g->V); i++) {
        g->distance[i] = INFINITY;
        g->predecessor[i] = 0;
    }

    for (int i = 0; i < (g->V); i++) {
        for (int j = 0; j < (g->V); j++) {
            g->matrix[i][j] = INFINITY;
        }
    }

    return;
}

GRAPH * initGraph(int noVertex, int noEdges)
{
    GRAPH *g = malloc(sizeof(GRAPH));
    g->V = noVertex;
    g->E = noEdges;
    g->distance = (int *)malloc(noVertex * sizeof(int));
    g->predecessor = (int *)malloc(noVertex * sizeof(int));

    initGraphArrays(g);

    return g;
}

void updateEdge(GRAPH *g, int u, int v, int w)
{
    g->matrix[u][v] = w;
}

void printMatrix(GRAPH *g)
{
    for (int i = 0; i < (g->V); i++) {
        for (int j = 0; j < (g->V); j++) {

            if (g->matrix[i][j] == INFINITY)
                printf("%10s", "INFY");
            else
                printf("%10d", g->matrix[i][j]);
        }

        printf("\n");
    }
    printf("\n\n");
}

void floydWarshall(struct Graph *g, int source)
{
    int matrix[g->V][g->V];
    int i, j, k;

    printMatrix(g);

    for (i = 0; i < g->V; i++) {
        for (j = 0; j < g->V; j++) {
            matrix[i][j] = g->matrix[i][j];
        }
    }

    // Adding vertices individually
    for (k = 0; k < g->V; k++) {
        for (i = 0; i < g->V; i++) {
            for (j = 0; j < g->V; j++) {
                if (matrix[i][k] + matrix[k][j] < matrix[i][j])
                matrix[i][j] = matrix[i][k] + matrix[k][j];
            }
        }
    }

    for (i = 0; i < g->V; i++) {
        for (j = 0; j < g->V; j++) {
            g->matrix[i][j] = matrix[i][j];
        }
    }

    printMatrix(g);
}

int main(void)
{
    int source = 0;
    GRAPH *g =  initGraph(4, 6);

    updateEdge(g, 0, 1, 3);
    updateEdge(g, 0, 3, 5);
    updateEdge(g, 1, 0, 2);
    updateEdge(g, 1, 3, 4);
    updateEdge(g, 2, 1, 1);
    updateEdge(g, 3, 2, 2);

    floydWarshall(g, source);

    return 0;
}
