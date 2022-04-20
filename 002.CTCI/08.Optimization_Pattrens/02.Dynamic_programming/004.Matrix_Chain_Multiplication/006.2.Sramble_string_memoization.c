#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

// A utility function that returns
// maximum of two integers
int max(int a, int b) { return (a > b) ? a : b; }
// minimum of two integers
int min(int a, int b) { return (a < b) ? a : b; }

/*
    Given a string s1, we may represent it as a binary tree by partitioning
    it to two non-empty substrings recursively.

    Below is one possible representation of s1 = "great":

        great
       /    \
      gr    eat
      / \    /  \
      g   r  e   at
              / \
              a   t
    To scramble the string, we may choose any non-leaf node and swap its
    two children.

    For example, if we choose the node "gr" and swap its two children,
    it produces a scrambled string "rgeat".

        rgeat
       /    \
       rg    eat
       / \    /  \
       r   g  e   at
               / \
               a   t
    We say that "rgeat" is a scrambled string of "great".

    Similarly, if we continue to swap the children of nodes "eat" and "at", it produces a scrambled string "rgtae".

        rgtae
    /    \
    rg    tae
    / \    /  \
    r   g  ta  e
        / \
        t   a
    We say that "rgtae" is a scrambled string of "great".

    Given two strings s1 and s2 of the same length, determine if s2 is a scrambled string of s1. Return 0/1 for this problem.

    Hint 1
    Hint : Divide and Conquer + Dynamic Programming

    On every step, you need to figure out if there exists one of the positions at root, using which the 2 parts of strings are scrambled strings of each other.

    Try writing a bruteforce solution and see which steps can be memoized.
*/

// https://github.com/benhoyt/ht/blob/master/ht.c
#define INITIAL_CAPACITY 16  // must not be zero

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static u_int64_t hash_key(const char* key) {
    u_int64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (u_int64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

// Hash table entry (slot may be filled or empty).
typedef struct {
    const char* key;  // key is NULL if this slot is empty
    int value;
} ht_entry;

// Hash table structure: create with ht_create, free with ht_destroy.
struct ht {
    ht_entry* entries;  // hash slots
    size_t capacity;    // size of _entries array
    size_t length;      // number of items in hash table
};

// Hash table structure: create with ht_create, free with ht_destroy.
typedef struct ht ht;

// Hash table iterator: create with ht_iterator, iterate with ht_next.
typedef struct {
    const char* key;  // current key
    int value;      // current value

    // Don't use these fields directly.
    ht* _table;       // reference to hash table being iterated
    size_t _index;    // current index into ht._entries
} hti;


ht* ht_create(void) {
    // Allocate space for hash table struct.
    ht* table = malloc(sizeof(ht));
    if (table == NULL) {
        return NULL;
    }
    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    // Allocate (zero'd) space for entry buckets.
    table->entries = calloc(table->capacity, sizeof(ht_entry));
    if (table->entries == NULL) {
        free(table); // error, free table before we return!
        return NULL;
    }
    return table;
}

void ht_destroy(ht* table) {
    // First free allocated keys.
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key != NULL) {
            free((void*)table->entries[i].key);
        }
    }

    // Then free entries array and table itself.
    free(table->entries);
    free(table);
}

int ht_get(ht* table, const char* key) {
    // AND hash with capacity-1 to ensure it's within entries array.
    u_int64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (u_int64_t)(table->capacity - 1));

    // Loop till we find an empty entry.
    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            // Found key, return value.
            return table->entries[index].value;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return -1;
}

// Internal function to set an entry (without expanding table).
static const char* ht_set_entry(ht_entry* entries, size_t capacity,
        const char* key, int value, size_t* plength) {
    // AND hash with capacity-1 to ensure it's within entries array.
    u_int64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (u_int64_t)(capacity - 1));

    // Loop till we find an empty entry.
    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            // Found key (it already exists), update value.
            entries[index].value = value;
            return entries[index].key;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }

    // Didn't find key, allocate+copy if needed, then insert it.
    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*plength)++;
    }
    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool ht_expand(ht* table) {
    // Allocate new entries array.
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity) {
        return false;  // overflow (capacity would be too big)
    }
    ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));
    if (new_entries == NULL) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new table's entries.
    for (size_t i = 0; i < table->capacity; i++) {
        ht_entry entry = table->entries[i];
        if (entry.key != NULL) {
            ht_set_entry(new_entries, new_capacity, entry.key,
                         entry.value, NULL);
        }
    }

    // Free old entries array and update this table's details.
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

