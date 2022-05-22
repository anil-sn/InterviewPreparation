#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define NO_CHARS 26

typedef struct trieNode_ {
    struct trieNode_ children[NO_CHARS];
    char character;
    bool endOfWord;
} trieNode;

typedef struct trie_ {
    trieNode *root;
} trie;

trie *
newTrie()
{
    return (calloc (1, sizeof (trie)));
}

trieNode *
newTrieNode()
{
    trieNode *node = calloc (1, sizeof (trieNode));
    return (node);
}

bool
trieNodeContainKey (trieNode *n, char ch)
{
    return (n->children[ch - 'a'] != NULL);
}

void
trieNodeput (trieNode *n, char ch, trieNode *node)
{
    n->children[ch - 'a'] = node;
}

trieNode *
trieNodeget (trieNode *n, char ch)
{
    return (n->children[ch - 'a']);
}

void
trieNodeSetEnd (trieNode *n)
{
    n->endOfWord = true;
}

bool
trieNodeisEnd (trieNode *n)
{
    return n->endOfWord;
}

void
trieInsert (trie *t, char *word)
{
    trieNode *node = t->root;
    int len = strlen (word);

    for (int i = 0; i < len; i++) {
        if (false == trieNodeContainKey (node, word[i])) {
            trieNodeput (node, word[i], newTrieNode());
        }

        node = trieNodeget (node, word[i]);
    }

    node->endOfWord = true;
}


bool
match (trieNode *node, char *word, int j)
{
    int len = strlen (word);

    for (int i = 0; i < len; i++) {
        char c = word[i];

        if (c == '.') {
            for (i = j; i < NO_CHARS; i++) {
                if (node->children[i] == NULL) {
                    continue;
                }

                return match (node->children[i], word, i + 1);
            }
        } else {
            if (false == trieNodeContainKey (node, c)) {
                return false;
            }

            node = trieNodeget (node, word[i]);
        }
    }

    return trieNodeisEnd (node);
}

bool
trieSearch (trie *t, char *word)
{
    trieNode *root = t->root;
    return match (root, word, 0);
}