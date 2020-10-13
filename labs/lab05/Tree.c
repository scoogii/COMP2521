// Implementation of the BST ADT using an AVL tree

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Time.h"
#include "Tree.h"

typedef struct node *Node;
struct node {
    Time time;
    int height;
    Node left;
    Node right;
};

struct tree {
    Node root;
};

////////////////////////////////////////////////////////////////////////
// Function Prototypes

static void doFree(Node n);
static Node doInsert(Node n, Time time);
static Node rotateLeft(Node n);
static Node rotateRight(Node n);
static int height(Node n);
static int max(int a, int b);
static Node newNode(Time time);
Time doTreeFloor(Node node, Time time);
Time doTreeCeiling(Node node, Time time);

////////////////////////////////////////////////////////////////////////
// Constructor and Destructor

// Creates a new empty tree
Tree TreeNew(void) {
    Tree t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    t->root = NULL;
    return t;
}

// Frees all memory associated with the given tree
void TreeFree(Tree t) {
    doFree(t->root);
    free(t);
}

static void doFree(Node n) {
    if (n != NULL) {
        doFree(n->left);
        doFree(n->right);
        TimeFree(n->time);
        free(n);
    }
}

////////////////////////////////////////////////////////////////////////
// Insertion

// Inserts  a  copy of the given time into the tree if it is not already
// in the tree
void TreeInsert(Tree t, Time time) { t->root = doInsert(t->root, time); }

static Node doInsert(Node n, Time time) {
    if (n == NULL) { return newNode(time); }

    // insert recursively
    int cmp = TimeCmp(time, n->time);
    if (cmp < 0) {
        n->left = doInsert(n->left, time);
    } else if (cmp > 0) {
        n->right = doInsert(n->right, time);
    } else {  // (cmp == 0)
        // if time is already in the tree,
        // we can return straight away
        return n;
    }

    // insertion done
    // fix the height of the current subtree
    n->height = 1 + max(height(n->left), height(n->right));

    int balance = height(n->left) - height(n->right);

    // Case 1. If balance > 1 - occurring on left subtree
    // unbalanced tree, rotate right always
    if (balance > 1) {
        // If time of current node > time of left child's time
        if (TimeCmp(time, n->left->time) > 0) n->left = rotateLeft(n->left);
        n = rotateRight(n);
    }
    // Case 2. If balance < -1 - occurring on right subtree
    // unbalanced tree, rotate left always
    if (balance < -1) {
        // If time of current node is < time of right child's time
        if (TimeCmp(time, n->right->time) < 0) n->right = rotateRight(n->right);
        n = rotateLeft(n);
    }

    return n;
}

static Node newNode(Time time) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    n->time = TimeCopy(time);
    n->height = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Rotates  the  given  subtree left and returns the root of the updated
// subtree.
static Node rotateLeft(Node n) {
    if (n == NULL || n->right == NULL) return n;

    // Create n1 Node - the right subtree of n
    Node n1 = n->right;

    n->right = n1->left;  // n->right points to the node of n1->left
    n1->left = n;         // n1->left now points to n, n1 is the new root

    // Adjust heights of the nodes after rotating
    n->height = max(height(n->left), height(n->right)) + 1;
    n1->height = max(height(n1->left), height(n1->right)) + 1;

    return n1;
}

// Rotates the given subtree right and returns the root of  the  updated
// subtree.
static Node rotateRight(Node n) {
    if (n == NULL || n->left == NULL) return n;

    // Create n2 Node - the left subtree of n
    Node n2 = n->left;

    n->left = n2->right;  // n1->left points to the node of n2->right
    n2->right = n;        // n2->right now points at n1, n2 is the new root

    // Adjust heights of the nodes after rotating
    n->height = max(height(n->left), height(n->right)) + 1;
    n2->height = max(height(n2->left), height(n2->right)) + 1;

    return n2;
}

// Returns the height of a subtree while assuming that the height field
// of the root node of the subtree is correct
static int height(Node n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->height;
    }
}

static int max(int a, int b) { return a > b ? a : b; }

////////////////////////////////////////////////////////////////////////
// Specialised Operations

// Returns the latest time in the tree that is earlier than or equal  to
// the  given  time,  or  NULL if no such time exists. The returned time
// should not be modified or freed.
Time TreeFloor(Tree t, Time time) {
    if (t == NULL) return NULL;
    return doTreeFloor(t->root, time);
}

