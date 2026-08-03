#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "LinkedList.h"

typedef bool (*PQCompareFn)(void*, void*);

class PriorityQueue {
private:
    struct Node {
        void* data;
        Node* next;
    };

    Node* head;
    PQCompareFn cmp;

    static bool defaultCompare(void* a, void* b);

public:
    PriorityQueue();
    PriorityQueue(PQCompareFn compare);
    ~PriorityQueue();

    bool isEmpty() const;
    void enqueue(void* item);
    void* dequeue();
    void* peek() const;
};

#endif
