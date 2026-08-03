#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "LinkedList.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "Vehicle.h"
#include "TrafficLight.h"

class Intersection {
private:
    int id;
    LinkedList lanes;
    TrafficLight light;

public:
    Intersection(int id_);
    ~Intersection();

    void initializeLanes();

    bool enqueueVehicle(int lane, Vehicle* v);
    Vehicle* dequeueFromLane(int lane);
    Vehicle* peekLane(int lane);

    void setActiveGreenLane(int lane);
    int getActiveGreenLane() const;

    Vehicle* removeVehicleById(int vid);
    bool isLaneEmpty(int lane) const;
    int getLanesCount() const;
   
    void update(int switchCost);
	int getLaneSize(int lane) const;
};

#endif
