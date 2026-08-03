#include "UI.h"
#include "TrafficControlCenter.h"
#include "Intersection.h"
#include "Vehicle.h"
#include <iomanip>
using namespace std;

UI::UI(TrafficControlCenter* t, ofstream* fout)
{
    tcc = t;
    out = fout;
}

UI::~UI()
{
}

void UI::printArrival(int id, int inter, int lane)
{
    if (!out) return;
    (*out) << "[LOG] Arrival vehicle " << id
        << " at intersection " << inter
        << " lane " << lane << "\n";
    out->flush();
}

void UI::printPromotion(int id, int inter, int lane)
{
    if (!out) return;
    (*out) << "[LOG] Vehicle " << id
        << " promoted to EV at intersection "
        << inter << " lane " << lane << "\n";
    out->flush();
}

void UI::printCancel(int id, int inter, int lane)
{
    if (!out) return;
    (*out) << "[LOG] Vehicle " << id
        << " canceled at intersection "
        << inter << " lane " << lane << "\n";
    out->flush();
}

void UI::printAccident(int inter, int from, int to)
{
    if (!out) return;
    (*out) << "[LOG] Accident at intersection "
        << inter << " lane " << from
        << " rerouted to lane " << to << "\n";
    out->flush();
}

void UI::printSnapshot()
{
    if (!out || !tcc) return;

    int time = tcc->getCurrentTime();
    int n = tcc->getIntersectionsCount();

    (*out) << "----- Time: " << time << " -----\n";

    for (int i = 0; i < n; ++i) {
        Intersection* I = tcc->getIntersection(i);
        if (I == nullptr) continue;

        (*out) << "Intersection " << i << "\n";

        int lanes = I->getLanesCount();
        for (int l = 0; l < lanes; ++l) {
            Vehicle* v = I->peekLane(l);
            (*out) << " Lane " << l << ": ";
            if (v == nullptr) {
                (*out) << "empty\n";
            }
            else {
                (*out) << "[id=" << v->getId()
                    << " rem=" << v->getRemainingCrossing()
                    << "]\n";
            }
        }
        (*out) << "\n";
    }
    out->flush();
}

void UI::printSummary(int a, int f, int c, int p, double avg)
{
    if (!out) return;
    (*out) << "\n===== FINAL SUMMARY =====\n";
    (*out) << "Total Arrivals  : " << a << "\n";
    (*out) << "Total Finished  : " << f << "\n";
    (*out) << "Total Canceled  : " << c << "\n";
    (*out) << "Total Promoted  : " << p << "\n";
    (*out) << "Average Waiting : " << avg << "\n";
    (*out) << "=========================\n";
    out->flush();

    printSnapshot();
}
