#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
What is Minimum Spanning Tree?
Given a connected and undirected graph, a spanning tree of that graph is a subgraph that
is a tree and connects all the vertices together. A single graph can have many different
spanning trees. A minimum spanning tree (MST) or minimum weight spanning tree for a weighted,
connected, undirected graph is a spanning tree with a weight less than or equal to the weight
of every other spanning tree. The weight of a spanning tree is the sum of weights given to
each edge of the spanning tree.

How many edges does a minimum spanning tree has?
A minimum spanning tree has (V – 1) edges where V is the number of vertices in the given graph.

Below are the steps for finding MST using Kruskal’s algorithm

1. Sort all the edges in non-decreasing order of their weight.
2. Pick the smallest edge. Check if it forms a cycle with the spanning tree formed so far.
    If cycle is not formed, include this edge. Else, discard it.
3. Repeat step#2 until there are (V-1) edges in the spanning tree.
*/

struct Edge {
	int from;
    int to;
    int weight;
};

struct Graph {

	int numberOfVertices;
    int numberOfEdges;
	struct Edge* edge;
};

struct Subset {
	int parent;
	int rank;
};

struct Graph* createGraph(int numberOfVertices, int numberOfEdges) {
	struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
	graph->numberOfVertices = numberOfVertices;
	graph->numberOfEdges = numberOfEdges;

	graph->edge = (struct Edge*) malloc( graph->numberOfEdges * sizeof( struct Edge ) );

	return graph;
}

int findSubset(struct Subset subsets[], int setIndex) {
	if (subsets[setIndex].parent != setIndex) {
		subsets[setIndex].parent = findSubset(subsets, subsets[setIndex].parent);
    }

	return subsets[setIndex].parent;
}

void findUnion(struct Subset subsets[], int setA, int setB) {
	int rootSetA = findSubset(subsets, setA);
	int rootSetB = findSubset(subsets, setB);

	if (subsets[rootSetA].rank < subsets[rootSetB].rank){
		subsets[rootSetA].parent = rootSetB;
	} else if  (subsets[rootSetA].rank > subsets[rootSetB].rank) {
		subsets[rootSetB].parent = rootSetA;
	} else {
		subsets[rootSetB].parent = rootSetA;
		subsets[rootSetA].rank++;
	}
}

int compareEdges(const void* comparedFirstEdge, const void* comparedSecondEdge) {
	struct Edge* firstEdge = (struct Edge*)comparedFirstEdge;
	struct Edge* secondEdge = (struct Edge*)comparedSecondEdge;
	return firstEdge->weight > secondEdge->weight;
}

void buildKruskal(struct Graph* graph) {
	int vertices = graph->numberOfVertices;
	struct Edge result[vertices];
	int indexForResults = 0;
	int indexforEdges = 0;

	qsort(graph->edge, graph->numberOfEdges, sizeof(graph->edge[0]), compareEdges);

	struct Subset *subsets = (struct Subset*) malloc( vertices * sizeof(struct Subset) );

	for (int i = 0; i < vertices; ++i) {
		subsets[i].parent = i;
		subsets[i].rank = 0;
	}

	while (indexForResults < vertices - 1 && indexforEdges < graph->numberOfEdges) {

		struct Edge nextEdge = graph->edge[indexforEdges++];

		int subsetA = findSubset(subsets, nextEdge.from);
		int subsetB = findSubset(subsets, nextEdge.to);

		if (subsetA != subsetB) {
			result[indexForResults++] = nextEdge;
			findUnion(subsets, subsetA, subsetB);
		}

	}

	printf("Edges: \n");
	for (indexforEdges = 0; indexforEdges < indexForResults; ++indexforEdges){
        printf("%d -- %d == %d\n", result[indexforEdges].from, result[indexforEdges].to, result[indexforEdges].weight);
    }
	return;
}


int main() {

	int numberOfVertices = 4; // Number of vertices in graph
	int numberOfEdges = 5; // Number of edges in graph
	struct Graph* graph = createGraph(numberOfVertices, numberOfEdges);

	graph->edge[0].from = 0;
	graph->edge[0].to = 1;
	graph->edge[0].weight = 3;

	graph->edge[1].from = 0;
	graph->edge[1].to = 2;
	graph->edge[1].weight = 4;

	graph->edge[2].from = 0;
	graph->edge[2].to = 3;
	graph->edge[2].weight = 10;

	graph->edge[3].from = 1;
	graph->edge[3].to = 2;
	graph->edge[3].weight = 11;


	graph->edge[4].from = 2;
	graph->edge[4].to = 3;
	graph->edge[4].weight = 4;

	buildKruskal(graph);

	return 0;
}