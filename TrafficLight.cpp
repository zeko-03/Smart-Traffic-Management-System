#include "TrafficLight.h"

TrafficLight::TrafficLight() {
    greenLane = 0;
    timer = 0;
}

void TrafficLight::update(int switchCost) {
    timer++;
    if (timer >= switchCost) {
        timer = 0;
        greenLane = (greenLane + 1) % 4;
    }
}

int TrafficLight::getGreenLane() const {
    return greenLane;
}
void TrafficLight::setGreen(int lane) {
    if (lane < 0) lane = 0;
    greenLane = lane % 4;
    timer = 0;
}
