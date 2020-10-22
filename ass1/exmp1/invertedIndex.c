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
InvertedIndexBST FileListInsert(InvertedIndexBST tree, FileList fileNode);
double calculateTf(char *filename, char *term);
static void showInvertedIndexBSTNode(FILE* f, InvertedIndexBST tree);
static void showFileListNodes(FILE *f, FileList list); 
void BSTreeInfixToFile(FILE *f, InvertedIndexBST tree);

////////////////////////////////////////////////////////////////////////
//                  ADAPTED BST FUNCTIONS FROM LAB03                  //
////////////////////////////////////////////////////////////////////////


/**
 * Creates a new InvertedIndexBSTNode pointer and returns it
 */
InvertedIndexBST newInvertedIndexBST(char *key) {
    InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));

    new->fileList = NULL;
    new->left = NULL;
    new->right = NULL;
    new->word = strdup(key);

    return new;
}


/**
 * Creates a new FileListNode pointer and returns it
 */
FileList newFileListNode(char *filename, double tf) {
    FileList new = malloc(sizeof(struct FileListNode));
    new->filename = strdup(filename);
    new->tf = tf;
    new->next = NULL;

    return new;
}


/**
 * Inserts an initialised InvertedIndexBST into the current tree and
 * returns the root of the updated tree. Does not insert duplicate keys
 */
InvertedIndexBST InvertedIndexBSTInsert(InvertedIndexBST tree, char *filename, char *key) {
    if (tree == NULL) {
        // Create a new tree node and insert word to filelist when tree node is NULL
        tree = newInvertedIndexBST(key);
        FileList fileListNodeNew = newFileListNode(filename, calculateTf(filename, key));
        return FileListInsert(tree, fileListNodeNew);
    }
    else if (strcmp(key, tree->word) < 0) {
        tree->left = InvertedIndexBSTInsert(tree->left, filename, key);
    }
    else if (strcmp(key, tree->word) > 0) {
        tree->right = InvertedIndexBSTInsert(tree->right, filename, key);
    }
    else {
        // If word is found, insert file into filelist for that tree node
        FileList fileListNodeNew = newFileListNode(filename, calculateTf(filename, key));
        return FileListInsert(tree, fileListNodeNew);
    }

    return tree;
}


/**
 * Inserts a new FileListNode into the given InvertedIndexBST and returns
 * the root of the updated InvertedIndexBST
 */
InvertedIndexBST FileListInsert(InvertedIndexBST tree, FileList fileNode) {
    // Case 1. Tree is NULL
    if (tree->fileList == NULL) {
        tree->fileList = fileNode;
        fileNode->next = NULL;
        return tree;
    }

    // Case 2. If fileNode's filename is before first filename
    if (strcmp(fileNode->filename, tree->fileList->filename) < 0) {
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
    }

    // Case 4. fileNode to be inserted as the last node in the list
    prev->next = fileNode;
    fileNode->next = NULL;
    return tree;
}

static void showInvertedIndexBSTNode(FILE* f, InvertedIndexBST tree) {
    if (tree == NULL) return;

    fprintf(f, "%s ", tree->word);
}


static void showFileListNodes(FILE *f, FileList list) {
    if (list == NULL) return;

    for (; list != NULL; list = list->next) fprintf(f, "%s (%lf) ", list->filename, list->tf);
    fprintf(f, "\n");
}


void BSTreeInfixToFile(FILE *f, InvertedIndexBST tree) {
    if (tree == NULL) return;

    BSTreeInfixToFile(f,tree->left);
    showInvertedIndexBSTNode(f, tree);
    showFileListNodes(f, tree->fileList);
    BSTreeInfixToFile(f, tree->right);
}


////////////////////////////////////////////////////////////////////////
//  					 PART 1 FUNCTIONS                             //
////////////////////////////////////////////////////////////////////////


/**
 * Normalises a given string. See the spec for details. Note: you should
 * modify the given string - do not create a copy of it.
 */
char *normaliseWord(char *str) {
    // Make all characters lowercase
    for (char *currChar = str; *currChar; currChar++) { *currChar = tolower(*currChar); }

    // Remove leading whitespaces
    while (isspace(str[0])) str++;

    // Remove trailing whitespaces
    char *lastChar = str + strlen(str) - 1;
    while (isspace((*lastChar))) lastChar--;

    // Remove ending punctuation mark
    if (*lastChar == '.' || *lastChar == ',' || *lastChar == ';' || *lastChar == '?') lastChar--;

    // Adding new null terminator taken from Stack Overflow
    // Post was about how to trim trailing whitespaces
    lastChar[1] = '\0';

    return str;
}


/**
 * Function takes in a file pointer and calculates the relative term
 * frequency adjusted for the document length of file f
 */
double calculateTf(char *filename, char *term) {
    int freq = 0;
    int numWords = 0;
    FILE *f = fopen(filename, "r");

    char scannedTerm[MAX_WORD_LEN] = {};
    while (fscanf(f, "%s", scannedTerm) != EOF) {
        if (strcmp(normaliseWord(scannedTerm), term) == 0) freq++;
        numWords++;
    }

    fclose(f);
    return (freq / (double)numWords);
}


/**
 * This function opens the collection file with the given name, and then
 * generates an inverted index from those files listed in the collection
 * file,  as  discussed  in  the spec. It returns the generated inverted
 * index.
 */
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    // Create a InvertedIndexBST root node that is currently empty
    InvertedIndexBST root = NULL;

    // Open collection file
    FILE *collectionFile = fopen(collectionFilename, "r");
    if (collectionFile == NULL) {
        perror(collectionFilename);
        exit(1);
    }

    char wordsFileName[MAX_WORD_LEN] = {};
    while (fscanf(collectionFile, "%s", wordsFileName) != EOF) {
        // Open the words file
        FILE *wordsFile = fopen(wordsFileName, "r");
        if (wordsFile == NULL) {
            perror(wordsFileName);
            exit(1);
        }

        // Parse words then insert/update the word's InvertedIndexBST node
        char word[MAX_WORD_LEN] = {};
        while (fscanf(wordsFile, "%s", word) != EOF) {
            root = InvertedIndexBSTInsert(root, wordsFileName, normaliseWord(word));
        }

        fclose(wordsFile);
    }

    fclose(collectionFile);

    return root;
}


/**
 * Outputs  the  given inverted index to a file named invertedIndex.txt.
 * The output should contain one line per word, with the  words  ordered
 * alphabetically  in ascending order. Each list of filenames for a word
 * should be ordered alphabetically in ascending order.
 */
void printInvertedIndex(InvertedIndexBST tree) {
    // Create or overwrite the file invertedIndex.txt
    FILE *outputStream = fopen("invertedIndex.txt", "w");
    if (outputStream == NULL) {
        perror("invertedIndex.txt");
        exit(1);
    }

    // If tree is empty, don't print anything and return
    if (tree == NULL) {
        return;
    }

    // Print out tree nodes and their lists in ascending order (i.e. infix)
    BSTreeInfixToFile(outputStream, tree);
    fclose(outputStream);
} 
