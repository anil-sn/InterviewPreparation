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
    There is a bi-directional graph with n vertices, where each vertex is labeled from 0 to n - 1 (inclusive).
    The edges in the graph are represented as a 2D integer array edges, where each edges[i] = [ui, vi]
    denotes a bi-directional edge between vertex ui and vertex vi. Every vertex pair is connected by at most one edge,
    and no vertex has an edge to itself.

    You want to determine if there is a valid path that exists from vertex source to vertex destination.

    Given edges and the integers n, source, and destination, return true if there is a valid path from source to destination,
    or false otherwise.

    Example 1:
    Input: n = 3, edges = [[0,1],[1,2],[2,0]], source = 0, destination = 2
    Output: true
    Explanation: There are two paths from vertex 0 to vertex 2:
    - 0 → 1 → 2
    - 0 → 2

    Example 2:
    Input: n = 6, edges = [[0,1],[0,2],[3,5],[5,4],[4,3]], source = 0, destination = 5
    Output: false
    Explanation: There is no path from vertex 0 to vertex 5.


    Constraints:
    1 <= n <= 2 * 105
    0 <= edges.length <= 2 * 105
    edges[i].length == 2

    0 <= ui, vi <= n - 1
    ui != vi
    0 <= source, destination <= n - 1

    There are no duplicate edges.
    There are no self edges.

    https://leetcode.com/problems/find-if-path-exists-in-graph/
*/

int adj[100][100];

#define MAX 1000
typedef struct stack_ {
    int stack[MAX];
    int top;
} STACK;

void initializeStack(STACK *s)
{
	s->top=-1;
}

bool isEmpty(STACK *s)
{
	if(s->top == -1)
		return true;
	else
		return false;
}

bool isFull(STACK *s)
{
	if(s->top == MAX-1)
		return true;
	else
		return false;
}

void push(STACK *s, int x)
{
	if(isFull(s))
	{
		printf("Stack Overflow\n");
		return;
	}
	s->top = s->top+1;
	s->stack[s->top] = x;
}

int pop(STACK *s)
{
	int x;
	if(isEmpty(s))
	{
		printf("Stack Underflow\n");
		return -1;
	}
	x = s->stack[s->top];
	s->top = s->top-1;
	return x;
}

// function to add an edge to graph
void addEdge(int v,int w)
{
    adj[w][v] = 1;
    adj[v][w] = 1;
}

// A BFS based function to check whether d is reachable from s.
bool isReachable(int s, int d, int n)
{
    // Base case
    if(s == d) {
        return true;
    }

    // Mark all the vertices as not visited
    bool visited[n];
    memset(visited, false, sizeof(visited));

    // Create a queue for BFS
    STACK stack;
    initializeStack(&stack);

    push(&stack, s);

    while(false == isEmpty(&stack))
    {
        // Pop a vertex from stack and print it
        s = pop(&stack);

        // Mark the current node as visited
        visited[s]= true;

        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it
        // visited  and enqueue it
        for (int i = 0; i<n; i++)
        {
            if (0 == adj[s][i]) {
                continue;
            }

            // If this adjacent node is the destination node,
            // then return true
            if(i == d) {
                return true;
            } else if(false == visited[i]) {
                push(&stack, i);
            }
        }
    }

 // If BFS is complete without visiting d
    return false;
}

// Driver program to test methods of graph class
int main()
{
    int n = 4;

    addEdge(0,1);
    addEdge(0,2);
    addEdge(1,2);
    addEdge(2,0);
    addEdge(2,3);
    addEdge(3,3);

    int u = 1, v = 3;

    bool result = isReachable(u,v,n);
    printf("Path %s b/w Source %d to Destination %d \r\n", result ? "Exists" : "Does not exists", u, v);

    return 0;
}