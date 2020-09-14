// mn.c ... convert month ordinal to name
// Written by John Shepherd, July 2015
// This version shows "compact" formatting

#include <stdlib.h>
#include <stdio.h>

void giveUp(char *);
char *monthName(int);

int main(int argc, char **argv)
{
	// argument processing
	if (argc < 2) giveUp(argv[0]);
	int n = atoi(argv[1]);
	if (n < 1 || n > 12) giveUp(argv[0]);

	// compute and print name
	printf("%s\n", monthName(n));

	exit(EXIT_SUCCESS);
}

// pre: defined(integer,monthNum)
// post: ret == ((1 <= monthNum <= 12) ? nameOfMonth[monthNum] : "????")
char *monthName(int monthNum)
{
	char *monthNames[13] = {
		"????",  "January", "February", "March",
		"April", "May", "June", "July", "August",
		"September", "October", "November", "December"
	};

	if (monthNum >= 1 && monthNum <= 12)
		return monthNames[monthNum];
	else
		return monthNames[0];
}

// pre: defined(string,cmdName)
// post: program has terminated
void giveUp(char *cmdName)
{
	fprintf(stderr, "Usage: %s 1..12\n", cmdName);
	exit(EXIT_FAILURE);
}
