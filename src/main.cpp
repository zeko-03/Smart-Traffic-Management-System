#include "TrafficControlCenter.h"
#include "InputParser.h"
#include "UI.h"
#include <fstream>
#include <string>
using namespace std;

int main()
{
    PriorityQueue* pq = new PriorityQueue();

    ifstream in("input_phase1_2.txt");
    if (!in.is_open()) return 0;

    string text, line;
    while (getline(in, line)) text += line + "\n";
    in.close();

    int n, switchCost, autoPromo, cancelTh;
    bool reroute;

    InputParser parser;
    parser.parseText(text, pq, n, switchCost, autoPromo, cancelTh, reroute);

    TrafficControlCenter* tcc = new TrafficControlCenter(n, pq);
    tcc->setConfig(switchCost, autoPromo);

    ofstream out("output_phase2.txt");
    if (!out.is_open()) {
        delete tcc;
        delete pq;
        return 0;
    }

    UI* ui = new UI(tcc, &out);
    tcc->setUI(ui);

    tcc->run(false);

    delete tcc;
    delete ui;
    delete pq;

    out.close();

    return 0;
}