#ifndef EVENT_H
#define EVENT_H

#include "Vehicle.h"

enum EventType { EVT_ARRIVAL, EVT_PROMOTION, EVT_CANCEL, EVT_ACCIDENT };

class Event {
private:
    int time;
    EventType type;
    Vehicle* vehicle;
    int inter;
    int lane;
    int extra;

public:
    Event(int t, EventType tp, Vehicle* v, int i, int l, int x);

    int getTime() const;
    EventType getType() const;
    Vehicle* getVehicle() const;
    int getIntersection() const;
    int getLane() const;
    int getExtra() const;
};

#endif
