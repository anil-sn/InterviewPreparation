#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define type TrieNode*
#define AlphaCount 26

typedef struct TrieNode {
    bool end;
    struct TrieNode *parent;
    struct TrieNode *children[AlphaCount];
} TrieNode;

typedef struct Trie {
    TrieNode *root;
} Trie;

typedef struct vector {
    type *a;
    int size;
    int max;
} vector;

typedef struct listNode {
    type data;
    struct listNode *next;
    struct listNode *prev;
} listNode;

typedef struct list {
    struct listNode *begin;
    struct listNode *end;
    int size;
} list;

typedef struct stack {
    vector *v;
} stack;

typedef struct queue {
    list *ls;
} queue;

list *listCreate() {
    list *ls = malloc(sizeof(list));
    ls->begin = NULL;
    ls->end = NULL;
    ls->size = 0;
    return ls;
}

void clearList(listNode *node) {
    if (!node) {
        return;
    }

    clearList(node->next);
    free(node);
}

void deleteList(list *ls) {
    clearList(ls->begin);
    ls->size = 0;
    free(ls);
}

int listSize(list *ls) {
    return ls->size;
}

type listFront(list *ls) {
    if (!ls || !(ls->begin)) {
        return 0;
    }

    return ls->begin->data;
}

type listBack(list *ls) {
    if (!ls || !(ls->end)) {
        return 0;
    }

    return ls->end->data;
}

void listPushFront(list *ls, type val) {
    if (!ls) {
        return;
    }

    listNode *node = malloc(sizeof(listNode));
    node->data = val;
    node->next = ls->begin;
    node->prev = 0;

    if (ls->size) {
        ls->begin->prev = node;
    }

    ls->begin = node;

    if (!ls->size) {
        ls->end = node;
    }

    ls->size++;
}

void listPopFront(list *ls) {
    if (!ls) {
        return;
    }

    if (ls->size == 0) {
        return;
    }

    if (ls->size == 1) {
        free(ls->begin);
        ls->begin = 0;
        ls->end = 0;
    } else {
        listNode *node = ls->begin->next;
        free(ls->begin);
        ls->begin = node;
        ls->begin->prev = 0;
    }

    ls->size--;
}

void listPushBack(list *ls, type val) {
    if (!ls) {
        return;
    }

    listNode *node = malloc(sizeof(listNode));
    node->data = val;
    node->next = 0;
    node->prev = ls->end;

    if (ls->size) {
        ls->end->next = node;
    }

    ls->end = node;

    if (!ls->size) {
        ls->begin = node;
    }

    ls->size++;
}

void listPopBack(list *ls) {
    if (!ls) {
        return;
    }

    if (ls->size == 0) {
        return;
    }

    if (ls->size == 1) {
        free(ls->begin);
        ls->begin = 0;
        ls->end = 0;
    } else {
        listNode *node = ls->end->prev;
        free(ls->end);
        ls->end = node;
        ls->end->next = 0;
    }

    ls->size--;
}

void listInsert(list *ls, int pos, type val) {
    if (!ls) {
        return;
    }

    if (pos < 0 || pos > ls->size) {
        return;
    }

    if (pos == 0) {
        listPushFront(ls, val);
    } else if (pos == ls->size) {
        listPushBack(ls, val);
    } else {
        int i, mid;
        listNode *node, *prevNode, *nextNode;
        node = malloc(sizeof(listNode));
        node->data = val;
        mid = ls->size / 2;

        if (pos < mid) {
            prevNode = ls->begin;

            for (i = 0; i < pos - 1; i++) {
                prevNode = prevNode->next;
            }

            nextNode = prevNode->next;
        } else {
            nextNode = ls->end;

            for (i = ls->size; i > pos + 1; i--) {
                nextNode = nextNode->prev;
            }

            prevNode = nextNode->prev;
        }

        prevNode->next = node;
        nextNode->prev = node;
        node->prev = prevNode;
        node->next = nextNode;
        ls->size++;
    }
}

void listErase(list *ls, int pos) {
    if (!ls) {
        return;
    }

    if (pos < 0 || pos >= ls->size) {
        return;
    }

    if (pos == 0) {
        listPopFront(ls);
    } else if (pos == ls->size - 1) {
        listPopBack(ls);
    } else {
        int i, mid;
        listNode *node, *prevNode, *nextNode;
        mid = ls->size / 2;

        if (pos < mid) {
            node = ls->begin;

            for (i = 0; i < pos; i++) {
                node = node->next;
            }
        } else {
            node = ls->end;

            for (i = ls->size - 1; i > pos; i++) {
                node = node->prev;
            }
        }

        prevNode = node->prev;
        nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        free(node);
        ls->size--;
    }
}

