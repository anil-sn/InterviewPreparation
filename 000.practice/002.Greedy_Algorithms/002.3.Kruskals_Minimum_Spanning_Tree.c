#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility functions
// maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

/*
    Given a connected and undirected graph, a spanning tree of that graph is a subgraph
    that is a tree and connects all the vertices together.
    A single graph can have many different spanning trees.
    A minimum spanning tree (MST) or minimum weight spanning tree for a weighted, connected,
    undirected graph is a spanning tree with a weight less than or equal to the weight of
    every other spanning tree. The weight of a spanning tree is the sum of weights given to each edge
    of the spanning tree.

    How many edges does a minimum spanning tree has?
    A minimum spanning tree has (V – 1) edges where V is the number of vertices in the given graph.

    What are the applications of the Minimum Spanning Tree?
    See this for applications of MST.

    Below are the steps for finding MST using Kruskal’s algorithm

    1. Sort all the edges in non-decreasing order of their weight.
    2. Pick the smallest edge. Check if it forms a cycle with the spanning tree
       formed so far. If cycle is not formed, include this edge. Else, discard it.
    3. Repeat step#2 until there are (V-1) edges in the spanning tree.
*/

// a structure to represent an edge in graph
typedef struct Edge {
	int src;
    int dest;
    int weight;
} EDGE;

// a structure to represent a graph
typedef struct Graph {

    // V-> Number of vertices, E-> Number of edges
    int V, E;

    // graph is represented as an array of edges
    EDGE* edge;
} GRAPH;

typedef struct subset {
    int parent;
    int rank;
}SUBSET;

// Creates a graph with V vertices and E edges
GRAPH* createGraph(int V, int E) {
    GRAPH* graph = (GRAPH*) malloc( sizeof(GRAPH) );
    graph->V = V;
    graph->E = E;

    graph->edge = (EDGE*) malloc(graph->E * sizeof(EDGE) );

    return graph;
}

// A utility function to find set of an element i
// (uses path compression technique)
int find(SUBSET subsets[], int i) {

    // find root and make root as parent of i (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y (uses union by rank)
void Union(SUBSET subsets[], int xroot, int yroot)
{

    // Attach smaller rank tree under root of high rank tree
    // (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and
    // increment its rank by one
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int cmp (const void* E1, const void* E2) {
	EDGE* a = (EDGE*)E1;
	EDGE* b = (EDGE*)E2;

	return a->weight > b->weight;
}

void kruskals(GRAPH* graph) {

	int V = graph->V;
	EDGE result[V];

	qsort(graph->edge, graph->E, sizeof(graph->edge[0]), cmp);

	printf("Sorted Graph: \n");
	int e;
	for (e = 0; e < graph->E; e++){
        printf("%d -- %d == %d \n", graph->edge[e].src, graph->edge[e].dest, graph->edge[e].weight);
    }

	SUBSET *subsets = (SUBSET*) malloc( V * sizeof(SUBSET));
	for (int i = 0; i < V; ++i) {
		subsets[i].parent = i;
		subsets[i].rank = 0;
	}

	int i = 0;
	e = 0;
	while (i < V - 1 && e < graph->E) {

		EDGE nextEdge = graph->edge[e++];

		int subsetA = find(subsets, nextEdge.src);
		int subsetB = find(subsets, nextEdge.dest);

		if (subsetA != subsetB) {
			result[i++] = nextEdge;
			Union(subsets, subsetA, subsetB);
		}
	}

	printf("Edges: \n");
	for (e = 0; e < i; ++e){
        printf("%d -- %d == %d\n", result[e].src, result[e].dest, result[e].weight);
    }
	return;
}


int main() {

	int V = 4; // Number of vertices in graph
	int E = 5; // Number of edges in graph
	GRAPH* graph = createGraph(V, E);

	graph->edge[0].src = 0;
	graph->edge[0].dest = 1;
	graph->edge[0].weight = 3;

	graph->edge[1].src = 0;
	graph->edge[1].dest = 2;
	graph->edge[1].weight = 4;

	graph->edge[2].src = 0;
	graph->edge[2].dest = 3;
	graph->edge[2].weight = 10;

	graph->edge[3].src = 1;
	graph->edge[3].dest = 2;
	graph->edge[3].weight = 11;


	graph->edge[4].src = 2;
	graph->edge[4].dest = 3;
	graph->edge[4].weight = 4;

	printf("Graph: \n");
	for (int e = 0; e < E; e++){
        printf("%d -- %d == %d \n", graph->edge[e].src, graph->edge[e].dest, graph->edge[e].weight);
    }

	kruskals(graph);

	return 0;
}