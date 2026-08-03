#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

class TrafficLight {
public:
    TrafficLight();

    void update(int switchCost);
    int getGreenLane() const;
    void setGreen(int lane);

private:
    int greenLane;
    int timer;
};

#endif
