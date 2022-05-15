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
    Implement LRU Cache
    Problem Statement: “Design a data structure that follows the constraints of
    Least Recently Used (LRU) cache”.

    Implement the LRUCache class:

    LRUCache(int capacity) we need to initialize the LRU cache with positive size capacity.
    int get(int key) returns the value of the key if the key exists, otherwise return -1.
    Void put(int key,int value), Update the value of the key if the key exists. Otherwise, add the key-value pair to the cache.if the number of keys exceeds the capacity from this operation, evict the least recently used key.
    The functions get and put must each run in O(1) average time complexity.

    Example:

    Input:
    ["LRUCache",  "put",     "put",   "get",     "put",   "get",   "put", "get",  "get", "get"]
    [[2],        [1, 1],    [2, 2],     [1],    [3, 3],     [2],  [4, 4],   [1],    [3],  [4]]

    Output:
    [null, null, null, 1, null, -1, null, -1, 3, 4]

    Explanation:

    LRUCache lRUCache = new LRUCache(2);
    lRUCache.put(1, 1); // cache is {1=1}
    lRUCache.put(2, 2); // cache is {1=1, 2=2}
    lRUCache.get(1);    // return 1
    lRUCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
    lRUCache.get(2);    // returns -1 (not found)
    lRUCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
    lRUCache.get(1);    // return -1 (not found)
    lRUCache.get(3);    // return 3
    lRUCache.get(4);    // return 4
*/

#define MAX 100
#define CACHE_LIMIT 2

typedef struct cache_ {
    struct cache_ *next;
    struct cache_ *prev;
    int key;
    int value;
}CACHE;

typedef struct hash_ {
    int key;
    int value;
    CACHE *node;
}HASH;

HASH* hashTable[MAX];

void initHashTable()
{
    memset(hashTable, 0, sizeof(hashTable));
    return;
}

HASH* lookUpHashTable(int key)
{
    if (key < 0) {
        return NULL;
    }

    return hashTable[key % MAX];
}

void addToHashTable(int key, int val, CACHE *node)
{
    if (key < 0) {
        return;
    }

    HASH *newHash = calloc(1, sizeof(HASH));
    newHash->key = key;
    newHash->value = val;
    newHash->node = node;

    hashTable[key % MAX] = newHash;

    return;
}

void deleteFromHashTable(int key)
{
    if (key < 0) {
        return;
    }

    HASH *newHash = hashTable[key % MAX];
    hashTable[key % MAX] = NULL;
    free(newHash);

    return;
}

typedef struct lru_ {
    CACHE *head;
    CACHE *tail;
    int maxLimit;
    int activeCount;
} LRU;

CACHE * newCacheNode(int key, int val)
{
    CACHE *c = calloc(1, sizeof(CACHE));
    c->next = NULL;
    c->prev = NULL;
    c->key = key;
    c->value = val;

    return c;
}

LRU * initLru(int max)
{
    LRU *lru = calloc(1, sizeof(LRU));

    lru->head = newCacheNode(INT_MIN, INT_MAX);
    lru->tail = newCacheNode(INT_MIN, INT_MAX);
    lru->head->next = lru->tail;
    lru->tail->prev = lru->head;
    lru->maxLimit = max;
    lru->activeCount = -1;

    initHashTable();

    return lru;
}

void deleteCache(LRU *lru, int key)
{
    HASH *temp = NULL;
    CACHE *current = NULL;

    temp = lookUpHashTable(key);
    if (NULL == temp) {
        return;
    }

    printf("Deleting Cache %d -> %d \r\n", temp->key, temp->value);
    current = temp->node;
    current->next->prev = current->prev;
    current->prev->next = current->next;
    deleteFromHashTable(key);
    free(current);

    return;
}

void addCache(LRU *lru, int key, int val)
{
    HASH *temp = NULL;
    CACHE *current = NULL;

    lru->activeCount++;

    printf("Adding new object %d->%d to cache LRU(active %d) \r\n", key, val, lru->activeCount);
    /* delete LRU cache */
    if ((lru->activeCount + 1) > (lru->maxLimit)) {
        temp = lookUpHashTable(key);
        printf("\tCache is full replacing LRU \r\n");
        if (NULL != temp) {
            printf("\tReplacing OLD entry %d -> %d \r\n", key, val);
            deleteCache(lru, key);
        } else {
            current = lru->tail->prev;
            printf("\tReplacing LRU entry %d -> %d \r\n", current->key, current->value);
            lru->tail->prev = current->prev;
            current->prev->next = lru->tail->prev;
            deleteFromHashTable(current->key);
            free(current);
        }
    }

    current = newCacheNode(key, val);

    lru->head->next->prev = current;
    current->next = lru->head->next;
    current->prev = lru->head;
    lru->head->next = current;

    addToHashTable(key, val, current);
    printf("\tAdding new Cache entry %d -> %d \r\n", current->key, current->value);
    return;
}

int lookUpCache(LRU *lru, int key)
{
    HASH *temp = lookUpHashTable(key);

    if (NULL != temp) {
        return (temp->value);
    } else {
        return -1;
    }
}

int getActiveCacheCount(LRU *lru)
{
    return lru->activeCount;
}


int main ()
{
    LRU *lru = initLru(CACHE_LIMIT);

    addCache(lru, 1, 1);
    addCache(lru, 2, 2);
    printf ("Lookup result : %d \r\n", lookUpCache(lru, 1));
    addCache(lru, 3, 3);
    printf ("Lookup result : %d \r\n", lookUpCache(lru, 2));
    addCache(lru, 4, 4);
    printf ("Lookup result : %d \r\n", lookUpCache(lru, 1));
    printf ("Lookup result : %d \r\n", lookUpCache(lru, 3));
    printf ("Lookup result : %d \r\n", lookUpCache(lru, 4));
}