Time doTreeFloor(Node node, Time time) {
    // Node is null therefore no such time exists
    if (node == NULL) return NULL;

    // current time > given time
    if (TimeCmp(node->time, time) > 0) return doTreeFloor(node->left, time);

    // current time < given time
    if (TimeCmp(node->time, time) < 0) {
        if (node->left == NULL || node->right == NULL) return node->time;

        if (TimeCmp(time, node->left->time) > 0 &&
                TimeCmp(time, node->right->time) < 0) {
            return node->time;
        }

        return doTreeFloor(node->right, time);
    }

    // Given time is found
    return time;
}

// Returns the earliest time in the tree that is later than or equal  to
// the  given  time,  or  NULL if no such time exists. The returned time
// should not be modified or freed.
Time TreeCeiling(Tree t, Time time) {
    if (t == NULL) return NULL;
    return doTreeCeiling(t->root, time);
}

Time doTreeCeiling(Node node, Time time) {
    // Node is null therefore no such time exists
    if (node == NULL) return NULL;

    // current time < given time
    if (TimeCmp(node->time, time) < 0) return doTreeCeiling(node->right, time);

    // current time > given time
    if (TimeCmp(node->time, time) > 0) {
        if (node->left == NULL || node->right == NULL) return node->time;

        if (node->left != NULL && node->right != NULL) {
            if (TimeCmp(time, node->left->time) > 0 &&
                    TimeCmp(time, node->right->time) < 0)
                return node->time;
        }

        return doTreeCeiling(node->left, time);
    }
    // time is found
    else {
        return time;
    }
}

////////////////////////////////////////////////////////////////////////
// Printing

//////////////////
// List the items

static void doList(Node n);

void TreeList(Tree t) { doList(t->root); }

static void doList(Node n) {
    if (n != NULL) {
        doList(n->left);
        TimeShow(n->time);
        printf("\n");
        doList(n->right);
    }
}

///////////////////////////
// Show the tree structure

typedef unsigned long long uint64;
static void doShow(Node n, int level, uint64 arms);

void TreeShow(Tree t) {
    if (t->root != NULL && t->root->height >= 64) {
        printf("Tree is too tall!\n");
    } else {
        doShow(t->root, 0, 0);
    }
}

// This  function  uses a hack to determine when to draw the arms of the
// tree and relies on the tree being reasonably balanced. Don't  try  to
// use this function if the tree is not an AVL tree!
static void doShow(Node n, int level, uint64 arms) {
    if (n == NULL) return;

    TimeShow(n->time);
    printf(" (height: %d)\n", n->height);

    if (n->left != NULL) {
        for (int i = 0; i < level; i++) {
            if ((1LLU << i) & arms) {
                printf("│     ");
            } else {
                printf("      ");
            }
        }
        printf("%s", n->right != NULL ? "┝━╸L: " : "┕━╸L: ");
        if (n->right != NULL) {
            arms |= (1LLU << level);
        } else {
            arms &= ~(1LLU << level);
        }
        doShow(n->left, level + 1, arms);
    }

    if (n->right != NULL) {
        // if (n->left != NULL) {
        //     for (int i = 0; i <= level; i++) {
        //         if ((1LLU << i) & arms) {
        //             printf("│     ");
        //         } else {
        //             printf("      ");
        //         }
        //     }
        //     printf("\n");
        // }
        for (int i = 0; i < level; i++) {
            if ((1LLU << i) & arms) {
                printf("│     ");
            } else {
                printf("      ");
            }
        }
        printf("┕━╸R: ");
        arms &= ~(1LLU << level);
        doShow(n->right, level + 1, arms);
    }
}

////////////////////////////////////////////////////////////////////////
// Testing
// All  functions below exist for testing purposes ONLY. Do NOT use them
// in your code.

static Node doInsertLeaf(Node n, Time time);

void TreeRotateLeftAtRoot(Tree t) { t->root = rotateLeft(t->root); }

void TreeRotateRightAtRoot(Tree t) { t->root = rotateRight(t->root); }

void TreeInsertLeaf(Tree t, Time time) {
    t->root = doInsertLeaf(t->root, time);
}

static Node doInsertLeaf(Node n, Time time) {
    if (n == NULL) { return newNode(time); }

    int cmp = TimeCmp(time, n->time);
    if (cmp < 0) {
        n->left = doInsertLeaf(n->left, time);
    } else if (cmp > 0) {
        n->right = doInsertLeaf(n->right, time);
    }

    n->height = 1 + max(height(n->left), height(n->right));
    return n;
}
