////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//                          InvertedIndex                             //
//                       Implementation File                          //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "invertedIndex.h"
#include "myLists.h"

////////////////////////////////////////////////////////////////////////

#define MAX_WORD_LEN 101

////////////////////////////////////////////////////////////////////////
//                   InvertedIndexBST Functions                       //
////////////////////////////////////////////////////////////////////////


/**
 * Creates a new InvertedIndexBST that is initialised as NULL
 * Adapted from lab03 BSTree.c
 */
InvertedIndexBST InvertedIndexBSTNew(void) { return NULL;}


/**
 * Creates a new InvertedIndexBSTNode pointer and returns it
 */
InvertedIndexBST newInvertedIndexBST(char *key) {
    InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));

    new->fileList = NULL;
    new->left = NULL;
    new->right = NULL;
    new->word = generateNewString(key);

    return new;
}


/**
 * Inserts an initialised InvertedIndexBST node into the current tree and
 * returns the root of the updated tree
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
 * Takes in a file pointer and InvertedIndexBST node and prints the tree's 'key'
 * to the output file
 */
static void showInvertedIndexBSTNode(FILE* f, InvertedIndexBST tree) {
    if (tree == NULL) return;
    fprintf(f, "%s ", tree->word);
}


/**
 * Takes in a file pointer and InvertedIndexBST node and prints out the 'keys'
 * of the nodes in infix order while also printing out the list of filenames
 * and its tf value in each node to the output file
 */
void BSTreeInfixToFile(FILE *f, InvertedIndexBST tree) {
    if (tree == NULL) return;

    BSTreeInfixToFile(f,tree->left);
    showInvertedIndexBSTNode(f, tree);
    showFileListNodes(f, tree->fileList);
    BSTreeInfixToFile(f, tree->right);
}


/**
 * Takes in an InvertedIndexBST node and a given 'key'
 * Function searches for a node with a matching 'key' and returns it
 * If not node is found, NULL is returned
 */
FileList InvertedIndexBSTFind(InvertedIndexBST tree, char *key) {
    // If tree is NULL no key is found, return NULL
    if (tree == NULL) {
        return NULL;
    }
    // Traverse left subtree if key is 'before' tree->word
    else if (strcmp(key, tree->word) < 0) {
        return InvertedIndexBSTFind(tree->left, key);
    }
    // Traverse right subtree if key is 'after' tree->word
    else if (strcmp(key, tree->word) > 0) {
        return InvertedIndexBSTFind(tree->right, key);
    }
    // If there is a match, return the FileList
    else {
        return tree->fileList;
    }
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
    lastChar[1] = '\0';

    return str;
}


/**
 * This function opens the collection file with the given name, and then
 * generates an inverted index from those files listed in the collection
 * file,  as  discussed  in  the spec. It returns the generated inverted
 * index.
 */
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    // Create a InvertedIndexBST root node that is currently empty
    InvertedIndexBST root = InvertedIndexBSTNew();

    // Open collection file
    FILE *collectionFile = fopen(collectionFilename, "r");
    if (collectionFile == NULL) {
        perror(collectionFilename);
        exit(1);
    }

    // Loop through the file and open each subsequent file
    char wordsFileName[MAX_WORD_LEN] = {};
    while (fscanf(collectionFile, "%s", wordsFileName) != EOF) {
        FILE *wordsFile = fopen(wordsFileName, "r");
        if (wordsFile == NULL) {
            perror(wordsFileName);
            exit(1);
        }

        // Parse words in file then insert/update the word's InvertedIndexBST node
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


////////////////////////////////////////////////////////////////////////
//  					 PART 2 FUNCTIONS                             //
////////////////////////////////////////////////////////////////////////


/**
 * Returns  an  ordered list where each node contains a filename and the 
 * corresponding tf-idf value for a given searchWord. You only  need  to
 * include documents (files) that contain the given searchWord. The list
 * must  be  in  descending order of tf-idf value. If there are multiple
 * files with same tf-idf, order them by  their  filename  in  ascending
 * order. D is the total number of documents in the collection.
 */
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    // Create a TfIdfList head node that is currently empty
    TfIdfList head = TfIdfListNew();

    // Search for the given 'key'
    FileList filenames = InvertedIndexBSTFind(tree, searchWord);
    // If it doesn't exist, return NULL head node
    if (filenames == NULL) {
        return head;
    }

    // Calculate idf for filenames
    double idf = log10(D / (double)findLengthList(filenames));

    for (FileList curr = filenames; curr != NULL; curr = curr->next) {
        // Create new TfIdfList node and insert in the new list
        TfIdfList newNode = newTfIdfListNode(curr->filename, (curr->tf * idf));
        head = TfIdfListInsert(head, newNode);
    }

    return head;
}


/**
 * Returns  an  ordered list where each node contains a filename and the
 * summation of tf-idf values of all the matching search words for  that
 * file.  You only need to include documents (files) that contain one or
 * more of the given search words. The list must be in descending  order
 * of summation of tf-idf values (tfIdfSum). If there are multiple files
 * with  the  same tf-idf sum, order them by their filename in ascending
 * order. D is the total number of documents in the collection.
 */
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {
    // Create a TfIdfList head node that is currently empty
    TfIdfList head = TfIdfListNew();

    // Search for each given 'key' in the array of words
    // Generate a new TfIdfList from it and merge it with other lists
    for (int i = 0; searchWords[i] != NULL; i++) {
        TfIdfList unsortedList = calculateTfIdf(tree, searchWords[i], D);

        head = TfIdfListMerge(head, unsortedList);
    }

    // Insert duplicates of the current nodes into other list such that it is ordered
    // If filenames are the same, sum tfidf values
    TfIdfList sortedList = TfIdfListNew();
    for (TfIdfList curr = head; curr != NULL; curr = curr->next) {
        TfIdfList newNode = newTfIdfListNode(curr->filename, curr->tfIdfSum);
        sortedList = TfIdfListInsert(sortedList, newNode);
    }

    return sortedList;
}
