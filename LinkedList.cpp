#include "LinkedList.h"

LinkedList::LinkedList() {
    head = nullptr;
    count = 0;
}

LinkedList::~LinkedList() {
    Node* cur = head;
    while (cur != nullptr) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

bool LinkedList::isEmpty() const {
    return head == nullptr;
}

int LinkedList::size() const {
    return count;
}

void LinkedList::insertLast(void* item) {
    Node* n = new Node();
    n->data = item;
    n->next = nullptr;
    if (head == nullptr) {
        head = n;
    }
    else {
        Node* cur = head;
        while (cur->next != nullptr)
            cur = cur->next;
        cur->next = n;
    }
    count++;
}

void* LinkedList::removeFirst() {
    if (head == nullptr) return nullptr;
    Node* n = head;
    void* d = n->data;
    head = head->next;
    delete n;
    count--;
    return d;
}

void* LinkedList::getAt(int index) const {
    if (index < 0 || index >= count) return nullptr;
    Node* cur = head;
    int i = 0;
    while (i < index) {
        cur = cur->next;
        i++;
    }
    return cur->data;
}

void* LinkedList::removeByPtr(void* item) {
    if (head == nullptr) return nullptr;
    if (head->data == item) {
        Node* n = head;
        void* d = n->data;
        head = head->next;
        delete n;
        count--;
        return d;
    }
    Node* cur = head;
    while (cur->next != nullptr && cur->next->data != item)
        cur = cur->next;
    if (cur->next == nullptr) return nullptr;
    Node* n = cur->next;
    void* d = n->data;
    cur->next = n->next;
    delete n;
    count--;
    return d;
}
