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
    Implement Trie – 1
    Problem Statement: Implementing insertion, search, and startWith operations in a trie or prefix-tree.

    Implementation:
        Type 1: To insert a string “word” in Trie.
        Type 2: To check if the string “word” is present in Trie or not.
        Type 3: To check if there is any string in the Trie that starts with the given prefix string “word”.

    Example:
        Input: type = {1, 1, 2, 3, 2};
        value = {"hello", "help", "help", "hel", "hel"};
        Output:
            true
            true
            false
        Explanation:
            Trie object initialized
            “hello” inserted in the trie.
            “help” inserted in the trie
            “help” searched in the trie //returns true
            Checked if any previously inserted word has the prefix “hel” //return true
            “hel” searches in the trie //returns true
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

int main()
{
	Trie trie;
    trieInit(&trie);
    Trie *t = &trie;

    trieInsertWord(t, "apple");
    trieInsertWord(t, "apple");
    trieInsertWord(t, "apps");
    trieInsertWord(t, "apps");

    char word1[] = "apps";
    char word2[] = "abc";
    char word3[] = "ap";
    char word4[] = "appl";

    printf("Count Words Equal to %s = %d \r\n", word1, trieCountWordsEqualTo(t, word1));
    printf("Count Words Equal to %s = %d \r\n", word2, trieCountWordsEqualTo(t, word2));
    printf("Count Words Starting With %s = %d \r\n", word3, trieCountWordsStartingWith(t, word3));
    printf("Count Words Starting With %s = %d \r\n", word4, trieCountWordsStartingWith(t, word4));

    trieInsertErase(t, word1);
    printf("Count Words Equal to %s = %d \r\n", word1, trieCountWordsEqualTo(t, word1));

    return 0;

}