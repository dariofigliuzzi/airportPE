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
    //timer = par("procTime");
    timer = rand() % 10 + 25;
    scheduleAt(simTime() + timer, beep);

    arrivalSignalId = registerSignal("arrivalId");
}

void Sky::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        EV << "SKY: Periodo di attesa scaduto, invio aereo\n";

        msg_creation(count_s);

        send(plane, "out");
        beep = new cMessage("beep");
        scheduleAt(simTime()+timer, beep);
        count_s++;

        emit(arrivalSignalId, plane->getId());
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