const char* ht_set(ht* table, const char* key, int value) {

    // If length will exceed half of current capacity, expand it.
    if (table->length >= table->capacity / 2) {
        if (!ht_expand(table)) {
            return NULL;
        }
    }

    // Set entry and update length.
    return ht_set_entry(table->entries, table->capacity, key, value,
                        &table->length);
}

size_t ht_length(ht* table) {
    return table->length;
}

hti ht_iterator(ht* table) {
    hti it;
    it._table = table;
    it._index = 0;
    return it;
}

bool ht_next(hti* it) {
    // Loop till we've hit end of entries array.
    ht* table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index;
        it->_index++;
        if (table->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and value.
            ht_entry entry = table->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}

void sort (char str[])
{
    int chTemp;
    int i, j, len;
    len = strlen(str);
    for(i=0; i<len; i++)
    {
        for(j=0; j<(len-1); j++)
        {
            if(str[j]>str[j+1])
            {
                chTemp = str[j];
                str[j] = str[j+1];
                str[j+1] = chTemp;
            }
        }
    }
}

bool isAnagram( char s1[], char s2[]){
    char temp_s1[100] = {'\0'};
    char temp_s2[100] = {'\0'};

    strcpy(temp_s1, s1);
    strcpy(temp_s2, s2);
    sort(temp_s1);
    sort(temp_s2);
    if(strcmp(temp_s1, temp_s2))
        return false;
    else
        return true;
}

// Following function extracts characters present in `src`
// between `m` and `n` (excluding `n`)
char* substr(const char *src, int m, int n)
{
    char *dest = (char*)malloc(sizeof(char) * (n + 1));
    strncpy(dest, src+m, n);

    // return the destination string
    return dest;
}

ht *global_hash;
bool isScramble(char s1[], char s2[])
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    char hashKey[100] = {'\0'};
    int status;

    sprintf(hashKey,"%s_%s", s1, s2);

    printf ("\t%s, %s\r\n", s1, s2);

    // Strings of non-equal length
    // cant' be scramble strings
    if(l1 != l2) {
        return false;
    }

    // Empty strings are scramble strings
    if(0 == l1) {
        return true;
    }

    // Equal strings are scramble string
    if(0 == strcmp(s1, s2)) {
        return true;
    }

    // Check for the condition of anagram
    if(true != isAnagram(s1, s2)) {
        return false;
    }

    status = ht_get(global_hash, hashKey);
    if (-1 != status) {
        return status;
    }

    for (int i = 1; i < l1; i++) {

        bool A_FirstPart_B_SecondPart = isScramble(substr(s1, 0, i), substr(s2, l1-i, i));
        bool A_SecondPart_B_FirstPart = isScramble(substr(s1, i, l1-i), substr(s2, 0, l1-i));
        if (A_FirstPart_B_SecondPart && A_SecondPart_B_FirstPart) {
            status = true;
            ht_set(global_hash, hashKey, status);
            return status;
        }

        bool A_FirstPart_B_FirstPart   = isScramble(substr(s1, 0, i), substr(s2, 0, i));
        bool A_SecondPart_B_SecondPart = isScramble(substr(s1, i, l1-i), substr(s2, i, l1-i));
        if (A_FirstPart_B_FirstPart &&  A_SecondPart_B_SecondPart) {
            status = true;
            ht_set(global_hash, hashKey, status);
            return status;
        }
    }

    status = false;
    ht_set(global_hash, hashKey, status);
    return status;
}


// Driver code
int main()
{
    char s1[] = "coder";
    char s2[] = "ocred";

    global_hash = ht_create();
    bool result = isScramble(s1, s2);
    ht_destroy(global_hash);

    printf ("Is Sting %s is Scrambled form of string %s : %s\r\n", s1, s2, result ? "TRUE" : "FALSE");

    return 0;
}