// seqq.c ... print sequences of integers
// Written by John Shepherd, July 2015

#include <stdio.h>
#include <stdlib.h>

void seq(int, int, int);
void checkStep(int, int, int);
int getValue(char **, int, char *);

int main(int argc, char **argv) {
  int start, step, finish;

  // process command-line args
  switch (argc) {
  case 2:
    start = 1;
    finish = getValue(argv, 1, "finish");
    step = (start < finish) ? 1 : -1;
    break;
  case 3:
    start = getValue(argv, 1, "start");
    finish = getValue(argv, 2, "finish");
    step = (start < finish) ? 1 : -1;
    break;
  case 4:
    start = getValue(argv, 1, "start");
    step = getValue(argv, 2, "step");
    finish = getValue(argv, 3, "finish");
    checkStep(start, step, finish);
    break;
  default:
    fprintf(stderr, "Usage: %s [start] [step] [finish]\n", argv[0]);
    exit(EXIT_FAILURE);
    break;
  }

  // got valid (start,step,finish)
  seq(start, step, finish);

  return EXIT_SUCCESS;
}

void seq(int start, int step, int finish) {
  int i;

  int seqIncreasing = (start < finish);

  for (i = start; (seqIncreasing ? i <= finish : i >= finish); i += step) {
    printf("%d ", i);
  }
  printf("\n");
}

int getValue(char **argv, int which, char *name) {
  int value;
  if (sscanf(argv[which], "%d", &value) != 1) {
    fprintf(stderr, "Invalid %s\n", name);
    exit(EXIT_FAILURE);
  }
  return value;
}

void checkStep(int start, int step, int finish) {
  int ok = 1;
  if (step == 0) {
    ok = 0;
  } else if (start < finish && step < 0) {
    ok = 0;
  } else if (start > finish && step > 0) {
    ok = 0;
  }
  if (!ok) {
    fprintf(stderr, "Invalid step %d\n", step);
    exit(EXIT_FAILURE);
  }
}
