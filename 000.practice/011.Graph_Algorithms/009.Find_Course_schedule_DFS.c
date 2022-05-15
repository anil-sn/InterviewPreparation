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
    https://leetcode.com/problems/course-schedule-ii/
    210. Course Schedule II
    There are a total of numCourses courses you have to take, labeled from 0 to numCourses - 1. You are given an array prerequisites where prerequisites[i] = [ai, bi] indicates that you must take course bi first if you want to take course ai.

    For example, the pair [0, 1], indicates that to take course 0 you have to first take course 1.
    Return the ordering of courses you should take to finish all courses. If there are many valid answers, return any of them. If it is impossible to finish all courses, return an empty array.



    Example 1:

    Input: numCourses = 2, prerequisites = [[1,0]]
    Output: [0,1]
    Explanation: There are a total of 2 courses to take. To take course 1 you should have finished course 0. So the correct course order is [0,1].
    Example 2:

    Input: numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
    Output: [0,2,1,3]
    Explanation: There are a total of 4 courses to take. To take course 3 you should have finished both courses 1 and 2. Both courses 1 and 2 should be taken after you finished course 0.
    So one correct course order is [0,1,2,3]. Another correct ordering is [0,2,1,3].
    Example 3:

    Input: numCourses = 1, prerequisites = []
    Output: [0]
*/

// A DFS based function to check if there is a cycle in the directed graph.
int adj[100][100];
#define MAX 1000

typedef struct queue_ {
    int queue[MAX];
    int front;
    int rear;
} QUEUE;

void initializeQueue(QUEUE *q)
{
	q->rear=-1;
	q->front=-1;
}

bool isEmpty(QUEUE *q)
{
	if(q->front==-1 || q->front == q->rear+1)
		return true;
	else
		return false;
}

bool isFull(QUEUE *q)
{
	if(q->rear == MAX-1)
		return true;
	else
		return false;
}

void enqueue(QUEUE *q, int x)
{
	if(isFull(q))
	{
		printf("Queue Overflow\n");
		return;
	}

	if(q->front == -1) {
		q->front = 0;
    }

	q->rear = q->rear + 1;
	q->queue[q->rear] = x;
}

int dequeue(QUEUE *q)
{
	int x;
	if(isEmpty(q))
	{
		printf("Queue Underflow\n");
		return -1;
	}

	x= q->queue[q->front];
	q->front = q->front+1;
	return x;
}

void addUnDirectedEdge(int v,int w)
{
    adj[w][v] = 1;
    adj[v][w] = 1;
}

void addDirectedEdge(int v,int w)
{
    adj[v][w] = 1;
}

void
printAdjMatrix (int n) {

    printf("Adjecency Matrix : \r\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", adj[i][j]);
        }
        printf("\r\n");
    }
    printf("\r\n");
    return;
}

void printCoursesOrder (int numCourses) {

    int in_degree[numCourses];
    memset(in_degree, 0, sizeof(in_degree));

    // Traverse adjacency lists to fill indegrees of vertices.
    // This step takes O(V+E) time
    for (int i = 0; i < numCourses; i++) {
        for (int j = 0; j < numCourses; j++) {
            if (0 != adj[i][j]) {
                in_degree[j] = in_degree[j] + 1;
            }
        }
    }

    // Create an queue and enqueue all vertices with indegree 0
    QUEUE q;
    initializeQueue(&q);

    for (int i = 0; i < numCourses; i++) {
        if (in_degree[i] == 0) {
            enqueue(&q, i);
        }
    }

    // Initialize count of visited vertices
    int cnt = 0;

    // Create a vector to store result (A topological ordering of the vertices)
    int top_order[numCourses];
    int topPos = numCourses - 1;
    memset(top_order, 0, sizeof(top_order));

    // One by one dequeue vertices from queue and enqueue adjacents if indegree of adjacent becomes 0
    while (false == isEmpty(&q)) {

        // Extract front of queue (or perform dequeue) and add it to topological order
        int u = dequeue(&q);
        top_order[topPos--] = u;

        for (int j = 0; j < numCourses; j++) {
            if (0 != adj[u][j]) {
                in_degree[j] = in_degree[j] - 1;
                if (0 == in_degree[j]) {
                    enqueue(&q, j);
                }
            }
        }

        cnt++;
    }

    // Check if there was a cycle
    if (cnt != numCourses) {
        printf("There exists a cycle in the graph \r\n");
        return;
    }

    // Print topological order
    printf("Topological Order:  ");
    for (int j = 0; j < numCourses; j++) {
        printf("%d ", top_order[j]);
    }
    printf("\r\n");

    return;
}

int main ()
{
    int numCourses = 4;
    int prerequisites[][2] = {{ 1, 0 },
                              { 2, 0 },
                              { 3, 1 },
                              { 3, 2 }};
    int numRows = 4;

    for (int i = 0; i < numRows; i++) {
        addDirectedEdge(prerequisites[i][0], prerequisites[i][1]);
    }

    printAdjMatrix(numCourses);
    printCoursesOrder(numCourses);
}