////////////////////////////////////////////////////////////////////////
//                      COMP2521 Assignment 1                         //
//                      Information Retrieval                         //
//                      FileLists & TfIdfLists                        //
//                       Implementation File                          //
//                                                                    //
//                    Christian Nguyen - z5310911                     //
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "myLists.h"

////////////////////////////////////////////////////////////////////////

#define MAX_WORD_LEN 101

////////////////////////////////////////////////////////////////////////
//                        FileList Functions                          //
////////////////////////////////////////////////////////////////////////


/**
 * Takes in a filename and tf value
 * Creates a new FileListNode pointer and returns it
 * Adapted from lab03 exercises
 */
FileList newFileListNode(char *filename, double tf) {
    FileList new = malloc(sizeof(struct FileListNode));
    new->filename = generateNewString(filename);
    new->tf = tf;
    new->next = NULL;

    return new;
}


/**
 * Takes in a file pointer and FileList node and prints the list of files
 * and its tf values to the output file
 */
void showFileListNodes(FILE *f, FileList list) {
    if (list == NULL) return;

    for (; list != NULL; list = list->next) fprintf(f, "%s (%lf) ", list->filename, list->tf);
    fprintf(f, "\n");
}


/**
 * Takes in a FileList as an argument and returns the length
 * of it
 */
int findLengthList(FileList list) {
    int listLength = 0;
    for (; list != NULL; list = list->next) listLength++;

    return listLength;
}


/**
 * Inserts a new FileList node into the given InvertedIndexBST and returns
 * the root of the updated InvertedIndexBST
 */
InvertedIndexBST FileListInsert(InvertedIndexBST tree, FileList fileNode) {
    // Case 1 & 2. Tree is NULL or filename is before first filename in the list
    if (tree->fileList == NULL || strcmp(fileNode->filename, tree->fileList->filename) < 0) {
        fileNode->next = tree->fileList;
        tree->fileList = fileNode;
        return tree;
    }

    // Case 3. fileNode to be inserted somewhere within the list
    FileList prev = tree->fileList;
    FileList curr = tree->fileList;
    for (; curr != NULL; prev = curr, curr = curr->next) {
        if (strcmp(fileNode->filename, curr->filename) < 0) {
            prev->next = fileNode;
            fileNode->next = curr;
            return tree;
        }

        // Repeated files should not be inserted again
        if (strcmp(fileNode->filename, curr->filename) == 0) {
            return tree;
        }
    }

    // Case 4. fileNode to be inserted as the tail
    prev->next = fileNode;
    fileNode->next = NULL;
    return tree;
}


////////////////////////////////////////////////////////////////////////
//                       TfIdfList Functions                          //
////////////////////////////////////////////////////////////////////////


/*
 * Creates a new TfIdfList that is initialised as NULL
 * Adapted from lab03 exercises
 */
TfIdfList TfIdfListNew(void) { return NULL; }


/**
 * Takes in a filename and a tfidf value
 * Creates a new TfIdfNode pointer from them and returns it
 */
TfIdfList newTfIdfListNode(char *filename, double tfidf) {
    TfIdfList new = malloc(sizeof(struct TfIdfNode));
    new->filename = generateNewString(filename);
    new->tfIdfSum = tfidf;
    new->next = NULL;

    return new;
}


/**
 * Function that returns the TfIdfList node if a filenane in a TfIdfList
 * is found otherwise returns NULL
 * Adapted from lab03 exercises
 */
TfIdfList TfIdfListFind(TfIdfList head, char *filename) {
    for (; head != NULL; head = head->next) {
        if (strcmp(head->filename, filename) == 0) return head;
    }

    return NULL;
}


/**
 * Inserts a new TfIdfList node into the current TfIdfList and returns
 * the head of the updated TfIdfList
 * List is sorted in descending tfidf value order and if values are equal
 * it is sorted in ascending filename order
 */
TfIdfList TfIdfListInsert(TfIdfList head, TfIdfList node) {
    // If node already exists, sum tfidf value instead
    TfIdfList filenameFound = TfIdfListFind(head, node->filename);
    if (filenameFound != NULL) {
        filenameFound->tfIdfSum += node->tfIdfSum;
        return head;
    }

    // Case 1 & 2. head is NULL or tfidf is the largest
    if (head == NULL || node->tfIdfSum > head->tfIdfSum) {
        node->next = head;
        head = node;
        return head;
    }

    // Case 3. node to be inserted somewhere within the list
    TfIdfList prev = head;
    TfIdfList curr = head;
    for (; curr != NULL; prev = curr, curr = curr->next) {
        // Insert 'before' current since current has smaller tfidf
        if (node->tfIdfSum > curr->tfIdfSum) {
            prev->next = node;
            node->next = curr;
            return head;
        }

        // If idf values are equal, insert in ascending filename order
        if (node->tfIdfSum == curr->tfIdfSum) {
            // Inserting 'before' if filename belongs 'before' the current node
            if (strcmp(node->filename, curr->filename) < 0) {
                prev->next = node;
                node->next = curr;
            }
            // Inserting 'after' the first tfidf match, loop to look for when node
            // should be inserted such that the filenames are ascending
            else if (strcmp(node->filename, curr->filename) > 0) {
                while (curr != NULL && node->tfIdfSum == curr->tfIdfSum) {
                    // Find breaking case
                    if (strcmp(node->filename, curr->filename) < 0) break;
                    prev = curr;
                    curr = curr->next;
                }
                // Insert 'before' so filenames are ascending
                prev->next = node;
                node->next = curr;
            }
            return head;
        }
    }

    // Case 4. node to be inserted as the tail of the list
    prev->next = node;
    node->next = NULL;
    return head;
}


/**
 * Function that merges two TfIdfList's together (list2 into list1)
 * If certain nodes have the same filenames, their tfidf values are summed
 * Returns the merged TfIdfList
 */
TfIdfList TfIdfListMerge(TfIdfList list1, TfIdfList list2) {
    // If one of the lists are NULL, return the latter
    // or if both are NULL, return NULL
    if (list1 != NULL && list2 == NULL) {
        return list1;
    }
    else if (list1 == NULL && list2 != NULL) {
        return list2;
    }
    else if (list1 == NULL && list2 == NULL) {
        return NULL;
    }

    // Insert duplicates of list2 nodes into list1, 'merging' them together
    for (TfIdfList curr = list2; curr != NULL; curr = curr->next) {
        TfIdfList newNode = newTfIdfListNode(curr->filename, curr->tfIdfSum);
        list1 = TfIdfListInsert(list1, newNode);
    }

    return list1;
}
