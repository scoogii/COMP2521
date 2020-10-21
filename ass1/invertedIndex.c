////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invertedIndex.h"
#include "myBST.h"

////////////////////////////////////////////////////////////////////////

#define MAX_WORD_LEN 100

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//                    BST FUNCTIONS FROM LAB03                        //
////////////////////////////////////////////////////////////////////////

/**
 * Creates a new FileListNode pointer and returns it
 */
FileList newFileListNode(char *filename, double tf) {
    FileList newNode = malloc(sizeof(FileList));
    newNode->filename = filename;
    newNode->tf = tf;
    newNode->next = NULL;

    return newNode;
}

/* Creates a new empty InvertedIndexBST */
InvertedIndexBST InvertedIndexBSTNew(void) { return NULL; }

/**
 * Inserts a new FileListNode into the given InvertedIndexBST and returns
 * the root of the updated InvertedIndexBST. Does not insert
 * duplicate values
 */
InvertedIndexBST FileListInsert(InvertedIndexBST, FileList);

////////////////////////////////////////////////////////////////////////
//  					 PART 1 FUNCTIONS                             //
////////////////////////////////////////////////////////////////////////

/**
 * Normalises a given string. See the spec for details. Note: you should
 * modify the given string - do not create a copy of it.
 */
char *normaliseWord(char *str) {
    // Remove all leading and trailing spaces
}
