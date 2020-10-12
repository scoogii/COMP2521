// Interface for the BST ADT

#ifndef TREE_H
#define TREE_H

#include "Time.h"

typedef struct tree *Tree;

// Creates a new empty tree
Tree TreeNew(void);

// Frees all memory associated with the given tree
void TreeFree(Tree t);

// Inserts  a  copy of the given time into the tree if it is not already
// in the tree
void TreeInsert(Tree t, Time time);

// Returns the latest time in the tree that is earlier than or equal  to
// the  given  time,  or  NULL if no such time exists. The returned time
// should not be modified or freed.
Time TreeFloor(Tree t, Time time);

// Returns the earliest time in the tree that is later than or equal  to
// the  given  time,  or  NULL if no such time exists. The returned time
// should not be modified or freed.
Time TreeCeiling(Tree t, Time time);

// Lists all the times in the tree, one per line
void TreeList(Tree t);

// Shows the tree structure
void TreeShow(Tree t);

////////////////////////////////////////////////////////////////////////
// For testing purposes only. Do not use these functions in your code!!!

void TreeInsertLeaf(Tree t, Time time);

void TreeRotateLeftAtRoot(Tree t);

void TreeRotateRightAtRoot(Tree t);

#endif
