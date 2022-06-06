struct TreeNode *searchBST (struct TreeNode *root, int val)
{
    struct TreeNode *p = root;

    while (p != NULL && p->val != val) {
        if (p->val > val) {
            p = p->left;
        } else {
            p = p->right;
        }
    }

    return p;
}

struct TreeNode *searchBST (struct TreeNode *root, int val)
{
    if (root == NULL) {
        return NULL;
    }

    if (root->val == val) {
        return root;
    }

    if (root->val < val) {
        return searchBST (root->right, val);
    } else {
        return searchBST (root->left, val);
    }
}