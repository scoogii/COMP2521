// Test program for tree operations

#include <stdio.h>
#include <stdlib.h>

#include "Time.h"
#include "Tree.h"

static void usage(char *progname);

static void testRotateLeft(void);
static void testRotateRight(void);
static void testDoInsert(void);
static void testTreeFloor(void);
static void testTreeCeiling(void);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage(argv[0]);
    }

    int choice = atoi(argv[1]);
    switch (choice) {
        case 1:  testRotateLeft();  break;
        case 2:  testRotateRight(); break;
        case 3:  testDoInsert();    break;
        case 4:  testTreeFloor();   break;
        case 5:  testTreeCeiling(); break;
        default: usage(argv[0]);    break;
    }
}

static void usage(char *progname) {
    printf("Usage: %s <1-5>\n", progname);
    exit(EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////////////

static void doTestRotateLeft(Time *ts, int nRotates);
static void rotateLeftAndShow(Tree t, int nRotates);
static void doTestRotateRight(Time *ts, int nRotates);
static void rotateRightAndShow(Tree t, int nRotates);
static void doTestDoInsert(Time *ts);
static void doTestTreeFloor(Time *ts, Time *queries);
static void doTestTreeCeiling(Time *ts, Time *queries);

static Tree makeTreeFromTimes(Time *ts);
static void freeTimes(Time *ts);

static void testRotateLeft(void) {
    // rotate an empty tree
    printf("\nTest 1\n\n");
    Time ts1[] = {
        NULL
    };
    doTestRotateLeft(ts1, 1);
    freeTimes(ts1);

    // rotate a tree with no right child
    printf("\nTest 2\n\n");
    Time ts2[] = {
        TimeNew(2020,  3, 15, 1500),
        TimeNew(2020,  3, 15, 1300),
        TimeNew(2020,  3, 15, 1200),
        TimeNew(2020,  3, 15, 1400),
        NULL
    };
    doTestRotateLeft(ts2, 1);
    freeTimes(ts2);

    // rotate a balanced tree
    printf("\nTest 3\n\n");
    Time ts3[] = {
        TimeNew(2020,  3, 15, 1200),
        TimeNew(2020,  3, 13, 1200),
        TimeNew(2020,  3, 12, 1200),
        TimeNew(2020,  3, 14, 1200),
        TimeNew(2020,  3, 17, 1200),
        TimeNew(2020,  3, 16, 1200),
        TimeNew(2020,  3, 18, 1200),
        NULL
    };
    doTestRotateLeft(ts3, 3);
    freeTimes(ts3);
    
    // rotate another random tree
    printf("\nTest 4\n\n");
    Time ts4[] = {
        TimeNew(2020,  1,  1, 1200),
        TimeNew(2020,  4,  1, 1200),
        TimeNew(2020,  3,  1, 1200),
        TimeNew(2020,  2,  1, 1200),
        TimeNew(2020,  7,  1, 1200),
        TimeNew(2020,  5,  1, 1200),
        TimeNew(2020,  6,  1, 1200),
        TimeNew(2020,  9,  1, 1200),
        TimeNew(2020,  8,  1, 1200),
        NULL
    };
    doTestRotateLeft(ts4, 4);
    freeTimes(ts4);
}

static void doTestRotateLeft(Time *ts, int nRotates) {
    Tree t = makeTreeFromTimes(ts);
    rotateLeftAndShow(t, nRotates);
    TreeFree(t);
}

static void rotateLeftAndShow(Tree t, int nRotates) {
    TreeShow(t);
    for (int i = 0; i < nRotates; i++) {
        printf("\nRotating left at the root...\n\n");
        TreeRotateLeftAtRoot(t);
        TreeShow(t);
    }
}

////////////////////////////////////////////////////////////////////////

static void testRotateRight(void) {
    // rotate an empty tree
    printf("\nTest 1\n\n");
    Time ts1[] = {
        NULL
    };
    doTestRotateRight(ts1, 1);
    freeTimes(ts1);

    // rotate a tree with no left child
    printf("\nTest 2\n\n");
    Time ts2[] = {
        TimeNew(2020,  3, 15, 1200),
        TimeNew(2020,  3, 15, 1400),
        TimeNew(2020,  3, 15, 1300),
        TimeNew(2020,  3, 15, 1500),
        NULL
    };
    doTestRotateRight(ts2, 1);
    freeTimes(ts2);

    // rotate a balanced tree
    printf("\nTest 3\n\n");
    Time ts3[] = {
        TimeNew(2020,  3, 15, 1200),
        TimeNew(2020,  3, 13, 1200),
        TimeNew(2020,  3, 12, 1200),
        TimeNew(2020,  3, 14, 1200),
        TimeNew(2020,  3, 17, 1200),
        TimeNew(2020,  3, 16, 1200),
        TimeNew(2020,  3, 18, 1200),
        NULL
    };
    doTestRotateRight(ts3, 3);
    freeTimes(ts3);
    
    // rotate another random tree
    printf("\nTest 4\n\n");
    Time ts4[] = {
        TimeNew(2020,  9,  1, 1200),
        TimeNew(2020,  6,  1, 1200),
        TimeNew(2020,  3,  1, 1200),
        TimeNew(2020,  1,  1, 1200),
        TimeNew(2020,  2,  1, 1200),
        TimeNew(2020,  5,  1, 1200),
        TimeNew(2020,  4,  1, 1200),
        TimeNew(2020,  7,  1, 1200),
        TimeNew(2020,  8,  1, 1200),
        NULL
    };
    doTestRotateRight(ts4, 4);
    freeTimes(ts4);
}

static void doTestRotateRight(Time *ts, int nRotates) {
    Tree t = makeTreeFromTimes(ts);
    rotateRightAndShow(t, nRotates);
    TreeFree(t);
}

static void rotateRightAndShow(Tree t, int nRotates) {
    TreeShow(t);
    for (int i = 0; i < nRotates; i++) {
        printf("\nRotating right at the root...\n\n");
        TreeRotateRightAtRoot(t);
        TreeShow(t);
    }
}

////////////////////////////////////////////////////////////////////////

void testDoInsert(void) {
    printf("\nTest 1\n");
    Time ts1[] = {
        TimeNew(2020,  6,  1, 1200),
        TimeNew(2020,  3,  1, 1200),
        TimeNew(2020,  2,  1, 1200),
        NULL
    };
    doTestDoInsert(ts1);
    freeTimes(ts1);

    printf("\nTest 2\n");
    Time ts2[] = {
        TimeNew(2020,  7,  1, 1200),
        TimeNew(2020,  1,  1, 1200),
        TimeNew(2020,  3,  1, 1200),
        NULL
    };
    doTestDoInsert(ts2);
    freeTimes(ts2);

    printf("\nTest 3\n");
    Time ts3[] = {
        TimeNew(2020,  4,  1, 1200),
        TimeNew(2020,  8,  1, 1200),
        TimeNew(2020,  5,  1, 1200),
        NULL
    };
    doTestDoInsert(ts3);
    freeTimes(ts3);

    printf("\nTest 4\n");
    Time ts4[] = {
        TimeNew(2020,  1,  1, 1200),
        TimeNew(2020,  5,  1, 1200),
        TimeNew(2020,  9,  1, 1200),
        NULL
    };
    doTestDoInsert(ts4);
    freeTimes(ts4);

    printf("\nTest 5\n");
    Time ts5[] = {
        TimeNew(2020,  5,  1, 1200),
        TimeNew(2020,  4,  1, 1200),
        TimeNew(2020,  2,  1, 1200),
        TimeNew(2020,  9,  1, 1200),
        TimeNew(2020,  7,  1, 1200),
        TimeNew(2020,  6,  1, 1200),
        TimeNew(2020,  3,  1, 1200),
        TimeNew(2020, 10,  1, 1200),
        TimeNew(2020, 12,  1, 1200),
        NULL
    };
    doTestDoInsert(ts5);
    freeTimes(ts5);

    printf("\nTest 6\n");
    Time ts6[] = {
        TimeNew(2020,  7,  1, 1200),
        TimeNew(2020,  5,  1, 1200),
        TimeNew(2020,  9,  1, 1200),
        TimeNew(2020,  3,  1, 1200),
        TimeNew(2020,  6,  1, 1200),
        TimeNew(2020,  8,  1, 1200),
        TimeNew(2020,  2,  1, 1200),
        TimeNew(2020,  4,  1, 1200),
        TimeNew(2020,  5, 15, 1200),
        TimeNew(2020,  6, 15, 1200),
        TimeNew(2020,  8, 15, 1200),
        TimeNew(2020,  5, 10, 1200),
        NULL
    };
    doTestDoInsert(ts6);
    freeTimes(ts6);
}

static void doTestDoInsert(Time *ts) {
    Tree t = TreeNew();
    TreeShow(t);
    for (int i = 0; ts[i] != NULL; i++) {
        printf("\nInserting ");
        TimeShow(ts[i]);
        printf("\n\n");
        TreeInsert(t, ts[i]);
        TreeShow(t);
    }
    TreeFree(t);
}

////////////////////////////////////////////////////////////////////////

void testTreeFloor(void) {
    printf("\nTest 1\n\n");
    Time ts1[] = {
        NULL
    };
    Time qs1[] = {
        TimeNew(2020,  7,  2, 1200),
        NULL
    };
    doTestTreeFloor(ts1, qs1);
    freeTimes(ts1);
    freeTimes(qs1);

    printf("\nTest 2\n\n");
    Time ts2[] = {
        TimeNew(2020,  4, 14, 1200),
        TimeNew(2020,  2, 17, 1200),
        TimeNew(2020,  1,  4, 1200),
        TimeNew(2020,  3, 29, 1200),
        TimeNew(2020,  6,  6, 1200),
        TimeNew(2020,  5, 19, 1200),
        TimeNew(2020,  7,  7, 1200),
        NULL
    };
    Time qs2[] = {
        TimeNew(2020,  1,  1, 1200),
        TimeNew(2020,  1,  9, 1200),
        TimeNew(2020,  3, 15, 1200),
        TimeNew(2020,  4,  1, 1200),
        TimeNew(2020,  4, 27, 1200),
        TimeNew(2020,  5, 30, 1200),
        TimeNew(2020,  6, 22, 1200),
        TimeNew(2020,  7, 12, 1200),
        NULL
    };
    doTestTreeFloor(ts2, qs2);
    freeTimes(ts2);
    freeTimes(qs2);
}

static void doTestTreeFloor(Time *ts, Time *queries) {
    Tree t = makeTreeFromTimes(ts);
    TreeShow(t);
    printf("\n");
    for (int i = 0; queries[i] != NULL; i++) {
        printf("The floor of ");
        TimeShow(queries[i]);
        printf(" is ");
        Time floor = TreeFloor(t, queries[i]);
        if (floor == NULL) {
            printf("NULL");
        } else {
            TimeShow(floor);
        }
        printf("\n");
    }
    TreeFree(t);
}

////////////////////////////////////////////////////////////////////////

void testTreeCeiling(void) {
    printf("\nTest 1\n\n");
    Time ts1[] = {
        NULL
    };
    Time qs1[] = {
        TimeNew(2020,  7,  2, 1200),
        NULL
    };
    doTestTreeCeiling(ts1, qs1);
    freeTimes(ts1);
    freeTimes(qs1);

    printf("\nTest 2\n\n");
    Time ts2[] = {
        TimeNew(2020,  4, 14, 1200),
        TimeNew(2020,  2, 17, 1200),
        TimeNew(2020,  1,  4, 1200),
        TimeNew(2020,  3, 29, 1200),
        TimeNew(2020,  6,  6, 1200),
        TimeNew(2020,  5, 19, 1200),
        TimeNew(2020,  7,  7, 1200),
        NULL
    };
    Time qs2[] = {
        TimeNew(2020,  1,  1, 1200),
        TimeNew(2020,  1,  9, 1200),
        TimeNew(2020,  3, 15, 1200),
        TimeNew(2020,  4,  1, 1200),
        TimeNew(2020,  4, 27, 1200),
        TimeNew(2020,  5, 30, 1200),
        TimeNew(2020,  6, 22, 1200),
        TimeNew(2020,  7, 12, 1200),
        NULL
    };
    doTestTreeCeiling(ts2, qs2);
    freeTimes(ts2);
    freeTimes(qs2);
}

static void doTestTreeCeiling(Time *ts, Time *queries) {
    Tree t = makeTreeFromTimes(ts);
    TreeShow(t);
    printf("\n");
    for (int i = 0; queries[i] != NULL; i++) {
        printf("The ceiling of ");
        TimeShow(queries[i]);
        printf(" is ");
        Time ceiling = TreeCeiling(t, queries[i]);
        if (ceiling == NULL) {
            printf("NULL");
        } else {
            TimeShow(ceiling);
        }
        printf("\n");
    }
    TreeFree(t);
}

////////////////////////////////////////////////////////////////////////

static Tree makeTreeFromTimes(Time *ts) {
    Tree t = TreeNew();
    for (int i = 0; ts[i] != NULL; i++) {
        TreeInsertLeaf(t, ts[i]);
    }
    return t;
}

static void freeTimes(Time *ts) {
    for (int i = 0; ts[i] != NULL; i++) {
        TimeFree(ts[i]);
    }
}
