
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

void printAdjMatrix(int G[][8]) {

    // Traverse the Adj[][]
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            // Print the value at Adj[i][j]
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
}
void DFS(int G[8][8], int visited[8], int i) {
    int j;
    printf("\t\tVisit %d\n",i);
    visited[i]=1;
    for(j=0; j<8; j++) {
        printf ("i = %d, j = %d\r\n", i, j);
        if(!visited[j] && G[i][j]==1) {
            DFS(G, visited, j);
        }
        printf("\t\t\tAlready Visited %d\n",j);
    }
}

//n is no of vertices and graph is sorted in array G[10][10]
int main() {
    int i,j;
    int G[8][8] = {{0, 1, 1, 1, 1, 0, 0, 0},
                   {1, 0, 0, 0, 0, 1, 0, 0},
                   {1, 0, 0, 0, 0, 1, 0, 0},
                   {1, 0, 0, 0, 0, 0, 1, 0},
                   {1, 0, 0, 0, 0, 0, 1, 0},
                   {0, 1, 1, 0, 0, 0, 0, 1},
                   {0, 0, 0, 1, 1, 0, 0, 1},
                   {0, 0, 0, 0, 0, 1, 1, 0}};
    int visited[10];
    int n;

    n = 8;

    printAdjMatrix(G);

    //visited is initialized to zero
    for(i=0; i<n; i++)
        visited[i]=0;

    DFS(G, visited, 0);
}
