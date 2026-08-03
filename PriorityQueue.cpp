#include "PriorityQueue.h"
#include "Vehicle.h"

bool PriorityQueue::defaultCompare(void* a, void* b) {
    if (a == nullptr || b == nullptr) return false;
    Vehicle* va = (Vehicle*)a;
    Vehicle* vb = (Vehicle*)b;
    if (va->getUrgency() == vb->getUrgency()) return false;
    return va->getUrgency() > vb->getUrgency();
}

PriorityQueue::PriorityQueue() {
    head = nullptr;
    cmp = defaultCompare;
}

PriorityQueue::PriorityQueue(PQCompareFn compare) {
    head = nullptr;
    cmp = compare ? compare : defaultCompare;
}

PriorityQueue::~PriorityQueue() {
    Node* cur = head;
    while (cur != nullptr) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

bool PriorityQueue::isEmpty() const {
    return head == nullptr;
}

void PriorityQueue::enqueue(void* item) {
    Node* n = new Node();
    n->data = item;
    n->next = nullptr;
    if (head == nullptr || (cmp && cmp(item, head->data))) {
        n->next = head;
        head = n;
        return;
    }
    Node* cur = head;
    while (cur->next != nullptr && !(cmp && cmp(item, cur->next->data)))
        cur = cur->next;
    n->next = cur->next;
    cur->next = n;
}

void* PriorityQueue::dequeue() {
    if (head == nullptr) return nullptr;
    Node* n = head;
    void* d = n->data;
    head = head->next;
    delete n;
    return d;
}

void* PriorityQueue::peek() const {
    if (head == nullptr) return nullptr;
    return head->data;
}
