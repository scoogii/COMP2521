////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//                       Implementation File                          //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invertedIndex.h"

////////////////////////////////////////////////////////////////////////

#define MAX_WORD_LEN 101

////////////////////////////////////////////////////////////////////////

FileList newFileListNode(char *filename, double tf);
InvertedIndexBST InvertedIndexBSTNew(void);
InvertedIndexBST FileListInsert(InvertedIndexBST, FileList);

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
    // Make all characters lowercase
    for (char *currChar = str; *currChar; currChar++) {
        *currChar = tolower(*currChar);
    }

    // Remove leading whitespaces
    while (isspace(str[0])) str++;

    // Create pointer to last character of string
    char *lastChar = str + strlen(str) - 1;

    // Remove trailing whitespaces
    while (isspace((*lastChar))) lastChar--;

    // Remove ending punctuation mark
    if (*lastChar == '.' || *lastChar == ',' || *lastChar == ';' ||
        *lastChar == '?')
        lastChar--;

    // Adding new null terminator taken from Stack Overflow
    // Post was about how to trim trailing whitespaces
    lastChar[1] = '\0';

    return str;
}
