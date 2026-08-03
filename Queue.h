#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"

class Queue {
private:
    LinkedList list;

public:
    Queue();
    bool isEmpty() const;
    int size() const;
    void enqueue(void* item);
    void* dequeue();
    void* peek() const;
};

#endif
