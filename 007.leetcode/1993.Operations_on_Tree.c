#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

//Definition for a binary tree node.
typedef struct TreeNode_ {
    int val;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;