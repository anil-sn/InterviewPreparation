
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
    Dont practice until you get it right
    practice until you can't get it worng
*/

/*
Depth-first Search

Understanding the problem
We are given an acyclic tree-like graph. Each node in this graph is going to be an
instance of a Node class, and is going to have a name and optionally some children
nodes. We are asked to write a depthFirstSearch method on the Node class, which is
going to performs a depth-first search on the graph. The method is going to accept
an empty array as argument. As we traverse the graph, we need to store the names of
all the nodes in the input array and return that array. We should visit the nodes
from left to right.

Depth First Search (DFS) Program in C

Most of graph problems involve traversal of a graph.
Traversal of a graph means visiting each node and visiting exactly once.
There are two types of traversal in graphs
i.e. Depth First Search (DFS) and Breadth First Search (BFS).

It is like tree. Traversal can start from any vertex, say Vi .
Vi is visited and then all vertices adjacent to Vi are traversed recursively using DFS. Since, a graph can have cycles. We must avoid revisiting a node. To do this, when we visit a vertex V, we mark it visited. A node that has already been marked as visited should not be selected for traversal. Marking of visited vertices can be done with the help of a global array visited[ ]. Array visited[ ] is initialized to false (0).
*/

typedef struct node {
    struct node *next;
    int vertex;
} node;

node *G[20];

//heads of linked list
int visited[20];
int n;

void read_graph();

//create adjacency list
void insert(int,int);

void DFS(int i) {
    node *p;

    printf("\n%d",i);
    p=G[i];
    visited[i]=1;
    while(p!=NULL) {
        i=p->vertex;

        if(!visited[i])
            DFS(i);
        p=p->next;
    }
}

//insert an edge (vi,vj) in te adjacency list
void read_graph() {
    int i,vi,vj,no_of_edges;
    printf("Enter number of vertices:");

    scanf("%d",&n);
    //initialise G[] with a null

    for(i=0; i<n; i++) {
        G[i]=NULL;
    }

    //read edges and insert them in G[]
    printf("Enter number of edges:");
    scanf("%d",&no_of_edges);

    for(i=0; i<no_of_edges; i++) {
        printf("Enter an edge(u,v):");
        scanf("%d%d",&vi,&vj);
        insert(vi,vj);
    }
}

void insert(int vi,int vj) {
    node *p,*q;

    //acquire memory for the new node
    q=(node*)malloc(sizeof(node));
    q->vertex=vj;
    q->next=NULL;

    //insert the node in the linked list number vi
    if(G[vi]==NULL)
        G[vi]=q;
    else {
        //go to end of the linked list
        p=G[vi];

        while(p->next!=NULL)
            p=p->next;
        p->next=q;
    }
}

int main() {
    int i;

    read_graph();
    //initialised visited to 0

    for(i=0; i<n; i++)
        visited[i]=0;
    DFS(0);
}

/*
Input :
    Enter number of vertices:8
    Enter number of edges:10
    Enter an edge(u,v):0 1
    Enter an edge(u,v):0 2
    Enter an edge(u,v):0 3
    Enter an edge(u,v):0 4
    Enter an edge(u,v):1 5
    Enter an edge(u,v):2 5
    Enter an edge(u,v):3 6
    Enter an edge(u,v):4 6
    Enter an edge(u,v):5 7
    Enter an edge(u,v):6 7
Output:
    0
    1
    5
    7
    2
    3
    6
    4
*/