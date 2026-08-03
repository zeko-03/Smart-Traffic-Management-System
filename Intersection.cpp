#include "Intersection.h"

Intersection::Intersection(int id_) {
    id = id_;
    initializeLanes();
}

Intersection::~Intersection() {
    int n = lanes.size();
    for (int i = 0; i < n; ++i) {
        void* p = lanes.removeFirst();
        if (i == 0) {
            delete (PriorityQueue*)p;
        }
        else {
            delete (Queue*)p;
        }
    }
}

void Intersection::initializeLanes() {
    while (!lanes.isEmpty()) lanes.removeFirst();
    lanes.insertLast((void*) new PriorityQueue());
    lanes.insertLast((void*) new Queue());
    lanes.insertLast((void*) new Queue());
    lanes.insertLast((void*) new Queue());
}

bool Intersection::enqueueVehicle(int lane, Vehicle* v) {
    if (lane < 0 || lane >= lanes.size()) return false;
    void* p = lanes.getAt(lane);
    if (p == nullptr) return false;
    if (lane == 0) {
        PriorityQueue* pq = (PriorityQueue*)p;
        pq->enqueue((void*)v);
    }
    else {
        Queue* q = (Queue*)p;
        q->enqueue((void*)v);
    }
    return true;
}

Vehicle* Intersection::dequeueFromLane(int lane) {
    if (lane < 0 || lane >= lanes.size()) return nullptr;
    void* p = lanes.getAt(lane);
    if (p == nullptr) return nullptr;
    if (lane == 0) {
        PriorityQueue* pq = (PriorityQueue*)p;
        return (Vehicle*)pq->dequeue();
    }
    else {
        Queue* q = (Queue*)p;
        return (Vehicle*)q->dequeue();
    }
}

Vehicle* Intersection::peekLane(int lane) {
    if (lane < 0 || lane >= lanes.size()) return nullptr;
    void* p = lanes.getAt(lane);
    if (p == nullptr) return nullptr;
    if (lane == 0) {
        PriorityQueue* pq = (PriorityQueue*)p;
        return (Vehicle*)pq->peek();
    }
    else {
        Queue* q = (Queue*)p;
        return (Vehicle*)q->peek();
    }
}

void Intersection::setActiveGreenLane(int lane) {
    light.setGreen(lane);
}
int Intersection::getActiveGreenLane() const {
    return light.getGreenLane();
}

void Intersection::update(int switchCost) {
    light.update(switchCost);
}

int Intersection::getLaneSize(int lane) const
{
    return 0;
}

Vehicle* Intersection::removeVehicleById(int vid) {
    if (lanes.size() <= 0) return nullptr;

    void* p0 = lanes.getAt(0);
    if (p0 != nullptr) {
        PriorityQueue* pq = (PriorityQueue*)p0;
        PriorityQueue temp;
        Vehicle* found = nullptr;
        while (!pq->isEmpty()) {
            Vehicle* v = (Vehicle*)pq->dequeue();
            if (v->getId() == vid) {
                found = v;
            }
            else {
                temp.enqueue((void*)v);
            }
        }
        while (!temp.isEmpty()) {
            pq->enqueue((void*)temp.dequeue());
        }
        if (found != nullptr) return found;
    }

    int n = lanes.size();
    for (int lane = 1; lane < n; ++lane) {
        void* p = lanes.getAt(lane);
        if (p == nullptr) continue;
        Queue* q = (Queue*)p;
        int s = q->size();
        Vehicle* found = nullptr;
        for (int i = 0; i < s; ++i) {
            Vehicle* v = (Vehicle*)q->dequeue();
            if (v->getId() == vid) {
                found = v;
            }
            else {
                q->enqueue((void*)v);
            }
        }
        if (found != nullptr) return found;
    }

    return nullptr;
}

bool Intersection::isLaneEmpty(int lane) const {
    if (lane < 0 || lane >= lanes.size()) return true;
    void* p = lanes.getAt(lane);
    if (p == nullptr) return true;
    if (lane == 0) {
        PriorityQueue* pq = (PriorityQueue*)p;
        return pq->isEmpty();
    }
    else {
        Queue* q = (Queue*)p;
        return q->isEmpty();
    }
}

int Intersection::getLanesCount() const {
    return lanes.size();
}
