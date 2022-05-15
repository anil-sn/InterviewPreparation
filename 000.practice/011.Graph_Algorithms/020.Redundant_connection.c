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
    684. Redundant Connection
    https://leetcode.com/problems/redundant-connection/

    In this problem, a tree is an undirected graph that is connected and has no cycles.
    You are given a graph that started as a tree with n nodes labeled from 1 to n,
    with one additional edge added. The added edge has two different vertices chosen from 1 to n,
    and was not an edge that already existed. The graph is represented as an array edges of length n
    where edges[i] = [ai, bi] indicates that there is an edge between nodes ai and bi in the graph.

    Return an edge that can be removed so that the resulting graph is a tree of n nodes.
    If there are multiple answers, return the answer that occurs last in the input.

    ┌──────┐
    │ A(0) ├─────┐
    └──┬───┘     │
       │      ┌──┴──┐
       │      │ C(2)│
       │      └──┬──┘
     ┌─┴────┐    │
     │ B(1) ├────┘
     └──────┘
    Input: edges = [[0,1],[0,2],[1,2]]
    Output: [1,2]
*/


#define MAX 100
typedef struct graph {
    int noOfVertex;
    int noOfEdges;
    int M[MAX][MAX];
}GRAPH;

bool isFound = false;

void dfs(int i, GRAPH *g, int visited[]) {
    visited[i] = true;
    for (int j = 0; j < g->noOfVertex; j++) {
        if (i != j && g->M[i][j] && !visited[j]) {
            dfs(j, g, visited);
            if (false  == isFound) {
                printf ("DFS : The redundent edge is %d -> %d \r\n", i, j);
                isFound = true;
            }
        }

    }
}

int findRedundantConnectionDfs(GRAPH *g) {
    if (0 == g->noOfVertex) {
        return 0;
    }

    int visited[g->noOfVertex];
    memset(visited, false, sizeof(visited));

    int groups = 0;
    dfs(0, g, visited);

    return groups;
}

int find(int x, int parents[]) {
    return parents[x] == x ? x : find(parents[x], parents);
}

int findRedundantConnectionUnionFind(GRAPH *g) {
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
                } else {
                    printf ("The redundent edge is %d -> %d \r\n", i, j);
                    return groups;
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
                      {  0,  2 },
                      {  1,  2 }};

    g.noOfVertex = 3;
    g.noOfEdges = 3;

    printf("\r\nProcessing Edges : \r\n");
    for (int i =0; i < g.noOfEdges; i++){
        addUnDirectedEdge(&g, edges[i][0], edges[i][1]);
        printf ("%3d   -> %3d \r\n", edges[i][0], edges[i][1]);
    }

    printMatrix(&g);
    findRedundantConnectionDfs(&g);
    findRedundantConnectionUnionFind(&g);
}