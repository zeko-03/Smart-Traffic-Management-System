#ifndef UI_H
#define UI_H

#include <fstream>
using namespace std;

class TrafficControlCenter;
class Intersection;

class UI {
private:
    TrafficControlCenter* tcc;
    ofstream* out;

public:
    UI(TrafficControlCenter* t, ofstream* fout);
    ~UI();

    void printArrival(int id, int inter, int lane);
    void printPromotion(int id, int inter, int lane);
    void printCancel(int id, int inter, int lane);
    void printAccident(int inter, int from, int to);
    void printSummary(int a, int f, int c, int p, double avg);

    void printSnapshot();
};

#endif