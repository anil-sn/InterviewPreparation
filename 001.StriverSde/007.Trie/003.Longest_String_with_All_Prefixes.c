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
    Problem Statement
        Ninja developed a love for arrays and strings so this time his teacher gave him an
        array of strings, ‘A’ of size ‘N’. Each element of this array is a string.
        The teacher taught Ninja about prefixes in the past, so he wants to test his knowledge.

        A string is called a complete string if every prefix of this string is also
        present in the array ‘A’. Ninja is challenged to find the longest complete
        string in the array ‘A’. If there are multiple strings with the same length,
        return the lexicographically smallest one and if no string exists, return "None".

        Note :
            String ‘P’ is lexicographically smaller than string ‘Q’, if :
            1. There exists some index ‘i’ such that for all ‘j’ < ‘i’ ,
                ‘P[j] = Q[j]’ and ‘P[i] < Q[i]’. E.g. “ninja” < “noder”.
            2. If ‘P’ is a prefix of string ‘Q’, e.g. “code” < “coder”.
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


int main()
{
	Trie trie;
    trieInit(&trie);
    Trie *t = &trie;

    char string[][100] = {{"n"}, {"ni"}, {"nin"}, {"ninj"}, {"ninja"}, {"ninga"}};
    int count = 6;

    for (int i = 0; i < count; i++) {
        trieInsertWord(t, string[i]);
    }

    for (int i = 0; i < count; i++) {
        if (trieIsCompleteString(t, string[i])){
            printf("%s is a complete string \r\n", string[i]);
        } else {
            printf("%s is not a complete string \r\n", string[i]);
        }
    }

    return 0;
}
