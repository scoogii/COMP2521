// Interface for the Time ADT

#ifndef TIME_H
#define TIME_H

typedef struct time *Time;

// Creates  a  new  time out of the given arguments. Returns NULL if the
// arguments do not describe a valid time.
Time TimeNew(int year, int month, int day, int hhmm);

// Creates a copy of a given time
Time TimeCopy(Time t);

// Frees all memory associated with a given time
void TimeFree(Time t);

// Compares two times. Returns a negative number if the  first  time  is
// earlier  than  the  second  time,  0 if the times are the same, and a
// positive number if the first time is later than the second.
int  TimeCmp(Time t1, Time t2);

// Returns  a  new  time  that  is numMinutes later than the given time.
// numMinutes must be between 0 and 59.
Time TimeAddMinutes(Time t, int numMinutes);

// Returns  a  new  time that is numMinutes earlier than the given time.
// numMinutes must be between 0 and 59.
Time TimeSubtractMinutes(Time t, int numMinutes);

// Prints a time in the form 'MMM DD hh:mm'
void TimeShow(Time t);

#endif
