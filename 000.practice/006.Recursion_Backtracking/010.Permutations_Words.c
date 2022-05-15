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

void permutations(int currentPos, char boxes[], char input[], int n){
    if (currentPos == n) {
        for(int i = 0; i < n; i++){
            printf("%c", boxes[i]);
        }
        printf("\r\n");
        return;
    }

    for (int i = 0; i < n; i++){
        if (boxes[i] == '\0') {
            boxes[i] = input[currentPos];
            permutations(currentPos + 1, boxes, input, n);
            boxes[i] = '\0';
        }
    }
}

int main ()
{
    char nboxes[4+1] = {'\0'};
    char input[] = "abcb";
    int n = 4;

    permutations(0, nboxes, input, n);

    return 0;
}