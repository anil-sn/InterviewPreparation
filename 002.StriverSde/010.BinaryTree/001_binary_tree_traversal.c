#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

#define MAX 1000

int traversal[100];
int pos;

typedef enum {
    OVERWRITE_DUPLICATE = 1,
    SKIP_DUPLICATE = 2,
    ALLOW_DUPLICATE = 3,
}operation;

typedef struct node {
    int data;
    int horizontalLevel;
    int verticalLevel;
    int order;
    struct node * left;
    struct node * right;
}NODE;

typedef struct map {
    int key;
    NODE * node;
    struct map * prev;
    struct map * next;
} MAP;

typedef struct Queue_ {
    NODE * queue[MAX];
    int start;
    int end;
    int currSize;
    int maxSize;
} QUEUE;

typedef struct stack_ {
    NODE * stack[MAX];
    int top;
    int capacity;
} STACK;

NODE * newNode(int data)
{
    NODE * node = (NODE * ) malloc(sizeof(NODE));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->horizontalLevel = 0;
    node->verticalLevel = 0;
    node->order = 0;

    return (node);
}

MAP * newMapNode (int key, NODE *node)
{
    MAP *new = malloc(sizeof(MAP));
    new->key = key;
    new->node = node;
    new->prev = NULL;
    new->next = NULL;

    return new;
}

MAP * initMap()
{
    MAP *head = newMapNode(INT_MIN, NULL);
    return head;
}

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX;
    return stack;
}

QUEUE* initQueue()
{
    QUEUE *queue = calloc(1, sizeof(QUEUE));
    queue->start = -1;
    queue->end = -1;
    queue->currSize = 0;
    queue->maxSize = MAX;
    return queue;
}

NODE * lookupMAP(MAP *m, int key)
{
    while ((NULL != m) && (m->key < key)) {
        m = m->next;
    }

    if (NULL == m) {
        return NULL;
    }

    if ((m->key > key) || (m->key < key)){
        return NULL;
    }

    return m->node;
}

void printMap(MAP *m)
{
    pos = 0;
    while (NULL != m) {

        if (m->node) {
            printf("[key %3d] %3d\t", m->key, m->node->data);
        }
        m = m->next;
    }
    printf("\r\n");
    return;
}

MAP * addToMap(MAP *m, int key, NODE *node, operation op)
{
    MAP *head = m;

    if (NULL == m) {

        /* New Entry */
        MAP *new = newMapNode(key, node);
        return new;
    }

    while ((NULL != m) && (NULL != m->next) && (m->key < key)) {
        m = m->next;
    }

    /* Updating the value for the old entry */
    if (m->key == key){

        if (op == OVERWRITE_DUPLICATE) {
            m->node = node;
        }

        if (op == SKIP_DUPLICATE) {
            return head;
        }
    }

    /* New Entry */
    MAP *new = newMapNode(key, node);

    /* Insert Behind */
    if (m->key > key) {

        new->next = m;
        new->prev = m->prev;
        if (m->prev) {
            m->prev->next = new;
        }

        m->prev = new;

    } else { /* Inser after */

        new->next = m->next;
        new->prev = m;
        if (m->next) {
            m->next->prev = new;
        }

        m->next = new;
    }

    while (NULL != m->prev) {
        m = m->prev;
    }
    head = m;

    return head;
}

void walkMapUpdateResult(MAP *m)
{
    MAP * head = m;
    pos = 0;
    while (NULL != m) {

        if (m->node) {
            printf("%5d ", m->node->data);
            traversal[pos++] = m->node->data;
        }
        m = m->next;
    }

    m = head;
    while( m != NULL ) {

        MAP* next = m->next;
        free( m );
        m = next;

    }

    return;
}

int queueSize(QUEUE *q)
{
    return (q->currSize);
}

bool isQueueFull(QUEUE *q)
{
    return (q->currSize >= (q->maxSize - 1)) ? true : false;
}

bool isQueueEmpty(QUEUE *q)
{
    return (q->start < 0) ? true : false;
}

void enQueue(QUEUE *q, NODE * val)
{
    if (isQueueFull(q)) {
        printf("Queue is full\r\nExiting...\r\n");
    }

    if (q->end == -1) {
        q->end = 0;
        q->start = 0;
    } else {
        q->end = (q->end + 1) % q->maxSize;
    }

    q->queue[q->end] = val;
    //printf("Element %d is enqued \r\n", val->data);
    q->currSize++;
}

