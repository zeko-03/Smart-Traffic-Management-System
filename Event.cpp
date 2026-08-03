#include "Event.h"

Event::Event(int t, EventType tp, Vehicle* v, int i, int l, int x) {
    time = t;
    type = tp;
    vehicle = v;
    inter = i;
    lane = l;
    extra = x;
}

int Event::getTime() const { return time; }
EventType Event::getType() const { return type; }
Vehicle* Event::getVehicle() const { return vehicle; }
int Event::getIntersection() const { return inter; }
int Event::getLane() const { return lane; }
int Event::getExtra() const { return extra; }
