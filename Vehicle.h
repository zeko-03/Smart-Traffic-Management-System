#ifndef VEHICLE_H
#define VEHICLE_H

enum VehicleType { VEH_EV, VEH_PT, VEH_FV, VEH_NC };
enum VehicleStatus { VS_WAITING, VS_CROSSING, VS_FINISHED, VS_CANCELED };

class Vehicle {
private:
    int id;
    VehicleType type;
    VehicleStatus status;

    int arrivalTime;
    int crossingDuration;
    int remainingCrossing;

    int startTime;
    int finishTime;

    int currentIntersection;
    int currentLane;

    int urgency;

public:
    Vehicle(int id, VehicleType type, int arrivalTime, int crossingDuration);

    int getId() const;
    VehicleType getType() const;
    int getArrivalTime() const;
    int getCrossingDuration() const;
    int getRemainingCrossing() const;

    int getStartTime() const;
    int getFinishTime() const;
    VehicleStatus getStatus() const;

    void setCurrentIntersection(int interId);
    int getCurrentIntersection() const;

    void setCurrentLane(int laneId);
    int getCurrentLane() const;

    void markStarted(int start);
    void tickCrossing();
    void markFinished(int finish);
    void cancel();

    int waitingTime(int currentTime) const;

    void promoteToEV(int urg);

    void setUrgency(int u);
    int getUrgency() const;
};

#endif
