////////////////////////////////////////////////////////////////////////
//                      COMP2521 Assignment 1                         //
//                      Information Retrieval                         //
//                         Helper Functions                           //
//                           Header File                              //
//                                                                    //
//                   Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#ifndef _AUX_GUARD
#define _AUX_GUARD

////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "invertedIndex.h"

////////////////////////////////////////////////////////////////////////

/**
 * Function takes in a string and creates a new string with allocated
 * memory which is returned
 */
char *generateNewString(char* word);

/**
 * Function takes in a file pointer and calculates the relative term
 * frequency adjusted for the document length of file f
 */
double calculateTf(char *filename, char *term);


#endif
