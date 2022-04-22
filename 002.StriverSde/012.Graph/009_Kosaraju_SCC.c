#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


#define MAX     100

typedef struct graph_{
    int graph[MAX][MAX];
    int transGraph[MAX][MAX];

    bool visited[MAX];
    bool dfsVisitPath[MAX];

    unsigned int distance[MAX];

    int numOfNodes;
} GRAPH;

typedef struct stack_ {
    int stack[MAX];
    int top;
    int capacity;
} STACK;

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX;
    return stack;
}


bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

int stackPeek(STACK *s)
{
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return -1;
    }

    return (s->stack[s->top]);
}

void stackPush(STACK *s, int value)
{
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    //printf("Element %d is pushed to stack \r\n", value);
    s->stack[++s->top] = value;
    return;
}

int stackPop(STACK *s)
{
    if (isStackEmpty(s)) {
        return -1;
    }

    //printf("Element %d is poped out of stack \r\n", s->stack[s->top]);
    return (s->stack[s->top--]);
}

void initVisited(GRAPH *g)
{
    memset(g->visited, false, sizeof(g->visited));
    memset(g->dfsVisitPath, false, sizeof(g->dfsVisitPath));
    memset(g->distance, INT_MAX, sizeof(g->distance));

    return;
}

// function to initialise the adjacency matrix
GRAPH * initGrapth(int num)
{
    GRAPH *g = malloc(sizeof(GRAPH));
    memset(g, 0, sizeof(GRAPH));

    g->numOfNodes = num;
    initVisited(g);

    return g;
}


// Function to add edge between nodes
void addEdge(GRAPH *g, int a, int b, int weight)
{
    g->graph[a][b] = weight;
}

// Function to add edge between nodes
void removeEdge(GRAPH *g, int a, int b)
{
    g->graph[a][b] = -1;
}

void dfs(int node, STACK *st, GRAPH *g)
{
    g->visited[node] = true;

    for(int i = 0; i < g->numOfNodes; i++) {

        if (false == g->graph[node][i]) {
            continue;
        }

        if (i == node) {
            continue;
        }

        if(false == g->visited[i]) {
            dfs(i, st, g);
        }
    }

    printf("[DFS] Pushed into stack : %d \r\n", node);
    stackPush(st, node);
}

void revDfs(int node, GRAPH *g)
{
    printf("%3d", node);
    g->visited[node] = true;

    for(int i = 0; i < g->numOfNodes; i++) {

        if (false == g->transGraph[node][i]) {
            continue;
        }

        if (i == node) {
            continue;
        }

        if(false == g->visited[i]) {
            revDfs(i, g);
        }
    }

    return;
}

int main()
{
    GRAPH *g = initGrapth(11);
    addEdge(g, 0, 1, 1);
    addEdge(g, 0, 2, 1);
    addEdge(g, 0, 10, 1);
    addEdge(g, 1, 9, 1);
    addEdge(g, 1, 2, 1);
    addEdge(g, 2, 3, 1);
    addEdge(g, 3, 0, 1);
    addEdge(g, 4, 0, 1);
    addEdge(g, 4, 3, 1);
    addEdge(g, 4, 5, 1);
    addEdge(g, 5, 6, 1);
    addEdge(g, 6, 4, 1);
    addEdge(g, 6, 7, 1);
    addEdge(g, 7, 10, 1);
    addEdge(g, 7, 8, 1);
    addEdge(g, 8, 9, 1);
    addEdge(g, 9, 10, 1);
    addEdge(g, 10, 8, 1);

    // Expected SCC:
    //  7
    //  10, 8, 9
    //  0, 1, 3, 2
    //  4, 5, 6

    STACK *st = initStack();
    for(int i = 0; i < g->numOfNodes; i++) {
        if(false == g->visited[i]) {
            dfs(i, st, g);
        }
    }

    for(int i = 0; i < g->numOfNodes; i++) {
        for(int j = 0; j < g->numOfNodes; j++) {
            if (true == g->graph[i][j]) {
                g->transGraph[j][i] = true;
            }
        }
    }

    printf("\r\n");
    printf("Input Graph :\r\n");
    for(int i = 0; i < g->numOfNodes; i++) {
        for(int j = 0; j < g->numOfNodes; j++) {
            printf("%2d", g->graph[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    printf("Trasposed Graph :\r\n");
    for(int i = 0; i < g->numOfNodes; i++) {
        for(int j = 0; j < g->numOfNodes; j++) {
            printf("%2d", g->transGraph[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");

    initVisited(g);
	while(false == isStackEmpty(st)) {

	    int node = stackPop(st);
	    if(false == g->visited[node]) {

	        printf("SCC: ");
	        revDfs(node, g);
	        printf("\r\n");
	    }
	}

	return 0;
}