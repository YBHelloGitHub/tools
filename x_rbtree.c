#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


#define RB_RED   0
#define RB_BLACK 1

#define TRACE()     printf("%s %d \n", __FUNCTION__, __LINE__)

typedef struct tagRBNode {
    struct tagRBNode *parent;
    struct tagRBNode *left;
    struct tagRBNode *right;
    int color;
} RBNode;

typedef int (*cmp)(const void *, const void *);
typedef struct {
    RBNode *root;
    cmp cmp;
} RBTree;

void RBTreeTransplate(RBTree *tree, RBNode *u, RBNode *v)
{
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

RBNode *RBTreeMiniNum(RBNode *root)
{
    RBNode *nil = root;
    while (nil->left != NULL) {
        nil = nil->left;
    }

    return nil;
}

void RBTreeRorateLeft(RBTree *t, RBNode *x)
{
    RBNode *y = x->right;

    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void RBTreeRorateRight(RBTree *t, RBNode *y)
{
    RBNode *x = y->left;

    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == NULL) {
        t->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}


void RBTreeAddColor(RBTree *t, RBNode *z)
{
    TRACE();

    while (z->parent && z->parent->color == RB_RED) {
printf("%p\n", z->parent->parent);
TRACE();
        if (z->parent == z->parent->parent->left) {
            TRACE();
            RBNode *y = z->parent->parent->left;
            if (y && y->color == RB_RED) {
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
                continue;
            }

            if (z == z->parent->right) {
                z = z->parent;
                RBTreeRorateLeft(t, z);
            }
TRACE();
            z->parent->color = RB_BLACK;
            z->parent->parent->color = RB_RED;
            RBTreeRorateRight(t, z->parent->parent);
        } else {
            printf("%p\n", z->parent->parent);
            RBNode *y = z->parent->parent->left;
            if (y && y->color == RB_RED) {
TRACE();
                z->parent->color = RB_BLACK;
                y->color = RB_BLACK;
                z->parent->parent->color = RB_RED;
                z = z->parent->parent;
                continue;
            }
TRACE();
printf("%p\n", z->parent->parent);
            if (z == z->parent->left) {

                z = z->parent;
TRACE();
printf("%p\n", z->parent->parent);
                RBTreeRorateRight(t, z);
printf("%p\n", z->parent->parent);
            }
TRACE();
TRACE();
printf("%p\n", z->parent);
printf("%p\n", z->parent->parent);
            z->parent->color = RB_BLACK;
            z->parent->parent->color = RB_RED;
TRACE();
            RBTreeRorateLeft(t, z->parent->parent);
        }
    }
    TRACE();
    t->root->color = RB_BLACK;
}

int RBTreeAdd(RBTree *t, RBNode *z)
{
    RBNode *y = NULL;
    RBNode *x = t->root;
    int ret = 0;

    while (x != NULL) {
        y = x;
        ret = t->cmp(z, x);
        if (ret < 0) {
TRACE();
            x = x->left;
        } else if (ret > 0) {
TRACE();
            x  = x->right;
        } else {
            return -1;
        }
    }
TRACE();
    z->parent = y;
    if (y == NULL) {
        t->root = z;
    } else if (ret < 0) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = NULL;
    z->right = NULL;
    z->color = RB_RED;
    RBTreeAddColor(t, z);
TRACE();
    return 0;
}

void RBTreeDelColor(RBTree *t, RBNode *x)
{
    while (x != t->root && x->color == RB_BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RB_RED) {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                RBTreeRorateLeft(t, x->parent);
                w = x->parent->right;
            }

            if ((!w->left || w->left->color == RB_BLACK) &&
                (!w->right || w->right->color == RB_BLACK)) {
                w->color = RB_RED;
                x = x->parent;
                continue;
            }

            if (!w->right || w->right->color == RB_BLACK) {
                w->left->color = RB_BLACK;
                w->color = RB_RED;
                RBTreeRorateRight(t, w);
                w = x->parent->right;
            }

            w->color = x->parent->color;
            x->parent->color = RB_BLACK;
            RBTreeRorateLeft(t, x->parent);
            x = t->root;
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RB_RED) {
                w->color = RB_BLACK;
                x->parent->color = RB_RED;
                RBTreeRorateRight(t, x->parent);
                w = x->parent->left;
            }

            if ((!w->left || w->left->color == RB_BLACK) &&
                (!w->right || w->right->color == RB_BLACK)) {
                w->color = RB_RED;
                x = x->parent;
                continue;
            }

            if (!w->left || w->left->color == RB_BLACK) {
                w->right->color = RB_BLACK;
                w->color = RB_RED;
                RBTreeRorateLeft(t, w);
                w = x->parent->left;
            }

            w->color = x->parent->color;
            x->parent->color = RB_BLACK;
            RBTreeRorateRight(t, x->parent);
            x = t->root;
        }
    }
    if (x) {
        x->color = RB_BLACK;
    }
}

int RBTreeDel(RBTree *tree, RBNode *z)
{
    RBNode *y = NULL;
    RBNode *x = NULL;
    int color;

    y = z;
    color = y->color;

    if (z->left == NULL) {
        x = z->right;
        RBTreeTransplate(tree, z, x);
    } else if (z->right == NULL) {
        x = z->left;
        RBTreeTransplate(tree, z, x);
    } else {
        y = RBTreeMiniNum(z->right);
        color = y->color;

        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            RBTreeTransplate(tree, y, x);
            y->right = z->right;
            z->right->parent = y->right;
        }

        RBTreeTransplate(tree, z, y);

        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (color == RB_BLACK) {
        RBTreeDelColor(tree, x);
    }

    return 0;
}



typedef struct {
    RBNode node;
    int key;
} ERBNode;

int compareERBNode(const void *n1, const void *n2)
{
    ERBNode *d1 = (ERBNode *)n1;
    ERBNode *d2 = (ERBNode *)n2;

    return d1->key - d2->key;
}

void randUniqNum(int n, int *arr)
{
    int i = 0;
    int j = 0;
    while (i < n) {
        arr[i] = rand() % n;
        for (j = 0; j < i; j++) {
            if (arr[i] == arr[j]) {
                break;
            }
        }
        if (i == j) {
            i++;
        }
    }
}



int main(int argc, char **argv)
{


    int *arr = NULL;
    int count = 10;

    if (argc == 2) {
        count = atoi(argv[2]);
    }

    arr = (int *)calloc(count, sizeof(int));
    if (!arr) {
        printf("no mem.\n");
        exit(-1);
    }

    randUniqNum(count, arr);

    RBTree tree;
    RBTree *t = &tree;
    tree.root = NULL;
    tree.cmp = compareERBNode;

    ERBNode **nodeArr = (ERBNode **)calloc(count, sizeof(ERBNode *));

    for (int i = 0; i < count; i++) {
        ERBNode *enode = (ERBNode *)calloc(1, sizeof(ERBNode));

        nodeArr[i] = enode;
        enode->key = arr[i];
        printf("%d-%d-%p\n", i, arr[i], enode);
        RBTreeAdd(t, (RBNode *)&enode->node);
        printf("%d-P(%p)-L(%p)-R(%p)\n",
               enode->node.color, enode->node.parent, enode->node.left, enode->node.right);
    }

    return 0;
}