queue *queueCreate() {
    queue *q = malloc(sizeof(queue));
    q->ls = listCreate();
    return q;
}

void queueDelete(queue *q) {
    if (!q) {
        return;
    }

    deleteList(q->ls);
    free(q);
}

type queueTop(queue *q) {
    if (!q) {
        return 0;
    }

    return listFront(q->ls);
}

void queuePush(queue *q, type val) {
    if (!q) {
        return;
    }

    listPushBack(q->ls, val);
}

void queuePop(queue *q) {
    if (!q) {
        return;
    }

    listPopFront(q->ls);
}

vector *vectorCreate() {
    vector *v = malloc(sizeof(vector));
    v->size = 0;
    v->max = 4;
    v->a = malloc(v->max * sizeof(type));
    return v;
}

void vectorDelete(vector *v) {
    if (!v) {
        return;
    }

    free(v->a);
    v->a = 0;
    free(v);
    v = 0;
}

int vectorSize(vector *v) {
    return v->size;
}

int vectorMaxSize(vector *v) {
    return v->max;
}

void vectorResize(vector *v, int n) {
    if (!v) {
        return;
    }

    v->a = realloc(v->a, n * sizeof(type));
    v->max = n;
}

type vectorAtIndex(vector *v, int i) {
    if (!v) {
        return 0;
    }

    if (i < 0 || i >= v->size) {
        printf("Index out of bounds\n");
        return 0;
    }

    return v->a[i];
}

type *vectorData(vector *v) {
    if (!v) {
        return 0;
    }

    return v->a;
}

void vectorInsert(vector *v, int pos, type val) {
    if (!v) {
        return;
    }

    if (pos < 0 || pos > v->size) {
        return;
    }

    int i;

    if (v->size == v->max) {
        v->max = v->max * 2;
        vectorResize(v, v->max);
    }

    for (i = v->size; i > pos; i--) {
        v->a[i] = v->a[i - 1];
    }

    v->a[pos] = val;
    v->size++;
}

void vectorErase(vector *v, int pos) {
    if (!v) {
        return;
    }

    if (pos >= v->size) {
        return;
    }

    int i;

    for (i = pos; i < v->size - 1; i++) {
        v->a[i] = v->a[i + 1];
    }

    v->a[i] = 0;
    v->size--;
}

void vectorPushBack(vector *v, type val) {
    if (!v) {
        return;
    }

    vectorInsert(v, v->size, val);
}

void vectorPopBack(vector *v) {
    if (!v) {
        return;
    }

    vectorErase(v, v->size - 1);
}

stack *stackCreate() {
    stack *s;
    s = malloc(sizeof(s));
    s->v = vectorCreate();
    return s;
}

void stackDelete(stack *s) {
    if (!s) {
        return;
    }

    vectorDelete(s->v);
    free(s);
    s = 0;
}

type stackTop(stack *s) {
    if (!s) {
        return 0;
    }

    return vectorAtIndex(s->v, vectorSize(s->v) - 1);
}

void stackPush(stack *s, type val) {
    if (!s) {
        return;
    }

    vectorPushBack(s->v, val);
}

void stackPop(stack *s) {
    if (!s) {
        return;
    }

    vectorPopBack(s->v);
}

TrieNode *trieNodeCreate() {
    int i;
    TrieNode *node = malloc(sizeof(TrieNode));
    node->end = false;
    node->parent = 0;

    for (i = 0; i < AlphaCount; i++) {
        node->children[i] = 0;
    }

    return node;
}

Trie *trieCreate() {
    Trie *trie = malloc(sizeof(Trie));
    trie->root = trieNodeCreate();
    return trie;
}

int asciiChar2Integer(char c) {
    if (c < 97 || c > 122) {
        return -1;
    }

    return c - 97;
}

char integer2AsciiChar(int c) {

    if ((c + 97) > 122) {
        return '\0';
    }

    return (c + 97);
}

TrieNode *trieNodeInsertChar(TrieNode *node, int i, bool end) {
    if ((NULL == node) || (-1 == 1)) {
        return 0;
    }

    TrieNode *child;

    if (NULL == node->children[i]) {
        child = trieNodeCreate();
        node->children[i] = child;
        child->parent = node;
    } else {
        child = node->children[i];
    }

    if (true == end) {
        child->end = true;
    }

    return child;
}

void trieNodeDisplay(TrieNode *node) {
    if (NULL == node) {
        return;
    }

    for (int i = 0; i < AlphaCount; i++) {
        trieNodeDisplay(node->children[i]);
        if (NULL != node->children[i]) {
            printf ("%c  ",integer2AsciiChar(i));
        }
    }

    return;
}