NODE * deQueue(QUEUE *q)
{
    NODE * val;
    if (isQueueEmpty(q)) {
        printf("Queue is Empty \r\nExiting...\r\n");
    }

    val = q->queue[q->start];
    if (q->currSize == 1) {
        q->end = -1;
        q->start = -1;
    } else {
        q->start = (q->start + 1) % q->maxSize;
    }

    q->currSize--;
    return val;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

NODE * peek(STACK *s)
{
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return NULL;
    }

    return (s->stack[s->top]);
}

void stackPush(STACK *s, NODE * value)
{
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    //printf("Element %d is pushed to stack \r\n", value->data);
    s->stack[++s->top] = value;
    return;
}

NODE * pop(STACK *s)
{
    if (isStackEmpty(s)) {
        return NULL;
    }

    return (s->stack[s->top--]);
}

int height(NODE* node)
{
    if (node == NULL) {
        return 0;
    }

    /* compute the height of each subtree */
    int lheight = height(node->left);
    int rheight = height(node->right);

    /* use the larger one */
    if (lheight > rheight) {

        return (lheight + 1);
    } else {

        return (rheight + 1);
    }

    return 0;
}

/* Print nodes at a current level */
void currentLevel(NODE* root, int level)
{
    if (root == NULL) {
        return;
    }

    if (level == 1) {

        printf("%5d ", root->data);
        traversal[pos++] = root->data;

    } else if (level > 1) {

        currentLevel(root->left, level - 1);
        currentLevel(root->right, level - 1);
    }
    return;
}

void levelOrderTraversal(NODE* root)
{
    int h = height(root);
    for (int i = 1; i <= h; i++) {
        currentLevel(root, i);
    }
}

void preOrderTraversalMorris(NODE * cur)
{
    NODE * prev = NULL;

    while (cur != NULL) {

        if (cur->left == NULL) {

            printf("%5d ", cur->data);
            traversal[pos++] = cur->data;

            cur = cur->right;
        } else {

            prev = cur->left;
            while (prev->right != NULL && prev->right != cur) {
                prev = prev->right;
            }

            if (prev->right == NULL) {

                prev->right = cur;

                printf("%5d ", cur->data);
                traversal[pos++] = cur->data;

                cur = cur->left;
            } else {

                prev->right = NULL;
                cur = cur->right;
            }
        }
    }

    return;
}

void preOrderTraversal (NODE * curr)
{
    if (NULL == curr) {
        return;
    }

    pos = 0;
    STACK *s = initStack();
    NODE * topNode = NULL;

    stackPush(s, curr);

    while (false == isStackEmpty(s)) {

        topNode = pop(s);

        printf("%5d ", topNode->data);
        traversal[pos++] = topNode->data;

        if (topNode->right != NULL) {
            stackPush(s, topNode->right);
        }

        if (topNode->left != NULL) {
            stackPush(s, topNode->left);
        }
    }

    return;
}

void inOrderTraversalMorris(NODE * cur)
{
    NODE * prev = NULL;

    while (cur != NULL) {

        if (cur->left == NULL) {

            printf("%5d ", cur->data);
            traversal[pos++] = cur->data;

            cur = cur->right;
        } else {

            prev = cur->left;
            while (prev->right != NULL && prev->right != cur) {
                prev = prev->right;
            }

            if (prev->right == NULL) {

                prev->right = cur;
                cur = cur->left;
            } else {

                printf("%5d ", cur->data);
                traversal[pos++] = cur->data;

                prev->right = NULL;
                cur = cur->right;
            }
        }
    }

    return;
}

void inOrderTraversal (NODE * curr)
{
    STACK *s = initStack();
    pos = 0;

    while (true) {
        if (curr != NULL) {
            stackPush(s, curr);
            curr = curr->left;
        } else {
            if (true == isStackEmpty(s)) {
                break;
            }
            curr = pop(s);

            printf("%5d ", curr->data);
            traversal[pos++] = curr->data;

            curr = curr->right;
        }
    }

    return;
}

void postOrderTraversal (NODE * curr)
{
    if (NULL == curr) {
        return;
    }

    STACK *s1 = initStack();
    STACK *s2 = initStack();
    pos = 0;
    NODE * topNode = NULL;

    stackPush(s1, curr);
    while (false == isStackEmpty(s1)) {
        topNode = pop(s1);
        stackPush(s2, topNode);

        if (topNode->left != NULL) {
            stackPush(s1, topNode->left);
        }

        if (topNode->right != NULL) {
            stackPush(s1, topNode->right);
        }
    }

    while (false == isStackEmpty(s2)) {

        topNode = pop(s2);
        printf("%5d ", topNode->data);
        traversal[pos++] = topNode->data;
    }

    return;
}


void inOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    inOrderTrav(curr->left);

    printf("%5d ", curr->data);
    traversal[pos++] = curr->data;

    inOrderTrav(curr->right);
}

void preOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    printf("%5d ", curr->data);
    traversal[pos++] = curr->data;

    preOrderTrav(curr->left);
    preOrderTrav(curr->right);
}

void postOrderTrav(NODE * curr)
{
    if (curr == NULL) {
        return;
    }

    postOrderTrav(curr->left);
    postOrderTrav(curr->right);

    printf("%5d ", curr->data);
    traversal[pos++] = curr->data;
}

void leftSideViewRecursion(NODE * root, int level)
{
    if(root == NULL) {
        return;
    }

    if (pos == level) {
        printf("%5d ", root->data);
        traversal[pos++] = root->data;
    }

    leftSideViewRecursion(root->left, level+1);
    leftSideViewRecursion(root->right, level+1);
    return;
}

void leftSideView(NODE * root)
{
    printf("%5d ", root->data);
    traversal[pos++] = root->data;
    leftSideViewRecursion(root, 0);
}

void rightSideViewRecursion(NODE * root, int level)
{
    if(root == NULL) {
        return;
    }

    if (pos == level) {
        printf("%5d ", root->data);
        traversal[pos++] = root->data;
    }

    rightSideViewRecursion(root->right, level+1);
    rightSideViewRecursion(root->left, level+1);
    return;
}

void rightSideView(NODE * root)
{
    printf("%5d ", root->data);
    traversal[pos++] = root->data;
    rightSideViewRecursion(root, 0);
}

void bottomView(NODE *root)
{
    NODE* node = NULL;
    int line = 0;

    if(root == NULL) {
        return;
    }

    MAP *mpp = NULL;
    QUEUE *q = initQueue();

    root->verticalLevel = 0;
    enQueue(q, root);

    while(false == isQueueEmpty(q)) {

        node = deQueue(q);
        line = node->verticalLevel;

        mpp = addToMap(mpp, line, node, OVERWRITE_DUPLICATE);

        if(node->left != NULL) {
            node->left->verticalLevel = (line - 1);
            enQueue(q, node->left);
        }
        if(node->right != NULL) {
            node->right->verticalLevel = (line + 1);
            enQueue(q, node->right);
        }
    }

    walkMapUpdateResult(mpp);
    return;
}

void topView(NODE *root)
{
    NODE* node = NULL;
    int line = 0;

    if (root == NULL) {
        return;
    }

    MAP *mpp = NULL;
    QUEUE *q = initQueue();

    root->verticalLevel = 0;
    enQueue(q, root);

    while (false == isQueueEmpty(q)) {

        node = deQueue(q);
        line = node->verticalLevel;

        mpp = addToMap(mpp, line, node, SKIP_DUPLICATE);

        if(node->left != NULL) {
            node->left->verticalLevel = (line - 1);
            enQueue(q, node->left);
        }
        if(node->right != NULL) {
            node->right->verticalLevel = (line + 1);
            enQueue(q, node->right);
        }

    }

    walkMapUpdateResult(mpp);
    return;
}

void levelOrder(NODE* root)
{
    NODE* node = NULL;

    if(root == NULL) {
        return;
    }

    QUEUE *q = initQueue();

    root->horizontalLevel = 1;
    root->verticalLevel = 0;
    enQueue(q, root);

    while(false == isQueueEmpty(q)) {

        node = deQueue(q);

        printf("%5d [%2d,%2d] ", node->data, node->horizontalLevel, node->verticalLevel);
        traversal[pos++] = node->data;

        if(node->left != NULL) {
            node->left->horizontalLevel = node->horizontalLevel + 1;
            node->left->verticalLevel = (node->verticalLevel - 1);
            enQueue(q, node->left);
        }
        if(node->right != NULL) {
            node->right->horizontalLevel = node->horizontalLevel + 1;
            node->right->verticalLevel = (node->verticalLevel + 1);
            enQueue(q, node->right);
        }
    }

    return;
}

