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
    A tree is an undirected graph in which any two vertices are connected by exactly one path.
    In other words, any connected graph without simple cycles is a tree.
    Given a tree of n nodes labelled from 0 to n - 1, and an array of n - 1 edges
    where edges[i] = [ai, bi] indicates that there is an undirected edge between
    the two nodes ai and bi in the tree, you can choose any node of the tree as the root.
    When you select a node x as the root, the result tree has height h.
    Among all possible rooted trees, those with minimum height (i.e. min(h))
    are called minimum height trees (MHTs).

    Return a list of all MHTs' root labels.
    You can return the answer in any order.

    The height of a rooted tree is the number of edges on the longest downward path between
    the root and a leaf.

    First let's review some statement for tree in graph theory:

    (1) A tree is an undirected graph in which any two vertices are
    connected by exactly one path.

    (2) Any connected graph who has n nodes with n-1 edges is a tree.

    (3) The degree of a vertex of a graph is the number of
    edges incident to the vertex.

    (4) A leaf is a vertex of degree 1. An internal vertex is a vertex of
    degree at least 2.

    (5) A path graph is a tree with two or more vertices that is not
    branched at all.

    (6) A tree is called a rooted tree if one vertex has been designated
    the root.

    (7) The height of a rooted tree is the number of edges on the longest
    downward path between root and a leaf.

    OK. Let's stop here and look at our problem.

    Our problem want us to find the minimum height trees and return their root labels.
    First we can think about a simple case -- a path graph.

    For a path graph of n nodes, find the minimum height trees is trivial.
    Just designate the middle point(s) as roots.

    Despite its triviality, let design a algorithm to find them.

    Suppose we don't know n, nor do we have random access of the nodes.
    We have to traversal. It is very easy to get the idea of two pointers.
    One from each end and move at the same speed.
    When they meet or they are one step away, (depends on the parity of n), we have the roots we want.

    This gives us a lot of useful ideas to crack our real problem.
    For a tree we can do some thing similar.
    We start from every end, by end we mean vertex of degree 1 (aka leaves).
    We let the pointers move the same speed. When two pointers meet, we keep only one of them,
    until the last two pointers meet or one step away we then find the roots.

    It is easy to see that the last two pointers are
    from the two ends of the longest path in the graph.

    The actual implementation is similar to the BFS topological sort.
    Remove the leaves, update the degrees of inner vertexes.
    Then remove the new leaves. Doing so level by level until there are 2 or 1 nodes left.
    What's left is our answer!

    The time complexity and space complexity are both O(n).

    Note that for a tree we always have V = n, E = n-1.
*/

#define MAX 100
typedef struct queue_ {
    int queue[MAX];
    int front;
    int rear;
    int itemCount;
} QUEUE;

void initializeQueue(QUEUE *q)
{
	q->rear = -1;
	q->front = 0;
    q->itemCount = 0;
}

int peek(QUEUE *q) {
   return (q->queue[q->front]);
}

bool isEmpty(QUEUE *q)
{
	return (q->itemCount == 0);
}

bool isFull(QUEUE *q)
{
	return (q->itemCount == MAX);
}

int size(QUEUE *q) {
   return (q->itemCount);
}

void enqueue(QUEUE *q, int data)
{
	if(isFull(q))
	{
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
	if(isEmpty(q))
	{
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


// This class represents a undirected graph using adjacency list
// representation
typedef struct Graph
{
    int V; // No. of vertices

    // Pointer to an array containing adjacency lists
    int adj[MAX][MAX];

    // Vector which stores degree of all vertices
    int degree[MAX];
}GRAPH;

// Initializes adjacency list and degree vector
void GraphInit(GRAPH *g, int V)
{
    g->V = V;
    int pos = 0;
    for (int i = 0; i < V; i++)
        g->degree[pos++] = 0;
}

// addEdge method adds vertex to adjacency list and increases degree by 1
void graphAddEdge(GRAPH *g, int v, int w)
{
    g->adj[v][w] = 1;    // Add w to v’s list
    g->adj[w][v] = 1;    // Add v to w’s list
    g->degree[v]++;         // increment degree of v by 1
    g->degree[w]++;         // increment degree of w by 1
}

//  Method to return roots which gives minimum height to tree
void findMinHeightTrees(GRAPH *g)
{
    QUEUE q;
    initializeQueue(&q);
    int V = g->V;

    //  first enqueue all leaf nodes in queue
    for (int i = 0; i < V; i++) {
        if (g->degree[i] == 1) {
            enqueue(&q, i);
        }
    }

    //  loop until total vertex remains less than 2
    while (V > 2)
    {
        int popEle = size(&q);
        V -= popEle;      // popEle number of vertices will be popped

        for (int i = 0; i < popEle; i++)
        {
            int t = dequeue(&q);

            // for each neighbour, decrease its degree and if it become leaf, insert into queue
            for (int i = 0; i < g->V; i++) {
                if (g->adj[t][i] == 1) {
                   g->degree[i]--;
                    if (g->degree[i] == 1) {
                        enqueue(&q, i);
                    }
                }
            }
        }
    }

    //  copying the result from queue to result vector
    int res[g->V];
    int pos = 0;
    memset(res, 0, sizeof(res));

    while (false == isEmpty(&q)) {
        res[pos++] = dequeue(&q);
    }

    for (int i = 0; i < pos; i++) {
        printf("%d ", res[i]);
    }

    printf("\r\n");
    return;
}

//  Driver code
int main()
{
    GRAPH g;
    memset(&g, 0, sizeof(g));
    GraphInit(&g, 6);
    graphAddEdge(&g, 0, 3);
    graphAddEdge(&g, 1, 3);
    graphAddEdge(&g, 2, 3);
    graphAddEdge(&g, 4, 3);
    graphAddEdge(&g, 5, 4);

    findMinHeightTrees(&g);
}