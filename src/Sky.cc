/*
 * ------------ SKY ------------------------------------------------------------------------
 *
 * Codice atto alla creazione periodica di aerei da aggiungere alla lista Landing
 *
 * ------------------------------------------------------------------------------------------
 */

#include "Sky.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Sky);

simtime_t timer;
int count_s;

void Sky::initialize()
{
    count_s = 0;                    //contatore aerei
    beep = new cMessage("beep");    //per creazione periodica aerei
    plane =nullptr;                 //cMessage con info su aereo
    timer = par("procTime");
    scheduleAt(simTime() + timer, beep);
}

void Sky::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        EV << "Wait period is over, sending plane\n";

        msg_creation(count_s);

        send(plane, "out");
        cancelAndDelete(beep);
        beep = new cMessage("beep");
        scheduleAt(simTime()+timer, beep);
        count_s++;
    }
}

/*creazione del messaggio con info aereo*/
void Sky::msg_creation(int n)
{

    plane = new Plane(nullptr);
    plane->setId(n);
    plane->setEnter(simTime());
    plane->setKind(0);
}

}; // namespace
