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
    char character;
    bool isEndOfWord;
} TrieNode;

TrieNode * trieNodeNew()
{
    TrieNode *node;
    node = calloc(1, sizeof(TrieNode));
    return (node);
}

bool trieNodeContainKey(TrieNode *n, char ch)
{
    return (n->children[ch - 'a'] != NULL);
}

void trieNodeput(TrieNode *n, char ch, TrieNode *node)
{
    n->children[ch - 'a'] = node;
}

TrieNode * trieNodeget(TrieNode *n, char ch)
{
    return n->children[ch - 'a'];
}

void trieNodesetEnd(TrieNode *n)
{
    n->isEndOfWord = true;
}

bool trieNodeisEnd(TrieNode *n)
{
    return n->isEndOfWord;
}

typedef struct Trie_ {
	TrieNode* root;
} Trie;

void trieInitilize(Trie *t)
{
    t->root = trieNodeNew();
}

void trieInsert(Trie *t, char *word)
{
    TrieNode *node = t->root;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {

        if (!trieNodeContainKey(node, word[i])) {

            trieNodeput(node, word[i], trieNodeNew());
        }

        node = trieNodeget(node, word[i]);
    }

    trieNodesetEnd(node);
}

bool trieSearch(Trie *t, char *word)
{
    TrieNode *node = t->root;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {

        if (false == trieNodeContainKey(node, word[i])) {

            return false;
        }

        node = trieNodeget(node, word[i]);
    }

    return trieNodeisEnd(node);
}

bool trieStartsWith(Trie *t, char *prefix)
{
    TrieNode *node = t->root;
    int len = strlen(prefix);

    for (int i = 0; i < len; i++) {

        if (false == trieNodeContainKey(node, prefix[i])) {

            return false;
        }

        node = trieNodeget(node, prefix[i]);
    }

    return true;
}

typedef enum  {
    INSERT = 1,
    SEARCH = 2,
    STARTS = 3,
} operation;

int main()
{
	int n = 5;
	operation type[] = {INSERT, INSERT, SEARCH, STARTS, SEARCH};
	char value[][100] = {"hello", "help", "help", "hel", "hel"};

	Trie trie;
    trieInitilize(&trie);
    Trie *t = &trie;

    for (int i = 0; i < n; i++) {

        if (type[i] == INSERT) {

            trieInsert(t, value[i]);
        } else if (type[i] == SEARCH) {

            if (trieSearch(t, value[i])) {
                printf("true \r\n");
            } else {
                printf("false \r\n");
            }
        } else if (type[i] == STARTS) {

            if (trieStartsWith(t, value[i])) {
                printf("true \r\n");
            } else {
                printf("false \r\n");
            }
        }
    }
}