void trieDisplay(Trie *trie) {
    if (NULL == trie) {
        return;
    }

    TrieNode *node = trie->root;
    trieNodeDisplay(node);
    printf("\r\n");

    return;
}

void trieInsert(Trie *trie, char *s) {
    if ((NULL == trie) || (NULL == s) || ('\0' == s[0])) {
        return;
    }

    int i, j, len;
    TrieNode *node;
    len = strlen(s);
    char sub[len - 1];
    node = trie->root;

    for (i = 0; i < len - 1; i++) {
        j = asciiChar2Integer(s[i]);
        node = trieNodeInsertChar(node, j, false);
        //trieDisplay(trie);
    }

    j = asciiChar2Integer(s[i]);
    trieNodeInsertChar(node, j, true);
    //trieDisplay(trie);

    for (i = 0; i < len; i++) {
        sub[i] = s[i + 1];
    }

    trieInsert(trie, sub);
}

bool isSubstring(Trie *trie, char *s) {
    if ((NULL == trie) || (NULL == s) || ('\0' == s[0])) {
        return false;
    }

    int i, j, len;
    TrieNode *node;
    len = strlen(s);
    node = trie->root;

    for (i = 0; i < len; i++) {
        j = asciiChar2Integer(s[i]);

        if (j == -1) {
            return false;
        }

        if (!node->children[j]) {
            return false;
        }

        node = node->children[j];
    }

    return true;
}

int numberOfOccurrence(Trie *trie, char *s) {
    if (!trie || !s) {
        return -1;
    }

    if (!s[0]) {
        return 0;
    }

    int i, j, l, n;
    stack *nodeStack;
    TrieNode *node;
    l = strlen(s);
    n = 0;
    nodeStack = stackCreate();
    node = trie->root;

    for (i = 0; i < l; i++) {
        j = asciiChar2Integer(s[i]);

        if (j == -1) {
            return 0;
        }

        if (!node->children[j]) {
            return 0;
        }

        node = node->children[j];
    }

    stackPush(nodeStack, node);

    while (vectorSize(nodeStack->v)) {
        node = stackTop(nodeStack);
        stackPop(nodeStack);

        if (node->end) {
            n++;
        }

        for (i = 0; i < AlphaCount; i++)
            if (node->children[i]) {
                stackPush(nodeStack, node->children[i]);
            }
    }

    return n;
}

char *nodeToString(TrieNode *node) {
    if (!node) {
        return 0;
    }

    int i, l;
    char backtrack[100000];
    char *result;
    i = 0;
    l = 0;

    while (node->parent) {
        for (i = 0; i < AlphaCount; i++)
            if (node == node->parent->children[i]) {
                backtrack[l] = integer2AsciiChar(i);
                l++;
                break;
            }

        node = node->parent;
    }

    result = malloc(l + 1);

    for (i = 0; i < l; i++) {
        result[i] = backtrack[l - i - 1];
    }

    result[l] = '\0';
    return result;
}

char *longestRepeated(Trie *trie) {
    if (!trie) {
        return 0;
    }

    int i;
    char *s, *result;
    queue *q;
    TrieNode *node;
    s = 0;
    result = 0;
    q = queueCreate();
    queuePush(q, trie->root);

    while (listSize(q->ls)) {
        node = queueTop(q);
        queuePop(q);

        for (i = 0; i < AlphaCount; i++)
            if (node->children[i]) {
                s = nodeToString(node->children[i]);

                if (numberOfOccurrence(trie, s) > 1) {
                    queuePush(q, node->children[i]);
                    result = s;
                }
            }
    }

    return result;
}

// driver program to test above functions
int main() {
    int i;
    Trie *trie = trieCreate();

    char inPut[] = "gttatagctgatcgcggcgtagcgg";

    printf("\n");
    printf("Input String : %s \r\n", inPut);
    printf("\n");

    trieInsert(trie, inPut);
#if 0
    printf("Substring %s fount at index %d\n", "ggt", isSubstring(trie, "ggt"));
    printf("Substring %s fount at index %d\n", "agg", isSubstring(trie, "agg"));
    printf("Substring %s fount at index %d\n", "tag", isSubstring(trie, "tag"));
    printf("\n");

    printf("Number of Occurance of string %s is %d\n", "tag", numberOfOccurrence(trie, "tga"));
    printf("Number of Occurance of string %s is %d\n", "cg", numberOfOccurrence(trie, "cg"));

    printf("\n");
    printf("Longest Repeated Sequence is %s \n", longestRepeated(trie));
#endif
    return 0;
}