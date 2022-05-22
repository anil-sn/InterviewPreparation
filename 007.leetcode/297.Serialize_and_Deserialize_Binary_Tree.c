#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

typedef struct TreeNode_ {
    int val;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;

TreeNode *
getTreeNode (int val)
{
    TreeNode *t = calloc (1, sizeof (TreeNode));
    t->val = val;
    return t;
}

char stringStream[1000];
int pos = 0;
char *decrypt = stringStream;

void
serialize (TreeNode *root)
{
    serialize (root);
    return;
}

TreeNode *
deserialize (void)
{
    TreeNode *root = deserialize();
    return root;
}

void
serialize (TreeNode *root)
{
    if (NULL != root) {
        pos += sprintf (stringStream + pos, "%d,", root->val);
        serialize (root->left);
        serialize (root->right);
    } else {
        pos += sprintf (stringStream + pos, "#,", root->val);
    }
}

TreeNode *
deserialize()
{
    TreeNode *root = NULL;
    char *p = strtok (decrypt, ",");
    decrypt = decrypt + strlen (p);

    if (p && *p != '#') {
        root = getTreeNode (atoi (p));
        root->left = deserialize();
        root->right = deserialize();
    }

    return root;
}