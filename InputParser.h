#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <string>
using namespace std;

#include "PriorityQueue.h"

class InputParser {
public:
    InputParser();
    bool parseText(const string& txt, PriorityQueue* pq,
        int& numInter, int& switchCost,
        int& autoPromo, int& cancelTh, bool& reroute);
};

#endif
