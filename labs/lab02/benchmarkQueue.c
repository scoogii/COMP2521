
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

int main(void) {
    Queue q = QueueNew();

    // Write a benchmark test to demonstrate the difference between
    // ArrayQueue and CircularArrayQueue

    // Aim:
    // -> expand 3 times
    // -> fill queue
    // -> dequeue a couple times (has to loop to end)

    // Enqueue a bunch of items
    // Times for both programs shouldn't be too diff here
    for (int i = 1; i < 100000; i++) { QueueEnqueue(q, i); }

    // Dequeue a bunch of items
    // Times should be VERY different here
    for (int j = 1; j < 100000; j++) { QueueDequeue(q); }

    QueueFree(q);
}