void verticalTraversal(NODE* root)
{
    NODE* node = NULL;
    MAP *mpp = NULL;

    if(root == NULL) {
        return;
    }

    QUEUE *q = initQueue();

    root->horizontalLevel = 1;
    root->verticalLevel = 0;
    enQueue(q, root);

    while(false == isQueueEmpty(q)) {

        node = deQueue(q);
        mpp = addToMap(mpp, node->verticalLevel, node, ALLOW_DUPLICATE);

        if(node->left != NULL) {
            node->left->horizontalLevel = node->horizontalLevel + 1;
            node->left->verticalLevel = (node->verticalLevel - 1);
            enQueue(q, node->left);
        }

        if(node->right != NULL) {
            node->right->horizontalLevel = node->horizontalLevel + 1;
            node->right->verticalLevel = (node->verticalLevel + 1);
            enQueue(q, node->right);
        }
    }

    walkMapUpdateResult(mpp);
    return;
}

void SpiralLevelOrder(NODE* root)
{
    NODE* node = NULL;

    if(root == NULL) {
        return;
    }

    QUEUE *q = initQueue();

    root->horizontalLevel = 1;
    root->verticalLevel = 0;
    enQueue(q, root);

    while (false == isQueueEmpty(q)) {

        int size = queueSize(q);
        for (int i = 0; i < size; i++) {

            node = deQueue(q);

            traversal[pos++] = node->data;

            if(node->left != NULL) {
                node->left->horizontalLevel = node->horizontalLevel + 1;
                node->left->verticalLevel = (node->verticalLevel - 1);
                enQueue(q, node->left);
            }

            if(node->right != NULL) {
                node->right->horizontalLevel = node->horizontalLevel + 1;
                node->right->verticalLevel = (node->verticalLevel + 1);
                enQueue(q, node->right);
            }

        }
    }

    return;
}

void zigzagLevelOrder(NODE* root)
{
    NODE* node = NULL;

    if(root == NULL) {
        return;
    }

    QUEUE *q = initQueue();

    root->horizontalLevel = 1;
    root->verticalLevel = 0;
    enQueue(q, root);

    bool leftToRight = true;

    while (false == isQueueEmpty(q)) {

        int size = queueSize(q);
        for (int i = 0; i < size; i++) {

            node = deQueue(q);

            // find position to fill node's value
            int index = (leftToRight) ? i : (size - 1 - i);

            traversal[pos + index] = node->data;

            if(node->left != NULL) {
                node->left->horizontalLevel = node->horizontalLevel + 1;
                node->left->verticalLevel = (node->verticalLevel - 1);
                enQueue(q, node->left);
            }

            if(node->right != NULL) {
                node->right->horizontalLevel = node->horizontalLevel + 1;
                node->right->verticalLevel = (node->verticalLevel + 1);
                enQueue(q, node->right);
            }

        }

        // after this level
        pos += size;
        leftToRight = !leftToRight;
    }

    return;
}

int pre[100];
int in[100];
int post[100];
int prePos = 0;
int inPos = 0;
int postPos = 0;

void allTraversal(NODE* root)
{
    prePos = 0;
    inPos = 0;
    postPos = 0;

    NODE *node = root;

    if (NULL == root) {
        return;
    }

    STACK *s = initStack();

    node->order = 1;
    stackPush(s, node);

    while (false == isStackEmpty(s)) {

        node = pop(s);

        // This is part of pre increment 1 to 2 push the left side of the tree
        if (node->order == 1) {

            pre[prePos++] = node->data;
            node->order++;
            stackPush(s, node);

            if (node->left != NULL) {

                node->left->order = 1;
                stackPush(s, node->left);
            }
        } else if (node->order == 2) { // This is a part of in increment 2 to 3 push right

            in[inPos++] = node->data;
            node->order++;
            stackPush(s, node);

            if (node->right != NULL) {

                node->right->order = 1;
                stackPush(s, node->right);
            }
        } else { // don't push it back again
            post[postPos++] = node->data;
        }
    }
}

int left[100];
int right[100];
int leaves[100];
int leftPos = 0;
int rightPos = 0;
int leafPos = 0;

bool isLeaf(NODE* root)
{
    if ((NULL == root->left) && (NULL == root->right)) {
        return true;
    }

    return false;
}

