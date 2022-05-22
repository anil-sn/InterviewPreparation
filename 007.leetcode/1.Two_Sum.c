#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"


/*******************/
typedef struct data_s {
    int val;
    int idx;
} data_t;

int
cmp (const void *a, const void *b)
{
    return ((data_t *)a)->val - ((data_t *)b)->val;
}

int *
twoSum (int *nums, int numsSize, int target, int *returnSize)
{
    int *indices;
    int i, j, k;
    data_t *array;
    array = malloc ((numsSize + 1) * sizeof (data_t));
    *returnSize = 2;

    for (i = 0; i < numsSize; i ++) {
        array[i].val = nums[i];
        array[i].idx = i;
    }

    qsort (array, numsSize, sizeof (data_t), cmp);
    i = 0;
    j = numsSize - 1;

    while (i < j) {
        k = array[i].val + array[j].val;

        if (k == target) {
            indices = malloc (2 * sizeof (int));
            //assert(indices);
            indices[0] = array[i].idx;
            indices[1] = array[j].idx;
            free (array);
            return indices;
        } else if (k < target) {
            i ++;
        } else {
            j --;
        }
    }

    free (array);
    return NULL;
}
/*******************/


int *
twoSum (int *nums, int numsSize, int target, int *returnSize)
{
    *returnSize = 2;
    int *returna = (int *)malloc (2 * (sizeof (int)));

    for (int i = 0; i < numsSize; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[i] + nums[j] == target) {
                returna[0] = i;
                returna[1] = j;
            }
        }
    }

    return returna;
}

/*************************/
#define PRIME 263

struct node {
    int index_in_nums;
    struct node *next;
};

void initialize_hashtable (struct node *);
int hash (int);
int *twoSum (int *, int, int);
void free_hashtable (struct node *);

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *
twoSum (int *nums, int numsSize, int target)
{
    int i, hash_value, hash_value_comp;
    int *result = (int *)malloc (sizeof (int) * 2);

    if (result == NULL) {
        printf ("Not enough memory\n");
        return NULL;
    }

    struct node *hashtable = (struct node *)malloc (sizeof (struct node) * PRIME); //declare an hashtable whose size is PRIME*1

    if (hashtable == NULL) {
        printf ("Not enough memory\n");
        return NULL;
    }

    initialize_hashtable (hashtable);  //initialize the values in hashtable

    for (i = 0; i < numsSize; i++) {   //Set hashtable according to the nums array in this for loop
        hash_value = hash (nums[i]);

        if (hashtable[hash_value].index_in_nums == -1) { // if value in hashtable is not occupied
            hashtable[hash_value].index_in_nums = i;
        } else {    // if value in hashtable is occupied
            struct node *tail = (struct node *)malloc (sizeof (struct node));

            if (tail == NULL) {
                printf ("Not enough memory\n");
                free_hashtable (hashtable);
                return NULL;
            } else {
                tail->index_in_nums = i;
                tail->next = hashtable[hash_value].next;
                hashtable[hash_value].next = tail;
            }
        }
    }

    for (i = 0; i < numsSize; i++) { //search "target - nums" in this for loop
        hash_value_comp = hash (target - nums[i]);

        if (hashtable[hash_value_comp].index_in_nums == -1) { //not exist
            continue;
        }

        if (nums[hashtable[hash_value_comp].index_in_nums] == target - nums[i] && hashtable[hash_value_comp].index_in_nums != i) {
            result[0] = i;
            result[1] = hashtable[hash_value_comp].index_in_nums;
            free_hashtable (hashtable);
            return result;
        } else {
            struct node *temp = hashtable[hash_value_comp].next;

            while (temp != NULL) {
                if (nums[temp->index_in_nums] == target - nums[i]) {
                    result[0] = i;
                    result[1] = temp->index_in_nums;
                    free_hashtable (hashtable);
                    return result;
                } else {
                    temp = temp->next;
                }
            }
        }
    }

    free_hashtable (hashtable);
    return NULL;
}


int
hash (int key)
{
    return abs ((key % PRIME));
}

void
initialize_hashtable (struct node *hashtable)
{
    int i;

    for (i = 0; i < PRIME; i++) {
        hashtable[i].index_in_nums = -1;
        hashtable[i].next = NULL;
    }
}


void
free_hashtable (struct node *hashtable)
{
    int i;

    for (i = 0; i < PRIME; i++) {
        struct node *current = hashtable[i].next;

        while (current != NULL) {
            struct node *prev = current;
            current = current->next;
            free (prev);
        }

        free (current);
    }
}