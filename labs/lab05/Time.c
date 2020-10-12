// Implementation of the Time ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Time.h"

struct time {
    int year;
    int month;
    int day;
    int hour;
    int minute;
};

static bool validTime(int year, int month, int day, int hhmm);
static int daysInMonth(int month, int year);
static bool isLeapYear(int year);

////////////////////////////////////////////////////////////////////////
// Constructor and Destructor

Time TimeNew(int year, int month, int day, int hhmm) {
    if (!validTime(year, month, day, hhmm)) {
        return NULL;
    }

    Time t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    t->year = year;
    t->month = month;
    t->day = day;
    t->hour = hhmm / 100;
    t->minute = hhmm % 100;
    return t;
}

static bool validTime(int year, int month, int day, int hhmm) {
    int hour = hhmm / 100;
    int minute = hhmm % 100;
    return (year >= 0) &&
           (month >= 1 && month <= 12) &&
           (day >= 1 && day <= daysInMonth(month, year)) &&
           (hhmm >= 0 && hhmm <= 2359) &&
           (hour >= 0 && hour <= 23) &&
           (minute >= 0 && minute <= 59);
}

static int daysInMonth(int month, int year) {
    int days[] = {0, 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    } else {
        return days[month];
    }
}

static bool isLeapYear(int year) {
    if (year % 400 == 0) {
        return true;
    } else if (year % 100 == 0) {
        return false;
    } else if (year % 4 == 0) {
        return true;
    } else {
        return false;
    }
}

Time TimeCopy(Time t) {
    Time new = malloc(sizeof(*new));
    if (new == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    new->year = t->year;
    new->month = t->month;
    new->day = t->day;
    new->hour = t->hour;
    new->minute = t->minute;
    return new;
}

void TimeFree(Time t) {
    free(t);
}

////////////////////////////////////////////////////////////////////////
// Time Comparison

int  TimeCmp(Time t1, Time t2) {
    if (t1->year != t2->year) {
        return t1->year - t2->year;
    }
    if (t1->month != t2->month) {
        return t1->month - t2->month;
    }
    if (t1->day != t2->day) {
        return t1->day - t2->day;
    }
    if (t1->hour != t2->hour) {
        return t1->hour - t2->hour;
    }
    return t1->minute - t2->minute;
}

////////////////////////////////////////////////////////////////////////
// Time Manipulation

Time TimeAddMinutes(Time t, int numMinutes) {
    assert(numMinutes >= 0 && numMinutes <= 59);

    int minute = t->minute;
    int hour = t->hour;
    int day = t->day;
    int month = t->month;
    int year = t->year;

    minute += numMinutes;
    if (minute >= 60) {
        minute -= 60;
        hour++;
    }
    if (hour >= 24) {
        hour -= 24;
        day++;
    }
    if (day > daysInMonth(month, year)) {
        day = 1;
        month++;
    }
    if (month > 12) {
        month = 1;
        year++;
    }

    return TimeNew(year, month, day, 100 * hour + minute);
}

Time TimeSubtractMinutes(Time t, int numMinutes) {
    assert(numMinutes >= 0 && numMinutes <= 59);

    int minute = t->minute;
    int hour = t->hour;
    int day = t->day;
    int month = t->month;
    int year = t->year;

    minute -= numMinutes;
    if (minute < 0) {
        minute += 60;
        hour--;
    }
    if (hour < 0) {
        hour += 24;
        day--;
    }
    if (day <= 0) {
        int newMonth = month == 1 ? 12 : month - 1;
        int newYear = month == 1 ? year - 1 : year;
        day = daysInMonth(newMonth, newYear);
        month--;
    }
    if (month <= 0) {
        month = 12;
        year--;
    }

    return TimeNew(year, month, day, 100 * hour + minute);
}

////////////////////////////////////////////////////////////////////////

void TimeShow(Time t) {
    char *months[] = {"???", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    printf("%s %02d %02d:%02d", months[t->month], t->day,
                                t->hour, t->minute);
}
