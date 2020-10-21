////////////////////////////////////////////////////////////////////////
// 					    COMP2521 Assignment 1                         //
//					    Information Retrieval                         //
//                     Main Program for Testing                       //
//                                                                    //
//				     Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "invertedIndex.h"

int main(void) {
    char string[] = " HELLO?   ";

    printf("Your normalised string is '%s'\n", normaliseWord(string));

    return 0;
}
