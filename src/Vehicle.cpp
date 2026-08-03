#include "Vehicle.h"

Vehicle::Vehicle(int id_, VehicleType type_, int arr, int dur) {
    id = id_;
    type = type_;
    arrivalTime = arr;
    crossingDuration = dur;
    remainingCrossing = dur;
    status = VS_WAITING;
    startTime = -1;
    finishTime = -1;
    currentIntersection = -1;
    currentLane = -1;
    urgency = 0;
}

int Vehicle::getId() const { return id; }
VehicleType Vehicle::getType() const { return type; }
int Vehicle::getArrivalTime() const { return arrivalTime; }
int Vehicle::getCrossingDuration() const { return crossingDuration; }
int Vehicle::getRemainingCrossing() const { return remainingCrossing; }
int Vehicle::getStartTime() const { return startTime; }
int Vehicle::getFinishTime() const { return finishTime; }
VehicleStatus Vehicle::getStatus() const { return status; }

void Vehicle::setCurrentIntersection(int interId) { currentIntersection = interId; }
int Vehicle::getCurrentIntersection() const { return currentIntersection; }

void Vehicle::setCurrentLane(int laneId) { currentLane = laneId; }
int Vehicle::getCurrentLane() const { return currentLane; }

void Vehicle::markStarted(int start) {
    startTime = start;
    status = VS_CROSSING;
    remainingCrossing = crossingDuration;
}

void Vehicle::tickCrossing() {
    if (status == VS_CROSSING && remainingCrossing > 0)
        remainingCrossing--;
}

void Vehicle::markFinished(int finish) {
    finishTime = finish;
    remainingCrossing = 0;
    status = VS_FINISHED;
}

void Vehicle::cancel() {
    status = VS_CANCELED;
}

int Vehicle::waitingTime(int currentTime) const {
    if (startTime >= 0) return startTime - arrivalTime;
    return currentTime - arrivalTime;
}

void Vehicle::promoteToEV(int urg) {
    type = VEH_EV;
    urgency = urg;
}

void Vehicle::setUrgency(int u) { urgency = u; }
int Vehicle::getUrgency() const { return urgency; }
