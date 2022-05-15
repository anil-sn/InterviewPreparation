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
    1. You are give a number of boxes (nboxes) and number of non-identical items (ritems).
    2. You are required to place the items in those boxes and print all such configurations possible.

    Items are numbered from 1 to ritems.
    Note 1 -> Number of boxes is greater than number of items, hence some of the boxes may remain
                    empty.
    Note 2 -> Check out the question video and write the recursive code as it is intended without
                    changing signature. The judge can't force you but intends you to teach a concept.
    Input Format
    Input is managed for you
    Output Format
    Check the sample ouput and question video. 0 means empty box.
*/


void permutations(int boxes[], int n, int ci, int ritems){
    if (ci > ritems) {
        for(int i = 0; i < n; i++){
            printf("%d", boxes[i]);
        }
        printf("\r\n");
        return;
    }

    for (int i =0; i < n; i++){
        if (boxes[i] == 0) {
            boxes[i] = ci;
            permutations(boxes, n, ci+1, ritems);
            boxes[i] = 0;
        }
    }
}

int main ()
{
    int nboxes[3] = {0};
    int ritems = 2;
    int n = 3;

    permutations(nboxes, n, 1, ritems);

    return 0;
}