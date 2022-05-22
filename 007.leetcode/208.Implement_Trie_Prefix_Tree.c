#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define ALPHABET_SIZE 26

/**************** Structure Def ****************/
typedef struct TrieNode_ {
    struct TrieNode_ *children[ALPHABET_SIZE];
    char character;
    bool isEndOfWord;
} TrieNode;

typedef struct Trie_ {
    TrieNode *root;
} Trie;

/**************** Node ****************/
TrieNode *
trieNodeNew()
{
    TrieNode *node = calloc (1, sizeof (TrieNode));
    return (node);
}

bool
trieNodeContainKey (TrieNode *n, char ch)
{
    return (n->children[ch - 'a'] != NULL);
}

void
trieNodeput (TrieNode *n, char ch, TrieNode *node)
{
    n->children[ch - 'a'] = node;
}

TrieNode *
trieNodeget (TrieNode *n, char ch)
{
    return n->children[ch - 'a'];
}

void
trieNodesetEnd (TrieNode *n)
{
    n->isEndOfWord = true;
}

bool
trieNodeisEnd (TrieNode *n)
{
    return n->isEndOfWord;
}

/**************** Trie ****************/
void
trieInitilize (Trie *t)
{
    t->root = trieNodeNew();
}

/**************** APIs ****************/
void
trieInsert (Trie *t, char *word)
{
    TrieNode *node = t->root;
    int len = strlen (word);

    for (int i = 0; i < len; i++) {
        if (false == trieNodeContainKey (node, word[i])) {
            trieNodeput (node, word[i], trieNodeNew());
        }

        node = trieNodeget (node, word[i]);
    }

    trieNodesetEnd (node);
}

bool
trieSearch (Trie *t, char *word)
{
    TrieNode *node = t->root;
    int len = strlen (word);

    for (int i = 0; i < len; i++) {
        if (false == trieNodeContainKey (node, word[i])) {
            return false;
        }

        node = trieNodeget (node, word[i]);
    }

    return trieNodeisEnd (node);
}

bool
trieStartsWith (Trie *t, char *prefix)
{
    TrieNode *node = t->root;
    int len = strlen (prefix);

    for (int i = 0; i < len; i++) {
        if (false == trieNodeContainKey (node, prefix[i])) {
            return false;
        }

        node = trieNodeget (node, prefix[i]);
    }

    return true;
}

typedef enum  {
    INSERT = 1,
    SEARCH = 2,
    STARTS = 3,
} operation;

int
main()
{
    int n = 5;
    operation type[] = {INSERT, INSERT, SEARCH, STARTS, SEARCH};
    char value[][100] = {"hello", "help", "help", "hel", "hel"};
    Trie trie;
    trieInitilize (&trie);
    Trie *t = &trie;

    for (int i = 0; i < n; i++) {
        if (type[i] == INSERT) {
            trieInsert (t, value[i]);
        } else if (type[i] == SEARCH) {
            if (trieSearch (t, value[i])) {
                printf ("true \r\n");
            } else {
                printf ("false \r\n");
            }
        } else if (type[i] == STARTS) {
            if (trieStartsWith (t, value[i])) {
                printf ("true \r\n");
            } else {
                printf ("false \r\n");
            }
        }
    }
}