////////////////////////////////////////////////////////////////////////
// 					 myBST Header File for BST ADT                    //
//			         Functions sourced from lab03                     //
//                                                                    //
//				      Christian Nguyen - z5310911                     //
////////////////////////////////////////////////////////////////////////

#ifndef MYBST_H
#define MYBST_H

#include "invertedIndex.h"

////////////////////////////////////////////////////////////////////////

/**
 * Creates a new FileListNode pointer and returns it
 */
FileList newFileListNode(char *filename, double tf);

/* Creates a new empty InvertedIndexBST */
InvertedIndexBST InvertedIndexBSTNew(void);

/** 
 * Inserts a new FileListNode into the given InvertedIndexBST and returns
 * the root of the updated InvertedIndexBST. Does not insert
 * duplicate values
 */
InvertedIndexBST FileListInsert(InvertedIndexBST, FileList);

////////////////////////////////////////////////////////////////////////

#endif