void addLeftBoundary(NODE* root)
{
    leftPos = 0;
    NODE* cur = root->left;

    while (cur) {

        if (false == isLeaf(cur)) {
            left[leftPos++] = cur->data;
        }

        if (cur->left) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
}

void addRightBoundary(NODE* root)
{
    int swap = 0;
    rightPos = 0;
    NODE* cur = root->right;

    while (cur) {

        /* Consider reversing the elements at the end */
        if (false == isLeaf(cur)) {
            right[rightPos++] = cur->data;
        }

        if (cur->right) {
            cur = cur->right;
        } else {
            cur = cur->left;
        }
    }

    for(int i = 0; i < rightPos/2; i++){
        swap = right[i];
        right[i] = right[rightPos-i-1];
        right[rightPos-i-1] = swap;
    }

    return;
}

void addLeaves(NODE* root)
{
    if (true == isLeaf(root)) {
        leaves[leafPos++] = root->data;
        return;
    }

    if (root->left) {
         addLeaves(root->left);
    }

    if (root->right) {
        addLeaves(root->right);
    }

    return;
}

void boundaryTraversal(NODE *root)
{
    if (NULL == root) {
        return;
    }

    addLeftBoundary(root);
    addLeaves(root);
    addRightBoundary(root);

    printf("Boundry of the tree : \r\n");

    printf("%4d", root->data);
    for (int i = 0; i < leftPos; i++) {
        printf("%4d", left[i]);
    }
    for (int i = 0; i < leafPos; i++) {
        printf("%4d", leaves[i]);
    }
    for (int i = 0; i < rightPos; i++) {
        printf("%4d", right[i]);
    }
    printf("\r\n");

    return;
}

bool getPath(NODE * root, int x) {

    if (NULL == root) {
        return false;
    }

    traversal[pos++] = root->data;

    if (root->data == x) {
        return true;
    }

    if (getPath(root->left, x) || getPath(root->right, x)) {
        return true;
    }

    traversal[pos--] = 0;
    return false;
}

int main() {

    NODE * root = newNode(1);

    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);
    root->left->right->left = newNode(8);
    root->right->left = newNode(6);
    root->right->right = newNode(7);
    root->right->right->left = newNode(9);
    root->right->right->right = newNode(10);

    printf("\r\nIterative Inorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    inOrderTraversal(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n");

    printf("Recursive Inorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    inOrderTrav(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n");

    printf("Morris Inorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    inOrderTraversalMorris(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("Iterative Preorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    preOrderTraversal(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n");

    printf("Recursive Preorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    preOrderTrav(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n");

    printf("Morris Preorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    preOrderTraversalMorris(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("Iterative Postorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    postOrderTraversal(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n");

    printf("Recursive Postorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    postOrderTrav(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");


    printf("\r\nRecursive Levelorder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    levelOrderTraversal(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("\r\nRecursive LeftSideView : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    leftSideView(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("\r\nRecursive RightSideView : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    rightSideView(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("\r\nIterative BottomView : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    bottomView(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");


    printf("\r\nIterative TopView : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    topView(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");


    printf("\r\nIterative LevelOrder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    levelOrder(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("\r\nIterative verticalOrder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    verticalTraversal(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("\r\nIterative Spiral : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    SpiralLevelOrder(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("\r\nIterative zigzagLevelOrder : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    zigzagLevelOrder(root);
    printf("\r\n");
    for (int i = 0; i < pos; i++) {
        printf("%5d ", traversal[i]);
    }
    printf("\r\n\r\n");

    printf("\r\nIterative AllTraversal : \r\n");
    memset(in, 0, sizeof(in));
    memset(pre, 0, sizeof(pre));
    memset(post, 0, sizeof(post));

    allTraversal(root);

    printf("\r\n");
    printf("Traversal Pre Order Output :\r\n");
    for (int i = 0; i < prePos; i++) {
        printf("%5d ", pre[i]);
    }
    printf("\r\n");
    printf("Traversal In Order Output :\r\n");
    for (int i = 0; i < inPos; i++) {
        printf("%5d ", in[i]);
    }
    printf("\r\n");
    printf("Traversal Post Order Output :\r\n");
    for (int i = 0; i < postPos; i++) {
        printf("%5d ", post[i]);
    }
    printf("\r\n\r\n");

    boundaryTraversal(root);
    printf("\r\n\r\n");

    printf("\r\nRecursive Get Path : \r\n");
    memset(traversal, 0, sizeof(traversal));
    pos = 0;
    if(getPath(root, 8)) {
        for (int i = 0; i < pos; i++) {
            printf("%5d ", traversal[i]);
        }
    } else {
        printf("Node not found \r\n");

    }
    printf("\r\n\r\n");


    return 0;
}