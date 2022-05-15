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
    https://takeuforward.org/data-structure/rotten-oranges-min-time-to-rot-all-oranges-bfs/
    Rotten Oranges : Min time to rot all oranges : BFS
    Problem Statement: You will be given an m x n grid, where each cell has the following values :

    2   represents a rotten orange
    1   represents a Fresh orange
    0   represents an Empty Cell
    Every minute, if a Fresh Orange is adjacent to a Rotten Orange in 4-direction ( upward, downwards, right, and left ) it becomes Rotten.

    Return the minimum number of minutes required such that none of the cells has a Fresh Orange. If it’s not possible, return -1.
    Examples:
        Example 1:
            Input: grid - [ [2,1,1] , [0,1,1] , [1,0,1] ]
            Output: -1

        Example 2:
            Input: grid - [ [2,1,1] , [1,1,0] , [0,1,1] ]
            Output:  4
*/

typedef struct cell {
    int row;
    int col;
} CELL;

CELL* newCell(int row, int col)
{
    CELL* newNode = malloc(sizeof(CELL));
    newNode->row = row;
    newNode->col = col;

    return newNode;
}

#define MAX 1000
typedef struct Queue_ {
    CELL* queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

QUEUE* initQueue()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = MAX;
    return queue;
}

bool isQueueFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQueueEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

CELL* queueFront(QUEUE *q)
{
    CELL* val;
    if (isQueueEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
    }

    val = q->queue[q->start];
    return val;
}

int queueSize(QUEUE *q)
{
    return q->currSize;
}

void enQueue(QUEUE *q, CELL* val)
{
    if (isQueueFull(q)) {
        printf("Queue is full\r\nExiting...\r\n");
    }

    if (q->end == -1) {
        q->end = 0;
        q->start = 0;
    } else {
        q->end = (q->end + 1) % q->maxSize;
    }

    q->queue[q->end] = val;
    //printf("Element %d is enqued \r\n", val);
    q->currSize++;
}

CELL* deQueue(QUEUE *q)
{
    CELL* val;
    if (isQueueEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
    }

    val = q->queue[q->start];
    if (q->currSize == 1) {
        q->end = -1;
        q->start = -1;
    } else {
        q->start = (q->start + 1) % q->maxSize;
    }

    q->currSize--;
    return val;
}

int orangesRotting(int grid[3][3], int row, int col)
{
    QUEUE* rotten = initQueue();

    int m = row;
    int n = col;
    int days = 0;
    int tot = 0;
    int cnt = 0;

    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){

            if(grid[i][j] != 0) {
                tot++;
            }

            if(grid[i][j] == 2) {
                enQueue(rotten, newCell(i, j));
            }
        }
    }

    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    while(false == isQueueEmpty(rotten)) {

        int k = queueSize(rotten);
        cnt += k;
        while(k--) {

            CELL* val = deQueue(rotten);
            int x = val->row;
            int y = val->col;

            for(int i = 0; i < 4; ++i) {

                int nx = x + dx[i];
                int ny = y + dy[i];

                if ( nx < 0 ||
                     ny < 0 ||
                     nx >= m ||
                     ny >= n ||
                     grid[nx][ny] != 1) {
                         continue;
                }

                grid[nx][ny] = 2;
                enQueue(rotten, newCell(nx, ny));
            }
        }

        if(false == isQueueEmpty(rotten)) {
            days++;
        }
    }

    return ((tot == cnt) ? days : -1);
}

int main ()
{
    int grid[3][3] = { {2,1,1} , {1,1,0} , {0,1,1} };

    int rotting = orangesRotting(grid, 3, 3);

    printf("Minimum Number of Minutes Required %d \r\n", rotting);
}