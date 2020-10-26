#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 101

char *normaliseWord(char *str);

int main(void) {
    char string[MAX_LEN] = "    ";
    /*char *string = "   HELLO??? ";*/
    printf("String before: '%s'\n", string);
    printf("String after: '%s'\n", normaliseWord(string));

}


/**
 * Normalises a given string. See the spec for details. Note: you should
 * modify the given string - do not create a copy of it.
 */
char *normaliseWord(char *str) {
    // Make all characters lowercase
    for (char *currChar = str; *currChar; currChar++) { *currChar = tolower(*currChar); }

    // Remove leading whitespaces - shift chars to the left
    while (isspace(str[0])) { for (int i = 0; i < (int)strlen(str); i++) str[i] = str[i + 1]; }

    // Remove trailing whitespaces - truncate from end of the string
    char *lastChar = str + strlen(str) - 1;
    while (isspace((*lastChar))) lastChar--;

    // Remove ending punctuation mark
    if (*lastChar == '.' || *lastChar == ',' || *lastChar == ';' || *lastChar == '?') lastChar--;

    // Adding new null terminator taken from Stack Overflow
    lastChar[1] = '\0';

    return str;
}
