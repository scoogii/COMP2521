#include "Que.h"
#include <assert.h>
#include <stdio.h>

struct queue {
    Item *items;
    int size;
    int capacity;
    int frontIndex;
};

void printQueue(Queue q);
static void testQueue1(void);
void printEverything(Queue q);

int main(void) { testQueue1(); }

static void testQueue1(void) {
    Queue q = QueueNew();

    // enqueue 1 to 10
    for (int i = 1; i <= 10; i++) {
        QueueEnqueue(q, i);
        assert(QueueSize(q) == i);
    }

    // dequeue 1 to 5
    for (int j = 1; j <= 5; j++) {
        assert(QueueFront(q) == j);
        assert(QueueDequeue(q) == j);
    }
    assert(QueueSize(q) == 5);

    // enqueue 11 to 20, dequeue 6 to 15
    for (int i = 11, j = 6; i <= 20; i++, j++) {
        QueueEnqueue(q, i);
        assert(QueueFront(q) == j);
        assert(QueueDequeue(q) == j);
    }
    assert(QueueSize(q) == 5);

    // dequeue 16 to 20
    for (int j = 16; j <= 20; j++) {
        printf("j is %d\n", j);
        printf("The mutha fuckin index is %d\n", q->frontIndex);
        printf("QueueFront is %d\n", QueueFront(q));
        assert(QueueFront(q) == j);
        assert(QueueDequeue(q) == j);
    }
    assert(QueueSize(q) == 0);
    assert(QueueIsEmpty(q));

    // enqueue a number
    QueueEnqueue(q, 21);
    assert(QueueFront(q) == 21);

    QueueFree(q);
}
