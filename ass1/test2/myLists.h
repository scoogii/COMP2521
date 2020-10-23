////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//					    FileLists & TfIdfLists                        //
//                           Header File                              //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#ifndef _LIST_GUARD
#define _LIST_GUARD

////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "invertedIndex.h"

////////////////////////////////////////////////////////////////////////
//                        FileList Functions                          //
////////////////////////////////////////////////////////////////////////

/**
 * Takes in a filename and a tf value
 * Creates a new FileListNode pointer and returns it
 * Adapted from lab03 exercises
 */
FileList newFileListNode(char *filename, double tf);

/**
 * Takes in a file pointer and FileList node and prints the list of files
 * and its tf values to the output file
 */
void showFileListNodes(FILE *f, FileList list);

/**
 * Takes in a FileList as an argument and returns the length
 * of it
 */
int findLengthList(FileList list);

/**
 * Inserts a new FileList node into the given InvertedIndexBST and returns
 * the root of the updated InvertedIndexBST
 */
InvertedIndexBST FileListInsert(InvertedIndexBST tree, FileList fileNode);

////////////////////////////////////////////////////////////////////////
//                       TfIdfList Functions                          //
////////////////////////////////////////////////////////////////////////

/*
 * Creates a new TfIdfList that is initialised as NULL
 * Adapted from lab03 exercises
 */
TfIdfList TfIdfListNew(void);

/**
 * Takes in a filename and a tfidf value
 * Creates a new TfIdfNode pointer from them and returns it
 */
TfIdfList newTfIdfListNode(char *filename, double tfidf);

/**
 * Function that returns the TfIdfList node if a filenane in a TfIdfList
 * is found otherwise returns NULL
 * Adapted from lab03 exercises
 */
TfIdfList TfIdfListFind(TfIdfList head, char *filename);

/**
 * Inserts a new TfIdfList node into the current TfIdfList and returns
 * the head of the updated TfIdfList
 * List is sorted in descending tfidf value order and if values are equal
 * it is sorted in ascending filename order
 */
TfIdfList TfIdfListInsert(TfIdfList head, TfIdfList node);

/**
 * Function that merges two TfIdfList's together (list2 into list1)
 * If certain nodes have the same filenames, their tfidf values are summed
 * Returns the merged TfIdfList
 */
TfIdfList TfIdfListMerge(TfIdfList list1, TfIdfList list2);


#endif
