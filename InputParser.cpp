#include "InputParser.h"
#include "Event.h"
#include "Vehicle.h"
#include <string>

InputParser::InputParser() {}

bool InputParser::parseText(const string& txt, PriorityQueue* pq,
    int& numInter, int& switchCost,
    int& autoPromo, int& cancelTh, bool& reroute)
{
    int i = 0;
    int len = (int)txt.length();
    string t;

    auto readToken = [&](string& out) {
        out = "";
        while (i < len && (txt[i] == ' ' || txt[i] == '\n' || txt[i] == '\r' || txt[i] == '\t')) i++;
        while (i < len && (txt[i] != ' ' && txt[i] != '\n' && txt[i] != '\r' && txt[i] != '\t')) {
            out += txt[i];
            i++;
        }
        return out.length() > 0;
        };

    if (!readToken(t)) return false;
    numInter = stoi(t);

    if (!readToken(t)) return false;
    switchCost = stoi(t);

    if (!readToken(t)) return false;
    autoPromo = stoi(t);

    if (!readToken(t)) return false;
    cancelTh = stoi(t);

    if (!readToken(t)) return false;
    reroute = (stoi(t) != 0);

    while (readToken(t)) {
        if (t == "A") {
            readToken(t); int time = stoi(t);
            readToken(t); int vid = stoi(t);
            readToken(t); char typeChar = t[0];
            readToken(t); int inter = stoi(t);
            readToken(t); int lane = stoi(t);
            readToken(t); int dur = stoi(t);

            VehicleType vt = VEH_NC;
            if (typeChar == 'E') vt = VEH_EV;
            if (typeChar == 'P') vt = VEH_PT;
            if (typeChar == 'F') vt = VEH_FV;

            Vehicle* v = new Vehicle(vid, vt, time, dur);
            Event* e = new Event(time, EVT_ARRIVAL, v, inter, lane, 0);
            pq->enqueue((void*)e);
        }
        else if (t == "P") {
            readToken(t); int time = stoi(t);
            readToken(t); int vid = stoi(t);
            readToken(t); int urg = stoi(t);

            Vehicle* v = new Vehicle(vid, VEH_EV, time, 0);
            v->setUrgency(urg);
            Event* e = new Event(time, EVT_PROMOTION, v, 0, 0, urg);
            pq->enqueue((void*)e);
        }
        else if (t == "C") {
            readToken(t); int time = stoi(t);
            readToken(t); int vid = stoi(t);
            readToken(t); int inter = stoi(t);
            readToken(t); int lane = stoi(t);

            Vehicle* v = new Vehicle(vid, VEH_NC, time, 0);
            Event* e = new Event(time, EVT_CANCEL, v, inter, lane, 0);
            pq->enqueue((void*)e);
        }
        else if (t == "X") {
            readToken(t); int time = stoi(t);
            readToken(t); int inter = stoi(t);
            readToken(t); int lane = stoi(t);
            readToken(t); int dur = stoi(t);

            Event* e = new Event(time, EVT_ACCIDENT, nullptr, inter, lane, dur);
            pq->enqueue((void*)e);
        }
    }
    Event* test = (Event*)pq->peek();
    if (test != nullptr) {

        int eventTime = test->getTime();

    }

    return true;
}
