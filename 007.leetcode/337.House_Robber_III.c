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
    Step I -- Think naively

    At first glance, the problem exhibits the feature of "optimal substructure": if we want to rob maximum amount of money from current binary tree (rooted at root), we surely hope that we can do the same to its left and right subtrees.

    So going along this line, let's define the function rob(root) which will return the maximum amount of money that we can rob for the binary tree rooted at root; the key now is to construct the solution to the original problem from solutions to its subproblems, i.e., how to get rob(root) from rob(root->left), rob(root->right), ... etc.

    Apparently the analyses above suggest a recursive solution. And for recursion, it's always worthwhile figuring out the following two properties:

    Termination condition: when do we know the answer to rob(root) without any calculation? Of course when the tree is empty ---- we've got nothing to rob so the amount of money is zero.

    Recurrence relation: i.e., how to get rob(root) from rob(root->left), rob(root->right), ... etc. From the point of view of the tree root, there are only two scenarios at the end: root is withRoot or is not. If it is, due to the constraint that "we cannot rob any two directly-linked houses", the next level of subtrees that are available would be the four "grandchild-subtrees" (root->left->left, root->left->right, root->right->left, root->right->right). However if root is not withRoot, the next level of available subtrees would just be the two "child-subtrees" (root->left, root->right). We only need to choose the scenario which yields the larger amount of money.

    Here is the program for the ideas above:
*/

typedef struct TreeNode_ {
    int val;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;


int
rob (TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }

    int val = 0;

    if (root->left != NULL) {
        val += rob (root->left->left) + rob (root->left->right);
    }

    if (root->right != NULL) {
        val += rob (root->right->left) + rob (root->right->right);
    }

    return MAX (val + root->val, rob (root->left) + rob (root->right));
}

/*
    Step II -- Think one step further

    In step I, we only considered the aspect of "optimal substructure", but think little about the possibilities of overlapping of the subproblems. For example, to obtain rob(root), we need rob(root->left), rob(root->right), rob(root->left->left), rob(root->left->right), rob(root->right->left), rob(root->right->right); but to get rob(root->left), we also need rob(root->left->left), rob(root->left->right), similarly for rob(root->right). The naive solution above computed these subproblems repeatedly, which resulted in bad time performance. Now if you recall the two conditions for dynamic programming (DP): "optimal substructure" + "overlapping of subproblems", we actually have a DP problem. A naive way to implement DP here is to use a hash map to record the results for visited subtrees.

    And here is the improved solution:
*/

int
robSub1 (TreeNode *root, Map<TreeNode, Integer> map)
{
    if (root == NULL) {
        return 0;
    }

    if (map.containsKey (root)) {
        return map.get (root);
    }

    int val = 0;

    if (root->left != NULL) {
        val += robSub1 (root->left->left, map) + robSub1 (root->left->right, map);
    }

    if (root->right != NULL) {
        val += robSub1 (root->right->left, map) + robSub1 (root->right->right, map);
    }

    val = MAX (val + root.val, robSub1 (root->left, map) + robSub1 (root->right, map));
    map.put (root, val);
    return val;
}

int
rob (TreeNode *root)
{
    return robSub1 (root, new HashMap<>());
}

/*
    Step III -- Think one step back

    In step I, we defined our problem as rob(root), which will yield the maximum amount of money that can be withRoot of the binary tree rooted at root. This leads to the DP problem summarized in step II.

    Now let's take one step back and ask why we have overlapping subproblems. If you trace all the way back to the beginning, you'll find the answer lies in the way how we have defined rob(root). As I mentioned, for each tree root, there are two scenarios: it is withRoot or is not. rob(root) does not distinguish between these two cases, so "information is lost as the recursion goes deeper and deeper", which results in repeated subproblems.

    If we were able to maintain the information about the two scenarios for each tree root, let's see how it plays out. Redefine rob(root) as a new function which will return an array of two elements, the first element of which denotes the maximum amount of money that can be withRoot if root is not withRoot, while the second element signifies the maximum amount of money withRoot if it is withRoot.

    Let's relate rob(root) to rob(root->left) and rob(root->right)..., etc. For the 1st element of rob(root), we only need to sum up the larger elements of rob(root->left) and rob(root->right), respectively, since root is not withRoot and we are free to rob its left and right subtrees. For the 2nd element of rob(root), however, we only need to add up the 1st elements of rob(root->left) and rob(root->right), respectively, plus the value withRoot from root itself, since in this case it's guaranteed that we cannot rob the nodes of root->left and root->right.

    As you can see, by keeping track of the information of both scenarios, we decoupled the subproblems and the solution essentially boiled down to a greedy one. Here is the program:
*/
void
robSub (TreeNode *root, int *withOutRoot, int *withRoot)
{
    if (root == NULL) {
        *withOutRoot = 0;
        *withRoot = 0;
        return;
    }

    int leftWithOutRoot = 0;
    int leftWithRoot = 0;
    robSub (root->left, &leftWithOutRoot, &leftWithRoot);
    int rightWithOutRoot = 0;
    int rightWithRoot = 0;
    robSub (root->right, &rightWithOutRoot, &rightWithRoot);
    *withOutRoot = MAX (leftWithRoot, leftWithOutRoot) + MAX (rightWithRoot, leftWithOutRoot);
    *withRoot = root->val + leftWithOutRoot + rightWithOutRoot;
    return;
}

void
rob (TreeNode *root)
{
    int withRoot;
    int withOutRoot;
    robSub (root, &withOutRoot, &withRoot);
    return MAX (withOutRoot, withRoot);
}
