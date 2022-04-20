#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"
#include "limits.h"
#include "math.h"
#include "sys/types.h"
#include "assert.h"

/*
    4.7 Build Order: You are given a list of projects and a list of dependencies
                    (which is a list of pairs of projects, where the second project is dependent on the first project).
                    All of a project's dependencies must be built before the project is.
                    Find a build order that will allow the projects to be built.
                    If there is no valid build order, return an error.
    EXAMPLE
    Input:  projects: a, b, c, d, e, f
            dependencies: (a, d), (f, b), (b, d), (f, a), (d, c)
    Output: f, e, a, b, d, c

    https://www.geeksforgeeks.org/find-the-ordering-of-tasks-from-given-dependencies/

    There are a total of n tasks you have to pick, labeled from 0 to n-1. Some tasks may have prerequisites tasks,
    for example to pick task 0 you have to first finish tasks 1, which is expressed as a pair: [0, 1]

    Given the total number of tasks and a list of prerequisite pairs,
    return the ordering of tasks you should pick to finish all tasks.

    There may be multiple correct orders, you just need to return one of them.
    If it is impossible to finish all tasks, return an empty array.

    Examples:

    Input: 2, [[1, 0]]
    Output: [0, 1]
    Explanation: There are a total of 2 tasks to pick.
                 To pick task 1 you should have finished task 0. So the correct task order is [0, 1] .

    Input: 4, [[1, 0], [2, 0], [3, 1], [3, 2]]
    Output: [0, 1, 2, 3]
         or [0, 2, 1, 3]
    Explanation: There are a total of 4 tasks to pick.
                 To pick task 3 you should have finished both tasks 1 and 2.
                 Both tasks 1 and 2 should be pick after you finished task 0.
                 So one correct task order is [0, 1, 2, 3].
                 Another correct ordering is [0, 2, 1, 3].

    Solution: We can consider this problem as a graph (related to topological sorting) problem.
              All tasks are nodes of the graph and if task u is a prerequisite of task v,
              we will add a directed edge from node u to node v.
              Now, this problem is equivalent to finding a topological ordering of nodes/tasks (using topological sorting)
              in the graph represented by prerequisites. If there is a cycle in the graph, then it is not possible to
              finish all tasks (because in that case there is no any topological order of tasks).
              Both BFS and DFS can be used for topological sorting to solve it.

              Since pair is inconvenient for the implementation of graph algorithms,
              we first transform it to a graph. If task u is a prerequisite of task v,
              we will add a directed edge from node u to node v.

    Topological Sorting using BFS
             Here we use Kahn’s algorithm for topological sorting.
             BFS uses the indegrees of each node. We will first try to find a node with 0 indegree.
             If we fail to do so, there must be a cycle in the graph and we return false.
             Otherwise we have found one. We set its indegree to be -1 to prevent from visiting it again
             and reduce the indegrees of all its neighbors by 1.
             This process will be repeated for n (number of nodes) times.

    https://www.thecrazyprogrammer.com/2017/05/topological-sort.html
    http://www.codezclub.com/c-topological-sorting-algorithm-example/

*/

#define MAX 100

int n;    /*Number of vertices in the graph*/
int adj[MAX][MAX]; /*Adjacency Matrix*/
void create_graph();

int queue[MAX], front = -1,rear = -1;
void insert_queue(int v);
int delete_queue();
int isEmpty_queue();

int indegree(int v);

void insert_queue(int vertex)
{
        if (rear == MAX-1) {
              printf("\nQueue Overflow\n");
        }
        else
        {
                if (front == -1)  /*If queue is initially empty */
                        front = 0;
                rear = rear+1;
                queue[rear] = vertex ;
        }
}/*End of insert_queue()*/

int isEmpty_queue()
{
        if(front == -1 || front > rear )
                return 1;
        else
                return 0;
}/*End of isEmpty_queue()*/

int delete_queue()
{
        int del_item;
        if (front == -1 || front > rear)
        {
                printf("\nQueue Underflow\n");
                exit(1);
        }
        else
        {
                del_item = queue[front];
                front = front+1;
                return del_item;
        }
}/*End of delete_queue() */

int indegree(int v)
{
        int i,in_deg = 0;
        for(i=0; i<n; i++)
                if(adj[i][v] == 1)
                        in_deg++;
        return in_deg;
}/*End of indegree() */

void create_graph()
{
        int i,max_edges,origin,destin;

        printf("\nEnter number of vertices : ");
        scanf("%d",&n);
        max_edges = n*(n-1);

        for(i=1; i<=max_edges; i++)
        {
                printf("\nEnter edge %d(-1 -1 to quit): ",i);
                scanf("%d %d",&origin,&destin);

                if((origin == -1) && (destin == -1))
                        break;

                if( origin >= n || destin >= n || origin<0 || destin<0)
                {
                        printf("\nInvalid edge!\n");
                        i--;
                }
                else
                        adj[origin][destin] = 1;
        }
}


int main()
{
        int i,v,count,topo_order[MAX],indeg[MAX];

        create_graph();

        /*Find the indegree of each vertex*/
        for(i=0;i<n;i++)
        {
                indeg[i] = indegree(i);
                if( indeg[i] == 0 )
                        insert_queue(i);
        }

        count = 0;

        while(  !isEmpty_queue( ) && count < n )
        {
                v = delete_queue();
        topo_order[++count] = v; /*Add vertex v to topo_order array*/
                /*Delete all edges going fron vertex v */
                for(i=0; i<n; i++)
                {
                        if(adj[v][i] == 1)
                        {
                                adj[v][i] = 0;
                                indeg[i] = indeg[i]-1;
                                if(indeg[i] == 0)
                                        insert_queue(i);
                        }
                }
        }

        if( count < n )
        {
                printf("\nNo topological ordering possible, graph contains cycle\n");
                exit(1);
        }
        printf("\nVertices in topological order are :\n");
        for(i=1; i<=count; i++)
                printf( "%d ",topo_order[i] );
        printf("\n");

        return 0;
}/*End of main()*/

