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
    Maximum XOR of Two Numbers in an Array

    Problem Statement: You are given two arrays of non-negative integers say ‘arr1’ and ‘arr2’ of sizes N
    and M respectively. Find the maximum value of ( ‘A’ xor ‘B’ ) where ‘A’ and ‘B’ are any elements from
    ‘arr1’ and ‘arr2’ respectively and ‘xor’ represents the bitwise xor operation. Maximum XOR of
    Two Numbers in an Array.

    Examples:
        Example 1:
            Input: N=2, M=3
                arr1 = [6, 8]
                arr2 = [7, 8, 2]
            Output: 15

            Explanation: Possible pairs are (6, 7), (6, 8), (6, 2), (8, 7), (8, 8), (6, 2).
            And 8 xor 7 will give the maximum result i.e. 15

        Example 2:
            Input: N=2, M=2
                arr1 = [1, 2 ]
                arr2 = [1, 1]
            Output: 3

            Explanation: 3 is the maximum possible xor among all possible pairs.
*/
typedef struct TrieNode {
    struct TrieNode *links[2];
} TrieNode;

TrieNode * newTrieNode()
{
    TrieNode *node = calloc(1, sizeof(TrieNode));
    return node;
}

bool trieNodeContainsKey(TrieNode *t, int ind) {
    return (t->links[ind] != NULL);
}

TrieNode * trieNodeGet(TrieNode *t, int ind) {
    return t->links[ind];
}

void trieNodePut(TrieNode *t, int ind, TrieNode * node) {
    t->links[ind] = node;
}

typedef struct Trie_ {
	TrieNode* root;
} Trie;

void trieInit(Trie *t)
{
    t->root =  newTrieNode();
    return;
}

void trieInsert(Trie *t, int num)
{
    TrieNode * node = t->root;

    for (int i = 31; i >= 0; i--) {

        int bit = (num >> i) & 1;

        if (false ==  trieNodeContainsKey(node, bit)) {
            trieNodePut(node, bit, newTrieNode());
        }

        node = trieNodeGet(node, bit);
    }
}

int trieFindMax(Trie *t, int num)
{
    TrieNode * node = t->root;
    int maxNum = 0;

    for (int i = 31; i >= 0; i--) {

        int bit = (num >> i) & 1;

        if (trieNodeContainsKey(node, !bit)) {
            maxNum = maxNum | (1 << i);
            node = trieNodeGet(node, !bit);
        } else {
            node = trieNodeGet(node, bit);
        }
    }

    return maxNum;
}
#define MAX(a_, b_) (a_ > b_ ? a_ : b_)
int maxXOR(int n, int m, int arr1[], int arr2[])
{
    Trie trie;
    trieInit(&trie);
    Trie *t = &trie;

    for (int i = 0; i < n; i++) {
        trieInsert(t, arr1[i]);
    }

    int maxi = 0;
    for (int i = 0; i < m; i++) {
        maxi = MAX(maxi, trieFindMax(t, arr2[i]));
    }

    return maxi;
}

int main() {
    int arr1[] = {6, 8};
    int arr2[] = {7, 8, 2};
    int n = sizeof(arr1)/sizeof(arr1[0]);
    int m = sizeof(arr2)/sizeof(arr2[0]);

    printf("Maximum XOR Value : %d \r\n", maxXOR(n, m, arr1, arr2));
    return 0;
}