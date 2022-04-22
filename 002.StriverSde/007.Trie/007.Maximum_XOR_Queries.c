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
    Max Xor Queries.

    Problem Statement
        You are given an array/list ‘ARR’ consisting of ‘N’ non-negative integers. You are also given a list ‘QUERIES’ consisting of ‘M’ queries, where the ‘i-th’ query is a list/array of two non-negative integers ‘Xi’, ‘Ai’, i.e ‘QUERIES[i]’ = [‘Xi’, ‘Ai’].
        The answer to the ith query, i.e ‘QUERIES[i]’ is the maximum bitwise xor value of ‘Xi’ with any integer less than or equal to ‘Ai’ in ‘ARR’.
        You should return an array/list consisting of ‘N’ integers where the ‘i-th’ integer is the answer of ‘QUERIES[i]’.
        Note:
        1. If all integers are greater than ‘Ai’ in array/list ‘ARR’  then the answer to this ith query will be -1.
        Input Format:
        The first line contains a single integer ‘T’ representing the number of test cases.
        The first line of each test case will contain two space-separated integers ‘N’ and ‘M’ representing the size of array/list ‘ARR’ and number of queries respectively.
        The second line of each test case will contain ‘N’ space-separated integers representing array/list ‘ARR’.
        The next ‘M’ line of each test case contains the description of ‘QUERIES’. The ‘i-th’ line of these ‘M’ lines consists of two space-separated integers ‘Xi’, ‘Ai’ as described in the problem statement.

        Output Format:
        For each test case, print ‘M’ space-separated integer where the ‘i-th’ integer is the answer of the ‘i-th’ query.

        Output for every test case will be printed in a separate line.
        Note:
            You don’t need to print anything; It has already been taken care of.
            Constraints:
            1 <= T <= 50
            1 <= N, M <= 10000
            0 <= ARR[i], Xi, Ai <= 10^9

            Where ‘T’ is the number of test cases, 'N' is the size of ‘ARR’, ‘M’  is the number of queries, ‘ARR[i]’ is an element of array/list ‘ARR’ and ‘Xi’, ‘Ai’ are the integers in ‘QUERIES[i]’.

        Time limit: 1 sec
    Sample Input 1:
        2
        5 2
        0 1 2 3 4
        1 3
        5 6
        1 1
        1
        1 0
        Sample Output 1:
        3 7
        -1
        Explanation Of Sample Input 1:
        In the first test case, the answer of query [1, 3] is 3 because 1^2 = 3 and 2 <= 3,  and the answer of query [5, 6] is 7 because  5 ^ 2 = 7 and 2 <= 6.

        In the second test case, no element is less than or equal to 0 in the given array ‘ARR’.
    Sample Input 2:
        2
        6 3
        6 6 3 5 2 4
        6 3
        8 1
        12 4
        5 2
        0 0 0 0 0
        1 0
        1 1
        Sample Output 2:
        5 -1 15
        1 1
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

typedef struct quries_{
    int xi;
    int ai;
    int index;
} QUERY;

int cmp(const void *a, const void *b)
{
    QUERY *x = (QUERY *)a;
    QUERY *y = (QUERY *)b;

    return (x->ai - y->ai);
}

void maxXOR(int n, int m, int arr1[], QUERY arr2[], int result[])
{
    Trie trie;
    trieInit(&trie);
    Trie *t = &trie;
    int idx = 0;

    printf("\r\n Input Quries \r\n");
    for (int i = 0; i < m; i++) {
        printf("xi = %d, ai = %d, index = %d \r\n", arr2[i].xi, arr2[i].ai, arr2[i].index);
    }

    qsort(arr2, m, sizeof(QUERY), cmp);

    printf("\r\n Input Sorted Quries \r\n");
    for (int i = 0; i < m; i++) {
        printf("xi = %d, ai = %d, index = %d \r\n", arr2[i].xi, arr2[i].ai, arr2[i].index);
        while (idx  < n && arr1[idx] <= arr2[i].ai) {
            trieInsert(t, arr1[idx]);
            idx++;
        }
        if (idx == 0) {
            result[arr2[i].index] = -1;
        } else {
            result[arr2[i].index] = trieFindMax(t, arr2[i].xi);
        }
    }

    printf("\r\n Result \r\n");
    for (int i = 0; i < n; i++) {
        printf("%d \r\n", result[i]);
    }
    printf("\r\n");

}

int main() {
    int arr1[] = {1, 3, 2, 5, 4};
    QUERY arr2[] = {{3, 4, 0}, {5, 2, 1}, {2, 5, 2}, {3, 1, 3}};
    int n = sizeof(arr1)/sizeof(arr1[0]);
    int m = sizeof(arr2)/sizeof(arr2[0]);
    int result[n];
    memset(result, INT_MIN, sizeof(result));

    maxXOR(n, m, arr1, arr2, result);
    return 0;
}