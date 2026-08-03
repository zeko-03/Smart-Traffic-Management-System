#ifndef LINKEDLIST_H
#define LINKEDLIST_H

class LinkedList {
private:
    struct Node {
        void* data;
        Node* next;
    };

    Node* head;
    int count;

public:
    LinkedList();
    ~LinkedList();

    bool isEmpty() const;
    int size() const;

    void insertLast(void* item);
    void* removeFirst();
    void* getAt(int index) const;
    void* removeByPtr(void* item);
};

#endif
