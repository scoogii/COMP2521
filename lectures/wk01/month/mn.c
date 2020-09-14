// mn.c ... convert month ordinal to name
// Written by John Shepherd, July 2015
// This version shows "typical" formatting

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
	char * monthName;

	switch (monthNum) {
	case 1: monthName = "January"; break;
	case 2: monthName = "February"; break;
	case 3: monthName = "March"; break;
	case 4: monthName = "April"; break;
	case 5: monthName = "May"; break;
	case 6: monthName = "June"; break;
	case 7: monthName = "July"; break;
	case 8: monthName = "August"; break;
	case 9: monthName = "September"; break;
	case 10: monthName = "October"; break;
	case 11: monthName = "February"; break;
	case 12: monthName = "February"; break;
	default: monthName = "????"; break;
	}
	return monthName;
}

// pre: defined(string,cmdName)
// post: program has terminated
void giveUp(char *cmdName)
{
	fprintf(stderr, "Usage: %s 1..12\n", cmdName);
	exit(EXIT_FAILURE);
}
