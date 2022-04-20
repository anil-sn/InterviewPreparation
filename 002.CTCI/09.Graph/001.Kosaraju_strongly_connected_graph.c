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
    A directed graph is strongly connected if there is a path between all
    pairs of vertices. A strongly connected component (SCC) of a directed
    graph is a maximal strongly connected subgraph.

    Given a directed graph, find out whether the graph is strongly connected or not.
    A directed graph is strongly connected if there is a path between any two pair
    of vertices.

    It is easy for undirected graph, we can just do a BFS and DFS starting
    from any vertex. If BFS or DFS visits all vertices, then the given
    undirected graph is connected. This approach won’t work for a directed graph.
    For example, consider the following graph which is not strongly connected.
    If we start DFS (or BFS) from vertex 0, we can reach all vertices,
    but if we start from any other vertex, we cannot reach all vertices.

    We can find all strongly connected components in O(V+E) time using Kosaraju’s algorithm.
    Following is detailed Kosaraju’s algorithm.
    1) Create an empty stack ‘S’ and do DFS traversal of a graph.
        In DFS traversal, after calling recursive DFS for adjacent vertices of a vertex,
        push the vertex to stack.
    2) Reverse directions of all arcs to obtain the transpose graph.
    3) One by one pop a vertex from S while S is not empty.
    Let the popped vertex be ‘v’. Take v as source and do DFS (call DFSUtil(v)).
    The DFS starting from v prints strongly connected component of v.


    How does this work?
    The above algorithm is DFS based. It does DFS two times.
    DFS of a graph produces a single tree if all vertices are reachable from the DFS starting point.
    Otherwise DFS produces a forest. So DFS of a graph with only one SCC always produces a tree.
    The important point to note is DFS may produce a tree or a forest when there are more than one
    SCCs depending upon the chosen starting point.

    In the next step, we reverse the graph. Consider the graph of SCCs.
    In the reversed graph, the edges that connect two components are reversed.
    if we do a DFS of the reversed graph using sequence of vertices in stack,
    we process vertices from sink to source (in reversed graph).
    That is what we wanted to achieve and that is all needed to print SCCs one by one.

    Kosaraju’s DFS based simple algorithm that does two DFS traversals of graph:
    1) Initialize all vertices as not visited.
    2) Do a DFS traversal of graph starting from any arbitrary vertex v.
       If DFS traversal doesn’t visit all vertices, then return false.
    3) Reverse all arcs (or find transpose or reverse of graph)
    4) Mark all vertices as not-visited in reversed graph.
    5) Do a DFS traversal of reversed graph starting from same vertex v (Same as step 2).
        If DFS traversal doesn’t visit all vertices, then return false. Otherwise return true.

    The idea is, if every node can be reached from a vertex v, and every node can reach v,
    then the graph is strongly connected.
    In step 2, we check if all vertices are reachable from v.
    In step 4, we check if all vertices can reach v
    (In reversed graph, if all vertices are reachable from v, then all
    vertices can reach v in original graph).
*/

#define MAX_SIZE 5

struct Graph *graph;
struct Graph *gr;

int stack[MAX_SIZE], top;

// A structure to represent an adjacency list node
struct adj_list_node
{
    int dest;
    int weight;
    struct adj_list_node *next;
};

// A structure to represent an adjacency list
struct adj_list
{
    struct adj_list_node *head;
};

// A structure to represent a graph
struct Graph
{
    int V;
    int *visited;
    struct adj_list *array;
};

// Function to push item to stack
void push(int x) {
    if (top >= MAX_SIZE - 1) {
        printf("\n\tSTACK is over flow");
    } else  {
        top++;
        stack[top] = x;
    }
}

// Function to pop item to stack
void pop() {
    if (top <= -1) {
        printf("\n\t Stack is under flow");
    } else {
        top--;
    }
}
// Function to create a new adjacency list node
struct adj_list_node *new_adj_list_node(int dest, int weight)
{
    struct adj_list_node *newNode = (struct adj_list_node *)malloc(sizeof(struct adj_list_node));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Function to creates a graph with V vertices
struct Graph *create_graph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;

    graph->array = (struct adj_list *)malloc(V * sizeof(struct adj_list));

    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Fuction to add edges to transpose graph
void get_transpose(struct Graph *gr, int src, int dest, int weight)
{
    struct adj_list_node *newNode = new_adj_list_node(src, weight);
    newNode->next = gr->array[dest].head;
    gr->array[dest].head = newNode;
}

// Fuction to add edges to graph
void add_edge(struct Graph *graph, struct Graph *gr, int src, int dest, int weight)
{
    struct adj_list_node *newNode = new_adj_list_node(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    get_transpose(gr, src, dest, weight);
}

// Function to print the graph
void print_graph(struct Graph *graph1)
{
    int v;
    for (v = 0; v < graph1->V; ++v) {
        struct adj_list_node *temp = graph1->array[v].head;
        while (temp) {
            printf("(%d -> %d(%d))\t", v, temp->dest, temp->weight);
            temp = temp->next;
        }
    }
}

// Fuction to fill the stack
void set_fill_order(struct Graph *graph, int v, bool visited[], int *stack)
{
    visited[v] = true;

    int i = 0;
    struct adj_list_node *temp = graph->array[v].head;
    while (temp) {
        if (!visited[temp->dest]) {
            set_fill_order(graph, temp->dest, visited, stack);
        }
        temp = temp->next;
    }

    push(v);
}

// A recursive function to print DFS starting from v
void dfs_recursive(struct Graph *gr, int v, bool visited[])
{
    visited[v] = true;
    printf("%d ", v);
    struct adj_list_node *temp = gr->array[v].head;

    while (temp) {
        if (!visited[temp->dest]) {
            dfs_recursive(gr, temp->dest, visited);
        }
        temp = temp->next;
    }
}

void strongly_connected_components(struct Graph *graph, struct Graph *gr, int V)
{
    bool visited[V];
    for (int i = 0; i < V; i++){
        visited[i] = false;
    }

    for (int i = 0; i < V; i++) {
        if (visited[i] == false) {
            set_fill_order(graph, i, visited, stack);
        }
    }

    int count = 1;
    for (int i = 0; i < V; i++) {
        visited[i] = false;
    }

    while (top != -1) {
        int v = stack[top];
        pop();

        if (visited[v] == false) {
            printf("Strongly connected component %d: \n", count++);
            dfs_recursive(gr, v, visited);
            printf("\n");
        }
    }
}

int main()
{
    top = -1;
    int v = 5;

    struct Graph *graph = create_graph(v);
    struct Graph *gr = create_graph(v);

    add_edge(graph, gr, 1, 0, 2);
    add_edge(graph, gr, 0, 2, 2);
    add_edge(graph, gr, 2, 1, 2);
    add_edge(graph, gr, 0, 3, 2);
    add_edge(graph, gr, 3, 4, 2);

    strongly_connected_components(graph, gr, v);

    return 0;
}