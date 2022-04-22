#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX 100
#define min(a, b)   (a < b ? a : b)

typedef struct Queue_ {
    int queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

QUEUE* newQ()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = MAX;

    return queue;
}

int qSize(QUEUE *q)
{
    return (q->currSize);
}

bool isQFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

void addQ(QUEUE *q, int val)
{
    if (isQFull(q)) {
        printf("Queue is full\r\nExiting...\r\n");
    }

    if (q->end == -1) {
        q->end = 0;
        q->start = 0;
    } else {
        q->end = (q->end + 1) % q->maxSize;
    }

    q->queue[q->end] = val;
    //printf("Element %d is enqued \r\n", val->data);
    q->currSize++;
}

int delQ(QUEUE *q)
{
    int val;
    if (isQEmpty(q)) {
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

// https://open.kattis.com/problems/dungeon
int escapeTheDungeon(int R, int C, char dungeon[R][C])
{
    int moveCount = 0;
    int nodes_left_in_layer = 1;
    int nodes_in_next_layer = 0;

    // North South East West direction vector
    int dr[4] = {-1, +1,  0,  0};
    int dc[4] = { 0,  0, +1, -1};

    bool reached_end = false;
    int visited[R][C];
    memset(visited, 0, sizeof(visited));

    QUEUE *rq = newQ();
    QUEUE *cq = newQ();

    // Start from 0,0
    int sr = 0;
    int sc = 0;

    addQ(rq, sr);
    addQ(cq, sc);

    visited[sr][sc] = true;
    while (false == isQEmpty(rq)) {

        int r = delQ(rq);
        int c = delQ(cq);

        if (dungeon[r][c] == 'E') {
            reached_end = true;
            break;
        }

        // Make this a function explore neighbors
        for (int i = 0; i < 4; i ++) {
            int rr = r + dr[i];
            int cc = c + dc[i];

            if ((rr < 0) || (cc < 0) || (rr >= R) || (cc >= C)) {
                continue;
            }

            if (true == visited[rr][cc]) {
                continue;
            }

            if (dungeon[rr][cc] == '#') {
                continue;
            }

            addQ(rq, rr);
            addQ(cq, cc);
            visited[rr][cc] = true;
            nodes_in_next_layer ++;
        }

        nodes_left_in_layer --;
        if (0 == nodes_left_in_layer){
            nodes_left_in_layer = nodes_in_next_layer;
            nodes_in_next_layer = 0;
            moveCount ++;
        }
    }

    if (true == reached_end){
        return moveCount;
    }

    return -1;
}


int main (void)
{
    char dungeon[5][7] = { {'S','.','.','#','.','.','.'},
                           {'.','#','.','.','.','#','.'},
                           {'.','#','.','.','.','.','.'},
                           {'.','.','#','#','#','#','.'},
                           {'#','.','#','E','.','.','.'}};
    int R = 5;
    int C = 7;

    int time = escapeTheDungeon(R, C, dungeon);
    printf("Time taken to escape the dungeon is : %d \r\n", time);

    return 0;
}