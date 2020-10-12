 // Interface for the Schedule ADT

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "Time.h"

#include <stdbool.h>

typedef struct schedule *Schedule;

// Creates a new schedule
Schedule ScheduleNew(void);

// Frees all memory associated with a given schedule
void ScheduleFree(Schedule s);

// Gets the number of times added to the schedule
int  ScheduleCount(Schedule s);

// Attempts to schedule a new landing time. Returns true if the time was
// successfully added, and false otherwise.
bool ScheduleAdd(Schedule s, Time t);

// Shows  all  the landing times in the schedule. If mode is 1, only the
// times are shown. If mode is 2, the underlying data structure is shown
// as well.
void ScheduleShow(Schedule s, int mode);

#endif
