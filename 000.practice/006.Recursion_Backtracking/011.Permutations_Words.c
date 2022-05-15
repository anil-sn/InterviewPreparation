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
    1. You are given a word (may have one character repeat more than once).
    2. You are required to generate and print all arrangements of these characters.

    Note -> Use the code snippet and follow the algorithm discussed in question video. The judge can't
                force you but the intention is to teach a concept. Play in spirit of the question.
    Input Format
    Input is managed for you
*/

void permutations(int currentPos, char boxes[], char input[], int n, int inputCount, int lastSpot){
    if (currentPos == inputCount) {
        for(int i = 0; i < n; i++){
            printf("%c", boxes[i]);
        }
        printf("\r\n");
        return;
    }

    char ch = input[currentPos];
    for (int i = lastSpot; i < n; i++){
        if (boxes[i] == '-') {
            boxes[i] = ch;
            permutations(currentPos + 1, boxes, input, n, inputCount, lastSpot+1);
            boxes[i] = '-';
        }
    }
}

int main ()
{
    char nboxes[4+1];
    memset(nboxes, '-', sizeof(nboxes));
    nboxes[4] = '\0';
    char input[] = "ab";
    int n = 4;
    int inputCount = 2;
    int lastSpot = -1;

    permutations(0, nboxes, input, n, inputCount, lastSpot);

    return 0;
}
