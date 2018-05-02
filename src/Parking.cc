/*
 * ------------ PARKING --------------------------------------------------------------------
 *
 * Codice atto alla gestione di due tipi di messaggi:
 *      -> isSelfMessage():;
 *      -> Messaggio con info dell'aereo in arrivo dalla pista:;
 *
 * ------------------------------------------------------------------------------------------
 */

#include "Parking.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Parking);

cQueue parking_queue("parking_queue");
int count_pk;
simtime_t timerpk;

void Parking::initialize()
{
    count_pk = 0;
    timerpk = par("procTime");
}

void Parking::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        //TODO: non ho capito st'if
        if(!parking_queue.isEmpty())
        {
           EV << "Start Take-off\n";
           cObject* obj_plane;
           obj_plane = parking_queue.pop();
           plane = check_and_cast<Plane*>(obj_plane);
           send(plane, "out_takeoff");
        }
        else
           EV<< "The parking_queue is empty\n";
    }

    //Gestione messaggio con info aereo da Pista
    else
    {

        EV << "Adding plane on parking_queue\n";
        Plane *myMsg;
        myMsg = dynamic_cast<Plane*>(msg);
        myMsg->setKind(1);
        parking_queue.insert(myMsg);
        count_pk = 0;
        plane = myMsg;

        EV << "PRINTING PARKING QUEUE:\n";
        for(cQueue::Iterator iter(parking_queue,0); !iter.end(); iter++)
        {
                  myMsg = (Plane*) iter();
                  EV << count_pk++ << " - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
        }

        //Start timer per tempo parcheggio
        cancelAndDelete(beep);
        beep = new cMessage("beep");
        scheduleAt(simTime()+timerpk, beep);

    }

}

}; // namespace
