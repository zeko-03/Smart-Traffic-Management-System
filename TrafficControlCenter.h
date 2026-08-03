#ifndef TRAFFICCONTROLCENTER_H
#define TRAFFICCONTROLCENTER_H

#include "PriorityQueue.h"
#include "Event.h"
#include "Intersection.h"
#include "TrafficLight.h"

class UI;

class TrafficControlCenter {
private:
    bool silentMode;
    int numIntersections;
    Intersection** inters;
    PriorityQueue* eventQ;
    int currentTime;
    UI* ui;
    int switchCost;
    int autoPromo;

    int totalArrivals;
    int totalFinished;
    int totalCanceled;
    int totalPromoted;
    int totalWaitingSum;

public:
    TrafficControlCenter(int n, PriorityQueue* pq);
    ~TrafficControlCenter();

    void setUI(UI* u);
    void setConfig(int switchCost_, int autoPromr_);
    int getCurrentTime() const;

    Intersection* getIntersection(int id);
    int getIntersectionsCount() const;

    void processEvent(Event* e);
    void assignLanes();
    void updateCrossings();

    void step();
    void run(bool silent);
    bool anyQueuesNonEmpty() const;

	double averageWaitingTime() const;
};

#endif
