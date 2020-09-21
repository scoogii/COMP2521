// Implementation of the Queue ADT using a circular array

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

#define DEFAULT_SIZE 16  // DO NOT change this line

// DO NOT modify this struct
struct queue {
    Item *items;
    int size;
    int capacity;
    int frontIndex;
};

// Function prototype
void extendQueue(Queue q, Item it, int backIndex);

/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
    Queue q = malloc(sizeof(*q));
    if (q == NULL) {
        fprintf(stderr, "couldn't allocate Queue");
        exit(EXIT_FAILURE);
    }

    q->items = malloc(DEFAULT_SIZE * sizeof(Item));
    if (q->items == NULL) {
        fprintf(stderr, "couldn't allocate Queue");
        exit(EXIT_FAILURE);
    }

    q->size = 0;
    q->capacity = DEFAULT_SIZE;
    q->frontIndex = 0;
    return q;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
    free(q->items);
    free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it) {
    // If queue is empty
    if (QueueIsEmpty(q)) {
        q->items[q->frontIndex] = it;
        q->size++;
        return;
    }

    // Calculate backIndex - this ensures it wraps around array
    int backIndex = (q->frontIndex + (q->size - 1)) % q->capacity;

    // 2 cases: if queue is not full or full
    if (q->size < q->capacity) {  // Case 1 - not full
        q->items[(backIndex + 1) % q->capacity] = it;
        q->size++;
    } else {  // Case 2 - full
        if (q->frontIndex > backIndex) q->frontIndex += q->capacity;
        extendQueue(q, it, backIndex);
        q->capacity *= 2;
    }
}

/**
 * Function that shifts all values from index 'frontIndex' to
 * the very right once the full queue is expanded
 * Function returns the new capacity size
 */
void extendQueue(Queue q, Item it, int backIndex) {
    // Double the size of the previous array using realloc
    q->items = realloc(q->items, (q->capacity * 2) * sizeof(Item));

    // If front > back, need to shift queue items to the very end of the new
    // queue
    if (q->frontIndex > backIndex) {
        // Loop through frontIndex to end of queue and shift them
        // in the extended queue
        for (int i = (q->capacity - 1); i >= q->frontIndex; i--) {
            q->items[i + q->capacity] = q->items[i];
        }
    }

    // Insert new item at the back of the queue
    q->items[backIndex + 1] = it;
    q->size++;
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
Item QueueDequeue(Queue q) {
    // Store initial front item and reduce size
    Item front = QueueFront(q);
    q->size--;

    // Adjust front index and return deq'd item
    q->frontIndex = (q->frontIndex + 1) % q->capacity;
    return front;
}

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 */
Item QueueFront(Queue q) {
    assert(q->size > 0);

    return q->items[q->frontIndex];
}

/**
 * Gets the size of the given queue
 */
int QueueSize(Queue q) { return q->size; }

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q) { return q->size == 0; }

/**
 * Prints the queue to the given file with items space-separated
 */
void QueueDump(Queue q, FILE *fp) {
    for (int i = q->frontIndex, j = 0; j < q->size;
            i = (i + 1) % q->capacity, j++) {
        fprintf(fp, "%d ", q->items[i]);
    }
}
