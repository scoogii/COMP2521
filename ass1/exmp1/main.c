////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//                     Main Program for Testing                       //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "invertedIndex.h"

static void showBSTreeNode(InvertedIndexBST t);
static void showListNodes(FileList list);
void BSTreeInfix(InvertedIndexBST t);

int main(void) {
    InvertedIndexBST t = generateInvertedIndex("collection.txt");

    // Check deets are right
    printInvertedIndex(t);

    return 0;
}

// Prints the value in the given node
static void showBSTreeNode(InvertedIndexBST t) {
    if (t == NULL) return;
    printf("%s :=: ", t->word);
}

// Prints out the filelist
static void showListNodes(FileList list) {
    if (list == NULL) return;

    while (list != NULL) {
        printf("%s ( %lf ) -> ", list->filename, list->tf);
        list = list->next;
    }
    printf("*\n");
}

// Prints the values in the given BSTree in infix order
void BSTreeInfix(InvertedIndexBST t) {
    if (t == NULL) return;

    BSTreeInfix(t->left);
    showBSTreeNode(t);
    showListNodes(t->fileList);
    BSTreeInfix(t->right);
}
