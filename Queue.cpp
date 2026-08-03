#include "Queue.h"

Queue::Queue() {}

bool Queue::isEmpty() const {
    return list.isEmpty();
}

int Queue::size() const {
    return list.size();
}

void Queue::enqueue(void* item) {
    list.insertLast(item);
}

void* Queue::dequeue() {
    return list.removeFirst();
}

void* Queue::peek() const {
    return list.getAt(0);
}
