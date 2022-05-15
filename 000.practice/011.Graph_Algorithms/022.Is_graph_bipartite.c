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
    785. Is Graph Bipartite?
    https://leetcode.com/problems/is-graph-bipartite/

    There is an undirected graph with n nodes, where each node is numbered between 0 and n - 1.
    You are given a 2D array graph, where graph[u] is an array of nodes that node u is adjacent to.
    More formally, for each v in graph[u], there is an undirected edge between node u and node v.
    The graph has the following properties:

    o There are no self-edges (graph[u] does not contain u).
    o There are no parallel edges (graph[u] does not contain duplicate values).
    o If v is in graph[u], then u is in graph[v] (the graph is undirected).
    o The graph may not be connected, meaning there may be two nodes u and v such that
      there is no path between them.

    A graph is bipartite if the nodes can be partitioned into two independent sets A and B
    such that every edge in the graph connects a node in set A and a node in set B.

    Return true if and only if it is bipartite.

    Input: graph = [[1,2,3],[0,2],[0,1,3],[0,2]]
    Output: false
    Explanation: There is no way to partition the nodes into two independent sets such that
                 every edge connects a node in one and a node in the other.

    Check whether a graph is bipartite
    A bipartite graph is a graph whose vertices can be divided into two disjoint sets
    so that every edge connects two vertices from different sets
    (i.e. there are no edges which connect vertices from the same set).
    These sets are usually called sides.

    You are given an undirected graph.
    Check whether it is bipartite, and if it is, output its sides.

    Algorithm
    There exists a theorem which claims that a graph is bipartite
    if and only if all its cycles have even length.
    However, in practice it's more convenient to use a different formulation of the definition:
    a graph is bipartite if and only if it is two-colorable.

    Let's use a series of breadth-first searches,
    starting from each vertex which hasn't been visited yet.
    In each search, assign the vertex from which we start to side 1.
    Each time we visit a yet unvisited neighbor of a vertex assigned to one side,
    we assign it to the other side.
    When we try to go to a neighbor of a vertex assigned to one side which has already been visited,
    we check that is has been assigned to the other side;
    if it has been assigned to the same side, we conclude that the graph is not bipartite.
    Once we've visited all vertices and successfully assigned them to sides,
    we know that the graph is bipartite and we have constructed its partitioning.
*/

#define MAX 100
typedef struct graph {
    int noOfVertex;
    int noOfEdges;
    int M[MAX][MAX];
    int colors[MAX];
}GRAPH;

#define BLUE 0
#define RED 1

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

void addUnDirectedEdge(GRAPH *g, int u,int v)
{
    g->M[u][v] = 1;
    g->M[v][u] = 1;
}

void initGraph(GRAPH *g, int edg[][2], int v, int e){
    memset(g, 0, sizeof(GRAPH));

    g->noOfVertex = v;
    g->noOfEdges = e;
    printf("\r\nProcessing Edges : \r\n");
    for (int i =0; i < g->noOfEdges; i++){
        addUnDirectedEdge(g, edg[i][0], edg[i][1]);
    }
}

/* DFS */
bool valid(GRAPH *g, int node, int color) {
    if (RED == g->colors[node]) {
        return g->colors[node] == color;
    }

    g->colors[node] = color;
    for (int u = 0; u < g->noOfVertex; u++) {
        if (g->M[node][u]) {
            if (false == valid(g, u, -color)){
                return false;
            }
        }
    }
    return true;
}

bool isBipartite_DFS(GRAPH *g) {
    for (int i = 0; i < g->noOfVertex; i++) {
        if ((BLUE != g->colors[i]) && (false == valid(g, i, RED))) {
            return false;
        }
    }
    return true;
}

/* BFS */
typedef struct queue_ {
    int itemCount;
    int front;
    int rear;
    int queue[MAX];
}QUEUE;

void initQueue(QUEUE *q){
    memset(q, 0, sizeof(QUEUE));
    q->front = 0;
    q->rear = -1;
}

int peek(QUEUE *q) {
   return (q->queue[q->front]);
}

bool isEmpty(QUEUE *q) {
	return (q->itemCount == 0);
}

bool isFull(QUEUE *q) {
	return (q->itemCount >= MAX-1);
}

int size(QUEUE *q) {
   return (q->itemCount);
}

void enqueue(QUEUE *q, int data) {
	if(isFull(q)) 	{
		printf("Queue Overflow\n");
		return;
	}

    if(q->rear == MAX-1) {
        q->rear = -1;
    }

    q->rear++;
    q->queue[q->rear] = data;
    q->itemCount++;
}

int dequeue(QUEUE *q)
{
	if(isEmpty(q)) {
		printf("Queue Empty\n");
		return 0;
	}

	int data = q->queue[q->front];
    q->front++;

    if(q->front == MAX) {
        q->front = 0;
    }

   q->itemCount--;
   return data;
}

bool isBipartite_BFS(GRAPH *g) {
    for (int i = 0; i < g->noOfVertex; i++) {
        if (BLUE == g->colors[i]) {
            QUEUE todo;
            initQueue(&todo);
            enqueue(&todo, i);
            g->colors[i] = RED;
            while (false == isEmpty(&todo)) {
                int node = dequeue(&todo);
                for (int neigh = 0; neigh < g->noOfVertex; neigh++) {
                    if (g->M[i][neigh]) {
                        if (BLUE == g->colors[neigh]) {
                            g->colors[neigh] = -(g->colors[node]);
                            enqueue(&todo, neigh);
                        } else if (g->colors[neigh] == g->colors[node]) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

int main () {

    GRAPH g;

    int edges1[][2] = {{ 0, 1 },
                      { 1, 2 },
                      { 2, 3 },
                      { 3, 4 },
                      { 4, 5 },
                      { 5, 0 }};

    initGraph(&g, edges1, 6, 6);
    if (isBipartite_DFS(&g)) {
        printf("Graph is Bipartite \r\n");
    } else {
        printf("Graph is not Bipartite \r\n");
    }

    initGraph(&g, edges1, 6, 6);
    if (isBipartite_BFS(&g)) {
        printf("Graph is Bipartite \r\n");
    } else {
        printf("Graph is not Bipartite \r\n");
    }

#if 0
    int edges2[][2] = {{ 0, 1 },
                      { 1, 2 },
                      { 2, 3 },
                      { 3, 4 },
                      { 4, 5 }};
    initGraph(&g, edges1, 6, 5);
    if (isBipartite(&g)) {
        printf("Graph is Bipartite \r\n");
    } else {
        printf("Graph is not Bipartite \r\n");
    }
#endif

}