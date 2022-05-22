#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define GATE 0
#define EMPTY 2147483647
#define WALL -1

int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

typedef struct queue {
    int front;
    int rear;
    int currItemCount;
    int *arrayRow;
    int *arrayCol;
} queue_t;

queue_t *
createQueue (int queueSize)
{
    queue_t *queue = malloc (sizeof (queue_t));

    if (!queue) {
        return NULL;
    }

    queue->front = 0;
    queue->rear = -1;
    queue->currItemCount = 0;
    queue->arrayRow = malloc (sizeof (int) * queueSize) ;
    queue->arrayCol = malloc (sizeof (int) * queueSize) ;

    if (! (queue->arrayRow) || ! (queue->arrayCol)) {
        return NULL;
    }

    return queue;
}

void
insertQueue (queue_t *queue, int rowIdx, int colIdx)
{
    queue->arrayRow[++queue->rear] = rowIdx;
    queue->arrayCol[queue->rear] = colIdx;
    queue->currItemCount++;
}

int
removeQueueRow (queue_t *queue)
{
    return queue->arrayRow[queue->front];
}

int
removeQueueCol (queue_t *queue)
{
    queue->currItemCount--;
    return queue->arrayCol[queue->front++];
}

bool
isQueueEmpty (queue_t *queue)
{
    return queue->currItemCount == 0;
}

void
freeQueue (queue_t *queue)
{
    free (queue->arrayRow);
    free (queue->arrayCol);
    free (queue);
}

void
wallsAndGates (int **rooms, int roomsSize, int *roomsColSize)
{
    if (roomsSize == 0 || *roomsColSize == 0) {
        return;
    }

    int numOfRows = roomsSize;
    int numOfCols = *roomsColSize;
    queue_t *queue = createQueue (numOfRows * numOfCols);

    // iterate over entire grid and find out where GATE is located
    // add the GATE to our list for traversal
    for (int r = 0; r < numOfRows; r++) {
        for (int c = 0; c < numOfCols; c++) {
            if (rooms[r][c] == GATE) {
                insertQueue (queue, r, c);
            }
        }
    }

    // iterate over all GATEs and traverse away from GATE
    // for each successful empty room, increment its count as distance
    while (!isQueueEmpty (queue)) {
        int rowIdx = removeQueueRow (queue);
        int colIdx = removeQueueCol (queue);

        for (int i = 0; i < 4; i++) {
            int r = rowIdx + directions[i][0];
            int c = colIdx + directions[i][1];

            if (r >= numOfRows || c >= numOfCols ||
                    r < 0 || c < 0 || rooms[r][c] != EMPTY) {
                continue;
            }

            rooms[r][c] = rooms[rowIdx][colIdx] + 1;
            insertQueue (queue, r, c);
        }
    }
}