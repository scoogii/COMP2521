////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//                       Implementation File                          //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <math.h>
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
FileList InvertedIndexBSTFind(InvertedIndexBST tree, char *key);
int findLengthList(FileList list);

////////////////////////////////////////////////////////////////////////
//                  ADAPTED BST FUNCTIONS FROM LAB03                  //
////////////////////////////////////////////////////////////////////////


/**
 * Creates a new InvertedIndexBST that is initialised as NULL
 */
InvertedIndexBST InvertedIndexBSTNew(void) { return NULL;}


/*
 * Creates a new TfIdfList that is initialised as NULL
 */
TfIdfList TfIdfListNew(void) { return NULL; }


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
 * Takes in a FileList node and the idf value
 * Creates a new TfIdfNode pointer from them and returns it
 */
TfIdfList newTfIdfListNode(FileList list, double idf) {
    TfIdfList new = malloc(sizeof(struct TfIdfNode));
    new->filename = strdup(list->filename);
    new->tfIdfSum = (list->tf * idf);
    new->next = NULL;

    return new;
}


/**
 * Inserts an initialised InvertedIndexBST node into the current tree and
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


/**
 * Inserts a new TfIdfList node into the current TfIdfList and returns
 * the head of the updated TfIdfList
 */
TfIdfList TfIdfListInsert(TfIdfList head, FileList node, double idf) {
    // Create new TfIdfList node to be inserted
    TfIdfList newNode = newTfIdfListNode(node, idf);

    // Case 1 & 2. head is NULL or idf is the largest
    if (head == NULL || newNode->tfIdfSum > head->tfIdfSum) {
        newNode->next = head;
        head = newNode;
        return newNode;
    }

    // Case 3. node to be inseted somewhere within the list
    TfIdfList prev = head;
    TfIdfList curr = head;
    for (; curr != NULL; prev = curr, curr = curr->next) {
        if (newNode->tfIdfSum > curr->tfIdfSum) {
            prev->next = newNode;
            newNode->next = curr;
            return head;
        }

        // If idf values are equal, insert in ascending filename order
        if (newNode->tfIdfSum == curr->tfIdfSum) {
            // Inserting before
            if (strcmp(node->filename, curr->filename) < 0) {
                prev->next = newNode;
                newNode->next = curr;
            }
            // Inserting after
            else {
                newNode->next = curr->next;
                curr->next = newNode;
            }

            return head;
        }
    }

    // Case 4. node to be inserted as the tail
    prev->next = newNode;
    newNode->next = NULL;
    return head;
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
 * Takes in a file pointer and FileList node and prints the list of files
 * and its tf values to the output file
 */
static void showFileListNodes(FILE *f, FileList list) {
    if (list == NULL) return;

    for (; list != NULL; list = list->next) fprintf(f, "%s (%lf) ", list->filename, list->tf);
    fprintf(f, "\n");
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


/**
 * Takes in a FileList as an argument and returns the length
 * of it
 */
int findLengthList(FileList list) {
    int listLength = 0;
    for (; list != NULL; list = list->next) listLength++;

    return listLength;
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
    InvertedIndexBST root = InvertedIndexBSTNew();

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


/**
 * Returns  an  ordered list where each node contains a filename and the 
 * corresponding tf-idf value for a given searchWord. You only  need  to
 * include documents (files) that contain the given searchWord. The list
 * must  be  in  descending order of tf-idf value. If there are multiple
 * files with same tf-idf, order them by  their  filename  in  ascending
 * order. D is the total number of documents in the collection.
 */
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    // Create a InvertedIndexBST root node that is currently empty
    TfIdfList head = TfIdfListNew();

    // Search for the given 'key'
    FileList filenames = InvertedIndexBSTFind(tree, searchWord);
    // If it doesn't exist, return NULL head node
    if (filenames == NULL) {
        return head;
    }

    double idf = log10(D / (double)findLengthList(filenames));

    for (FileList curr = filenames; curr != NULL; curr = curr->next) {
        if (strcmp(filenames->filename, "COMP9415") == 0) {
            printf("Gonna insert '%s' from file '%s'", searchWord, filenames->filename);
        }
        head = TfIdfListInsert(head, curr, idf);
    }

    return head;
}


TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {
    tree = NULL;
    searchWords = NULL;
    D = 0;
    return NULL;
}
