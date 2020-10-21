// Normalising words
// Christian Nguyen

// Removing leading and trailing whitespaces
// Converting all characters to lowercase
// Remove the following punctuation marks
// ONCE if they appear at the end of the word
/**
 * '.'
 * ','
 * ';'
 * '?'
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *normaliseWord(char *word);

int main(void) {
    char word[] = "     ICANTBREATHE????";

    printf("Your normalised word is:'%s'\n", normaliseWord(word));
}

char *normaliseWord(char *word) {
    // Make all characters lowercase
    for (char *currChar = word; *currChar; currChar++) {
        *currChar = tolower(*currChar);
    }

    // Remove leading whitespaces
    while (isspace(word[0])) word++;

    // Create pointer to last character of word
    char *lastChar = word + strlen(word) - 1;

    // Remove trailing whitespaces
    while (isspace((*lastChar))) lastChar--;

    // Remove ending punctuation mark
    if (*lastChar == '.' || *lastChar == ',' || *lastChar == ';' ||
        *lastChar == '?')
        lastChar--;

    // Adding new null terminator taken from Stack Overflow
    lastChar[1] = '\0';

    return word;
}
