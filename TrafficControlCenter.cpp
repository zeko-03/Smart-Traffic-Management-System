#include "TrafficControlCenter.h"
#include "UI.h"
#include <iostream>

TrafficControlCenter::TrafficControlCenter(int n, PriorityQueue * pq) {
    numIntersections = n;
    eventQ = pq;
    currentTime = 0;
    ui = nullptr;
    silentMode = false;
    switchCost = 1;
    autoPromo = 1000000;
    totalArrivals = 0;
    totalFinished = 0;
    totalCanceled = 0;
    totalPromoted = 0;
    totalWaitingSum = 0;
    inters = new Intersection * [n];
    for (int i = 0; i < n; i++) inters[i] = new Intersection(i);
}

TrafficControlCenter::~TrafficControlCenter() {
    for (int i = 0; i < numIntersections; i++) delete inters[i];
    delete[] inters;
}

void TrafficControlCenter::setUI(UI* u) {
    ui = u;
}

void TrafficControlCenter::setConfig(int switchCost_, int autoPromo_) {
    switchCost = switchCost_;
    autoPromo = autoPromo_;
}

int TrafficControlCenter::getCurrentTime() const {
    return currentTime;
}

Intersection* TrafficControlCenter::getIntersection(int id) {
    if (id < 0 || id >= numIntersections) return nullptr;
    return inters[id];
}

int TrafficControlCenter::getIntersectionsCount() const
{
    return numIntersections;
}


void TrafficControlCenter::processEvent(Event* e) {
    if (!e) return;
    EventType t = e->getType();
    if (t == EVT_ARRIVAL) {
        Vehicle* v = e->getVehicle();
        int inter = e->getIntersection();
        int lane = e->getLane();
        Intersection* I = getIntersection(inter);
        if (I != nullptr && v != nullptr) {
            I->enqueueVehicle(lane, v);
            v->setCurrentIntersection(inter);
            v->setCurrentLane(lane);
            totalArrivals++;
            if (!silentMode && ui != nullptr) ui->printArrival(v->getId(), inter, lane);
        }
        else {
            if (v != nullptr) delete v;
        }
    }
    else if (t == EVT_PROMOTION) {
        Vehicle* vtemp = e->getVehicle();
        int idToPromote = vtemp ? vtemp->getId() : -1;
        int urg = e->getExtra();
        if (vtemp) delete vtemp;
        if (idToPromote >= 0) {
            for (int i = 0; i < numIntersections; ++i) {
                Vehicle* v = inters[i]->removeVehicleById(idToPromote);
                if (v != nullptr) {
                    v->promoteToEV(urg);
                    inters[i]->enqueueVehicle(v->getCurrentLane(), v);
                    totalPromoted++;
                    if (!silentMode && ui != nullptr) ui->printPromotion(v->getId(), i, v->getCurrentLane());
                    break;
                }
            }
        }
    }
    else if (t == EVT_CANCEL) {
        Vehicle* vtemp = e->getVehicle();
        int idToCancel = vtemp ? vtemp->getId() : -1;
        if (vtemp) delete vtemp;
        if (idToCancel >= 0) {
            for (int i = 0; i < numIntersections; i++) {
                Vehicle* rem = inters[i]->removeVehicleById(idToCancel);
                if (rem != nullptr) {
                    rem->cancel();
                    totalCanceled++;
                    if (!silentMode && ui != nullptr) ui->printCancel(rem->getId(), i, rem->getCurrentLane());
                    delete rem;
                    break;
                }
            }
        }
    }
    else if (t == EVT_ACCIDENT) {
        int inter = e->getIntersection();
        int lane = e->getLane();
        Intersection* I = getIntersection(inter);
        if (I != nullptr) {
            int target = (lane + 1) % 4;
            while (!I->isLaneEmpty(lane)) {
                Vehicle* v = I->dequeueFromLane(lane);
                if (v == nullptr) break;
                v->setCurrentLane(target);
                I->enqueueVehicle(target, v);
            }
            if (!silentMode && ui != nullptr) ui->printAccident(inter, lane, target);
        }
    }
}

void TrafficControlCenter::assignLanes() {
    for (int i = 0; i < numIntersections; i++) {
        Intersection* I = inters[i];
        if (I == nullptr) continue;
        int active = I->getActiveGreenLane();
        if (active >= 0 && !I->isLaneEmpty(active)) continue;
        for (int l = 0; l < I->getLanesCount(); l++) {
            if (!I->isLaneEmpty(l)) {
                I->setActiveGreenLane(l);
                break;
            }
        }
    }
}

