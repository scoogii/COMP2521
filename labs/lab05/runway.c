// runway.c - a command-line interface to the Schedule ADT

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Schedule.h"
#include "Time.h"

#define CURRENT_YEAR 2020
#define MAX 8192

static void processOptions(int argc, char *argv[]);
static void showUsage(char *progName);
static void showWelcomeMessage(void);
static int getCommand(char *buf);
static char **tokenise(char *s, int *ntokens);
static void showHelp(void);

static void runAdd(Schedule s, char **tokens);
static void runCount(Schedule s, char **tokens);
static void runShow(Schedule s, char **tokens);

static bool validateAdd(char **tokens);
static bool validateShow(char **tokens);

////////////////////////////////////////////////////////////////////////
// Commands

typedef struct command {
    char  *code;
    int    numArgs;
    void (*fn)(Schedule, char **); // function that executes the command
    bool (*validateArgs)(char **); // some argument checking
    char  *argHint;
    char  *helpMsg;
} Command;

#define NUM_COMMANDS 5
static Command COMMANDS[NUM_COMMANDS] = {
    {"+", 3, runAdd,   validateAdd,  "<month> <day> <hhmm>",
                                     "add a new landing time to the schedule"},
    {"n", 0, runCount, NULL,         "",
                                     "get the number of times in the schedule"},
    {"s", 1, runShow,  validateShow, "<mode (1 or 2)>",
                                     "show the schedule"},

    // Meta-commands
    {"?", 0, NULL,     NULL,         "", "show this message"},
    {"q", 0, NULL,     NULL,         "", "quit"},
};

////////////////////////////////////////////////////////////////////////

bool ECHO = false;

int main(int argc, char *argv[]) {
    processOptions(argc, argv);
    showWelcomeMessage();

    Schedule s = ScheduleNew();

    bool done = false;
    char cmd[MAX] = {0};

    while (!done && getCommand(cmd)) {
        if (ECHO) {
            printf("%s", cmd);
        }

        int ntokens = 0;
        char **tokens = tokenise(cmd, &ntokens);
        char *cmd = tokens[0];

        // Meta-commands
        if (strcmp(cmd, "?") == 0) {
            showHelp();
        } else if (strcmp(cmd, "q") == 0) {
            done = true;
        
        // Actual commands
        } else {
            bool validCommand = false;

            for (int i = 0; i < NUM_COMMANDS; i++) {
                if (strcmp(cmd, COMMANDS[i].code) == 0) {
                    validCommand = true;
                    if (ntokens - 1 == COMMANDS[i].numArgs &&
                            (COMMANDS[i].validateArgs == NULL ||
                            COMMANDS[i].validateArgs(tokens))) {
                        COMMANDS[i].fn(s, tokens);
                    } else {
                        printf("Usage: %s %s\n", COMMANDS[i].code,
                                                 COMMANDS[i].argHint);
                    }
                }
            }

            if (!validCommand) {
                printf("Unknown command '%s'\n", cmd);
            }
        }
        free(tokens);
    }

    ScheduleFree(s);
}

static void processOptions(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            showUsage(argv[0]);
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "-e") == 0) {
            ECHO = true;
        }
    }
}

static void showUsage(char *progName) {
    printf("Usage: %s [options]...\n"
           "Options:\n"
           "    -h      show this help message\n"
           "    -e      echo - echo all commands\n",
           progName);
}

static void showWelcomeMessage(void) {
    printf("Runway Landing Scheduler v1.0\n");
    printf("Enter ? to see the list of commands.\n");
}

static int getCommand(char *buf) {
    printf("> ");
	return (fgets(buf, MAX, stdin) != NULL);
}

static char **tokenise(char *s, int *ntokens) {
    char p;

    // count number of tokens
    *ntokens = 0;
    p = ' ';
    for (char *c = s; *c != '\0'; p = *c, c++) {
        if (p == ' ' && *c != ' ') {
            (*ntokens)++;
        }
    }

    char **tokens = malloc((*ntokens + 1) * sizeof(char *));
    int i = 0;
    p = ' ';
    for (char *c = s; *c != '\0'; p = *c, c++) {
        if ((p == '\0' || p == ' ') && *c != ' ') {
            tokens[i++] = c;
        } else if (p != ' ' && (*c == ' ' || *c == '\n')) {
            *c = '\0';
        }
    }
    tokens[i] = NULL;
    
    return tokens;
}

static void showHelp(void) {
    printf("Commands:\n");
    for (int i = 0; i < NUM_COMMANDS; i++) {
        printf("%5s %-24s %s\n", COMMANDS[i].code, COMMANDS[i].argHint,
                                 COMMANDS[i].helpMsg);
    }
    printf("\n");
}

////////////////////////////////////////////////////////////////////////

static void runAdd(Schedule s, char **tokens) {
    int month, day, time;
    sscanf(tokens[1], "%d", &month);
    sscanf(tokens[2], "%d", &day);
    sscanf(tokens[3], "%d", &time);
    Time t = TimeNew(CURRENT_YEAR, month, day, time);
    if (t == NULL) {
        printf("Invalid time!\n");
    } else {
        bool success = ScheduleAdd(s, t);
        printf("%s ", success ? "Successfully added" : "Couldn't add");
        TimeShow(t);
        printf(" to the schedule!\n");
        TimeFree(t);
    }
}

static void runCount(Schedule s, char **tokens) {
    int count = ScheduleCount(s);
    printf("%d landing time%s %s been added to the schedule.\n",
           count, count == 1 ? "" : "s", count == 1 ? "has" : "have");
}

static void runShow(Schedule s, char **tokens) {
    int mode;
    sscanf(tokens[1], "%d", &mode);
    ScheduleShow(s, mode);
}

static bool validateAdd(char **tokens) {
    char str[MAX];
    sprintf(str, "%s %s %s 0", tokens[1], tokens[2], tokens[3]);
    int month, day, time, tmp;
    return sscanf(str, "%d %d %d %d", &month, &day, &time, &tmp) == 4;
}

static bool validateShow(char **tokens) {
    char str[MAX];
    sprintf(str, "%s 0", tokens[1]);
    int mode, tmp;
    if (sscanf(str, "%d %d", &mode, &tmp) == 2) {
        return mode == 1 || mode == 2;
    } else {
        return false;
    }
}
