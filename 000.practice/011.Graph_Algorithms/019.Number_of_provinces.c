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
    547. Number of Provinces

    There are n cities. Some of them are connected, while some are not.
    If city a is connected directly with city b, and city b is connected directly with city c,
    then city a is connected indirectly with city c.

    A province is a group of directly or indirectly connected cities
    and no other cities outside of the group.

    You are given an n x n matrix isConnected where isConnected[i][j] = 1
    if the ith city and the jth city are directly connected, and isConnected[i][j] = 0 otherwise.

    Return the total number of provinces.
*/

#define MAX 100
typedef struct graph {
    int noOfVertex;
    int noOfEdges;
    int M[MAX][MAX];
}GRAPH;

void dfs(int i, GRAPH *g, int visited[]) {
    visited[i] = true;
    for (int j = 0; j < g->noOfVertex; j++) {
        if (i != j && g->M[i][j] && !visited[j]) {
            dfs(j, g, visited);
        }
    }
}

int findCircleNumDfs(GRAPH *g) {
    if (0 == g->noOfVertex) {
        return 0;
    }

    int visited[g->noOfVertex];
    memset(visited, false, sizeof(visited));

    int groups = 0;
    for (int i = 0; i < g->noOfVertex; i++) {
        groups += !visited[i] ? dfs(i, g, visited), 1 : 0;
    }

    return groups;
}

int find(int x, int parents[]) {
    return parents[x] == x ? x : find(parents[x], parents);
}

int findCircleNumUnionFind(GRAPH *g) {
    if (0 == g->noOfVertex) {
        return 0;
    }

    int leads[g->noOfVertex];
    memset(leads, 0, sizeof(leads));
    for (int i = 0; i < g->noOfVertex; i++) { leads[i] = i; }   // initialize leads for every kid as themselves

    int groups = g->noOfVertex;
    for (int i = 0; i < g->noOfVertex; i++) {
        for (int j = i + 1; j < g->noOfVertex; j++) {   // avoid recalculate M[i][j], M[j][i]
            if (g->M[i][j]) {
                int lead1 = find(i, leads);
                int lead2 = find(j, leads);
                if (lead1 != lead2) {       // if 2 group belongs 2 different leads, merge 2 group to 1
                    leads[lead1] = lead2;
                    groups--;
                }
            }
        }
    }
    return groups;
}

void addUnDirectedEdge(GRAPH *g, int u,int v)
{
    g->M[u][v] = 1;
    g->M[v][u] = 1;
}

void printMatrix(GRAPH *g)
{
    printf("\r\nAdjcency Matrix : \r\n");
    for (int u = 0; u < g->noOfVertex; u++) {
        for (int v = 0; v < g->noOfVertex; v++) {
            printf("%3d", g->M[u][v]);
        }
        printf("\r\n");
    }
    printf("\r\n");
}

int main () {
    GRAPH g;
    memset(&g, 0, sizeof(g));
    int edges[][2] = {{  0,  1 },
                      {  2,  3 },
                      {  4,  5 }};

    g.noOfVertex = 6;
    g.noOfEdges = 3;

    printf("\r\nProcessing Edges : \r\n");
    for (int i =0; i < g.noOfEdges; i++){
        addUnDirectedEdge(&g, edges[i][0], edges[i][1]);
        printf ("%3d   -> %3d \r\n", edges[i][0], edges[i][1]);
    }

    printMatrix(&g);
    printf("\r\nNumber of Province by DFS: %d \r\n", findCircleNumDfs(&g));
    printf("Number of Province by Union Find: %d \r\n", findCircleNumUnionFind(&g));
}