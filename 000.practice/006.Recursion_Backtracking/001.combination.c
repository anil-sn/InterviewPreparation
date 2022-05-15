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
    1. You are give a number of boxes (nboxes) and number of identical items (ritems).
    2. You are required to place the items in those boxes and print all such configurations possible.

    Items are identical and all of them are named 'i'
    Note 1 -> Number of boxes is greater than number of items, hence some of the boxes may remain
                    empty.
    Note 2 -> Check out the question video and write the recursive code as it is intended without
                    changing signature. The judge can't force you but intends you to teach a concept.
    Input Format
    Input is managed for you
    Output Format
    Check the sample ouput and question video. - means empty box, i means occupied by item.
*/


void combinations(int boxes[], int totalBoxes, int currentItem, int totalItem, int lastBox){
    if (currentItem > totalItem) {
        for(int i = 0; i < totalBoxes; i++){
            if (boxes[i]) {
                printf("i");
            } else {
                printf("-");
            }
        }
        printf("\r\n");
        return;
    }

    for (int i = (lastBox + 1) ; i < totalBoxes; i++){
        if (boxes[i] == 0) {
            boxes[i] = 1;
            combinations(boxes, totalBoxes, currentItem+1, totalItem, i);
            boxes[i] = 0;
        }
    }
}

int main ()
{
    int nboxes[3] = {0};
    int totalBoxes = 3;
    int ritems = 2;

    combinations(nboxes, totalBoxes, 1, ritems, -1);

    return 0;
}