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
    Number of Distinct Substrings in a String Using Trie
    Problem Statement: Given a string of alphabetic characters. Return the count of distinct substrings of the string(including the empty string) using the Trie data structure.

    Examples:

        Example 1:
            Input: S1= “ababa”
            Output: Total number of distinct substrings : 10
            Explanation: All the substrings of the string are
            a, ab, aba, abab, ababa,
            b, ba, bab, baba,
            a, ab, aba, b, ba, a. Many of the substrings are duplicated.
            The distinct substrings are a, ab, aba, abab, ababa,
            b, ba, bab, baba. Total Count of the distinct substrings is 9 + 1
            (for empty string) = 10.



        Example 2:
            Input: S1= “ccfdf”
            Output: Total number of distinct substrings : 14
            Explanation:
            All the substrings of the string are
            c, cc, ccf, ccfd, ccfdf, c, cf, cfd, cfdf,
            f, fd, fdf, d, df, f. Many of the substrings are duplicated.
            The distinct substrings are c, cc, ccf, ccfd, ccfdf, cf, cfd, cfdf,
            f, fd, fdf, d, df. Total Count of the distinct substrings is 13 + 1
            (for empty string) = 14.
*/


#define ALPHABET_SIZE 26
typedef struct TrieNode {

    struct TrieNode *children[ALPHABET_SIZE];
    int cntEndWith;
    int cntPrefix;
} TrieNode;

bool trieNodeContainKey(TrieNode *t, char ch)
{
    if (NULL != t->children[ch - 'a']) {
        return true;
    }

    return false;
}

void trieNodePutData(TrieNode *t, char ch, TrieNode *n)
{
    if (NULL == t->children[ch - 'a']) {
        t->children[ch - 'a'] = n;
    }

    return;
}

TrieNode* trieNodeGetData(TrieNode *t, char ch)
{
    if (NULL == t->children[ch - 'a']) {
        return NULL;
    }

    return (t->children[ch - 'a']);
}

void trieNodeIncreaseEnd(TrieNode *t) {
    t->cntEndWith++;
}
void trieNodeIncreasePrefix(TrieNode *t) {
    t->cntPrefix++;
}
void trieNodeDeleteEnd(TrieNode *t) {
    t->cntEndWith--;
}
void trieNodeReducePrefix(TrieNode *t) {
    t->cntPrefix--;
}
int trieNodeGetEnd(TrieNode *t) {
    return t->cntEndWith;
}
int trieNodeGetPrefix(TrieNode *t) {
    return t->cntPrefix;
}

typedef struct Trie_ {
	TrieNode* root;
} Trie;

TrieNode * newTrieNode()
{
    TrieNode *node = calloc(1, sizeof(TrieNode));
    return node;
}

void trieInit(Trie *t)
{
    t->root =  newTrieNode();
    return;
}

void trieInsertErase(Trie *t, char *word) {

    int len = strlen(word);
    TrieNode *node = t->root;

    for (int i = 0; i < len; i++) {

        if (true == trieNodeContainKey(node, word[i])) {
            node = trieNodeGetData(node, word[i]);
            trieNodeReducePrefix(node);
        } else {
            return;
        }
    }

    trieNodeDeleteEnd(node);
}

void trieInsertWord(Trie *t, char *word) {

    int len = strlen(word);
    TrieNode *node = t->root;

    for (int i = 0; i < len; i++) {

        if (false == trieNodeContainKey(node, word[i])) {
            trieNodePutData(node, word[i], newTrieNode());
        }

        node = trieNodeGetData(node, word[i]);
        trieNodeIncreasePrefix(node);
    }

    trieNodeIncreaseEnd(node);
}

bool trieSearch(Trie *t, char *word) {

    int len = strlen(word);
    TrieNode *node = t->root;

    for (int i = 0; i < len; i++) {

        if (false == trieNodeContainKey(node, word[i])) {
            return false;
        }

        node = trieNodeGetData(node, word[i]);
    }

    return ((trieNodeGetEnd(node) > 0) ? true : false);
}

bool trieStartsWith(Trie *t, char *prefix)
{
    int len = strlen(prefix);
    TrieNode *node = t->root;

    for (int i = 0; i < len; i++) {

        if (false == trieNodeContainKey(node, prefix[i])) {
            return false;
        }

        node = trieNodeGetData(node, prefix[i]);
    }

    return ((trieNodeGetPrefix(node) > 0) ? true : false);
}

int trieCountWordsEqualTo(Trie *t, char *word)
{
    int len = strlen(word);
    TrieNode *node = t->root;

    for (int i = 0; i < len; i++) {

        if (true == trieNodeContainKey(node, word[i])) {
            node = trieNodeGetData(node, word[i]);
        } else {
            return 0;
        }
    }

    return (trieNodeGetEnd(node));
}

int trieCountWordsStartingWith(Trie *t, char *prefix)
{
    int len = strlen(prefix);
    TrieNode *node = t->root;

    for (int i = 0; i < len; i++) {

        if (true == trieNodeContainKey(node, prefix[i])) {
            node = trieNodeGetData(node, prefix[i]);
        } else {
            return 0;
        }
    }

    return (trieNodeGetPrefix(node));
}

int trieIsCompleteString(Trie *t, char *word)
{
    int len = strlen(word);
    TrieNode *node = t->root;

    for (int i = 0; i < len; i++) {

        if (true == trieNodeContainKey(node, word[i])) {
            node = trieNodeGetData(node, word[i]);
            if (0 >= trieNodeGetEnd(node)) {
                return false;
            }
        } else {
            return false;
        }
    }

    return (0 < trieNodeGetEnd(node) ? true : false);
}


int countDistinctSubstrings(Trie *t, char *s)
{
    int count = 0;
    int len = strlen(s);
    TrieNode *node = NULL;

    for (int i = 0; i < len; i++) {

        node = t->root;

        for (int j = i; j < len; j++) {

            if (false == trieNodeContainKey(node, s[j])) {
                trieNodePutData(node, s[i], newTrieNode());
                count++;
            }

            node = trieNodeGetData(node, s[i]);
            trieNodeIncreasePrefix(node);
        }
    }

    trieNodeIncreaseEnd(node);
    return count + 1;
}

int main() {

    Trie trie;
    trieInit(&trie);
    Trie *t = &trie;

    char s1[] = "ababa";
    char s2[] = "ccfdf";

    printf("Total number of distinct substrings : %d \r\n",countDistinctSubstrings(t, s1));
    printf("Total number of distinct substrings : %d \r\n",countDistinctSubstrings(t, s2));

  return 0;
}