void TrafficControlCenter::updateCrossings() {
    for (int i = 0; i < numIntersections; i++) {
        Intersection* I = inters[i];
        if (I == nullptr) continue;
        int lanes = I->getLanesCount();
        for (int l = 0; l < lanes; l++) {
            Vehicle* v = I->peekLane(l);
            if (v == nullptr) continue;
            if (v->getStatus() == VS_WAITING) {
                if (I->getActiveGreenLane() == l) {
                    Vehicle* started = I->dequeueFromLane(l);
                    if (started != nullptr) {
                        started->markStarted(currentTime);
                        I->enqueueVehicle(l, started);
                    }
                }
            }
            else if (v->getStatus() == VS_CROSSING) {
                v->tickCrossing();
                if (v->getRemainingCrossing() <= 0) {
                    I->dequeueFromLane(l);
                    v->markFinished(currentTime);
                    totalFinished++;
                    totalWaitingSum += v->waitingTime(currentTime);
                    delete v;
                }
            }
        }
    }

    for (int i = 0; i < numIntersections; i++) {
        Intersection* I = inters[i];
        if (I == nullptr) continue;
        int lanes = I->getLanesCount();
        for (int l = 0; l < lanes; l++) {
            int s = I->getLaneSize(l);
            for (int k = 0; k < s; k++) {
                Vehicle* v = I->dequeueFromLane(l);
                if (v == nullptr) continue;
                if (v->getStatus() == VS_WAITING) {
                    int waited = v->waitingTime(currentTime);
                    if (waited >= autoPromo && v->getType() != VEH_EV) {
                        v->promoteToEV(5);
                        v->setCurrentLane(0);
                        I->enqueueVehicle(0, v);
                        totalPromoted++;
                        if (!silentMode && ui != nullptr) ui->printPromotion(v->getId(), i, 0);
                        continue;
                    }
                }
                I->enqueueVehicle(l, v);
            }
        }
    }
}

void TrafficControlCenter::step() {
    bool hadEventAtThisTime = false;
    Event* nextPeek = (Event*)eventQ->peek();
    if (nextPeek != nullptr && nextPeek->getTime() == currentTime) hadEventAtThisTime = true;

    bool anyQBefore = anyQueuesNonEmpty();

    while (!eventQ->isEmpty()) {
        Event* e = (Event*)eventQ->peek();
        if (e == nullptr) break;
        if (e->getTime() != currentTime) break;
        e = (Event*)eventQ->dequeue();
        processEvent(e);
        delete e;
    }

    assignLanes();
    updateCrossings();

    for (int i = 0; i < numIntersections; ++i) {
        if (inters[i] != nullptr)
            inters[i]->update(switchCost);
    }

    if (ui != nullptr && (!silentMode) && (hadEventAtThisTime || anyQBefore || anyQueuesNonEmpty()))
        ui->printSnapshot();

    currentTime++;
}
bool TrafficControlCenter::anyQueuesNonEmpty() const {
    for (int i = 0; i < numIntersections; i++) {
        Intersection* I = inters[i];
        if (I == nullptr) continue;
        for (int l = 0; l < I->getLanesCount(); l++) {
            if (!I->isLaneEmpty(l)) return true;
        }
    }
    return false;
}

double TrafficControlCenter::averageWaitingTime() const {
    if (totalFinished == 0) return 0;
    return double(totalWaitingSum) / double(totalFinished);
}

void TrafficControlCenter::run(bool silent) {
    silentMode = silent;
    int safety = 1000000;
    int lastProgressTime = -1;

    while ((!eventQ->isEmpty() || anyQueuesNonEmpty()) && safety-- > 0) {
        Event* nextEv = (Event*)(eventQ->peek());
        int nextTime = nextEv ? nextEv->getTime() : -1;
        bool anyQ = anyQueuesNonEmpty();

        if (!anyQ && nextEv == nullptr) break;

        if (!anyQ && nextEv != nullptr && nextTime > currentTime) {
            currentTime = nextTime;
        }

        step();

        if (currentTime != lastProgressTime) {
            lastProgressTime = currentTime;
        }
        else {
            if (eventQ->isEmpty() && !anyQueuesNonEmpty()) break;
        }
    }

    silentMode = false;
    if (ui != nullptr) ui->printSummary(totalArrivals, totalFinished, totalCanceled, totalPromoted, averageWaitingTime());
}
