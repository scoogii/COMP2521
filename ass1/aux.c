////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//					       Helper Functions                           //
//                        Implementation File                         //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include "aux.h"

////////////////////////////////////////////////////////////////////////

#define MAX_WORD_LEN 101

////////////////////////////////////////////////////////////////////////

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
