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
    Finite Automata algorithm for Pattern Searching

    Given a text txt[0..n-1] and a pattern pat[0..m-1],
    write a function search(char pat[], char txt[]) that prints all occurrences of pat[] in txt[].
    You may assume that n > m.

    Examples:
    Input:  txt[] = "THIS IS A TEST TEXT"
            pat[] = "TEST"
    Output: Pattern found at index 10

    Input:  txt[] =  "AABAACAADAABAABA"
            pat[] =  "AABA"
    Output: Pattern found at index 0
            Pattern found at index 9
            Pattern found at index 12
    Pattern searching is an important problem in computer science. When we do search for a string in notepad/word file or browser or database, pattern searching algorithms are used to show the search results.
    We have discussed the following algorithms in the previous posts:
    Naive Algorithm
    KMP Algorithm
    Rabin Karp Algorithm

    In this post, we will discuss Finite Automata (FA) based pattern searching algorithm.
    In FA based algorithm, we preprocess the pattern and build a 2D array that represents
    a Finite Automata. Construction of the FA is the main tricky part of this algorithm.
    Once the FA is built, the searching is simple.
    In search, we simply need to start from the first state of the automata and the first
    character of the text. At every step, we consider next character of text,
    look for the next state in the built FA and move to a new state.
    If we reach the final state, then the pattern is found in the text.
     The time complexity of the search process is O(n).

    Before we discuss FA construction, let us take a look at the following FA for pattern ACACAGA.
    https://www.geeksforgeeks.org/finite-automata-algorithm-for-pattern-searching/
*/

#define NO_OF_CHARS 256

int getNextState(char *pat, int M, int state, int x)
{
    // If the character c is same as next character
    // in pattern,then simply increment state
    if (state < M && x == pat[state]) {
        return state+1;
    }

    // ns stores the result which is next state
    int ns, i;

    // ns finally contains the longest prefix
    // which is also suffix in "pat[0..state-1]c"

    // Start from the largest possible value
    // and stop when you find a prefix which
    // is also suffix
    for (ns = state; ns > 0; ns--) {
        if (pat[ns-1] == x) {
            for (i = 0; i < ns-1; i++) {
                if (pat[i] != pat[state-ns+1+i]) {
                    break;
                }
            }
            if (i == ns-1) {
                return ns;
            }
        }
    }

    return 0;
}

/* This function builds the TF table which represents4
    Finite Automata for a given pattern */
void computeTF(char *pat, int M, int TF[][NO_OF_CHARS])
{
    int state, x;
    for (state = 0; state <= M; state++) {
        for (x = 0; x < NO_OF_CHARS; x++) {
            TF[state][x] = getNextState(pat, M, state, x);
        }
    }
}

/* Prints all occurrences of pat in txt */
void search(char *pat, char *txt)
{
    int M = strlen(pat);
    int N = strlen(txt);

    int TF[M+1][NO_OF_CHARS];

    computeTF(pat, M, TF);

    // Process txt over FA.
    int i, state=0;
    for (i = 0; i < N; i++)
    {
        state = TF[state][txt[i]];
        if (state == M) {
            printf ("\n Pattern found at index %d\r\n", i-M+1);
        }
    }
}

// Driver program to test above function
int main()
{
    char *txt = "AABAACAADAABAAABAA";
    char *pat = "AABA";
    search(pat, txt);
    return 0;
}