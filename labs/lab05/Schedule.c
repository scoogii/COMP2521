// Schedule ADT implementation

#include <stdio.h>
#include <stdlib.h>

#include "Schedule.h"
#include "Time.h"
#include "Tree.h"

struct schedule {
    Tree times;
    int count;
};

// Creates a new schedule
Schedule ScheduleNew(void) {
    Schedule s = malloc(sizeof(*s));
    if (s == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    s->times = TreeNew();
    s->count = 0;
    return s;
}

// Frees all memory associated with a given schedule
void ScheduleFree(Schedule s) {
    TreeFree(s->times);
    free(s);
}

// Gets the number of times added to the schedule
int ScheduleCount(Schedule s) { return s->count; }

// Attempts to schedule a new landing time. Returns true if the time was
// successfully added, and false otherwise.
bool ScheduleAdd(Schedule s, Time t) {
    // If time is NULL, then cannot be added
    if (t == NULL) return false;

    Time timeFloor = TreeFloor(s->times, t);
    Time timeCeiling = TreeCeiling(s->times, t);

    // Lower and upper bounds for available times
    Time t1 = TimeSubtractMinutes(t, 10);
    Time t2 = TimeAddMinutes(t, 10);

    // If a floor exists, check that t1 (lower bound) is within floor
    if (timeFloor != NULL) {
        if (TimeCmp(t1, timeFloor) <= 0) return false;
    }

    // If a ceiling exists, check that t2 (upper bound) is within ceiling
    if (timeCeiling != NULL) {
        if (TimeCmp(t2, timeCeiling) >= 0) return false;
    }

    TreeInsert(s->times, t);
    s->count++;
    return true;
}

// Shows  all  the landing times in the schedule. If mode is 1, only the
// times are shown. If mode is 2, the underlying data structure is shown
// as well.
void ScheduleShow(Schedule s, int mode) {
    if (mode == 1) {
        TreeList(s->times);
    } else if (mode == 2) {
        TreeShow(s->times);
    }
}
