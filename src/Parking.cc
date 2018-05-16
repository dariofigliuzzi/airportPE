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
simtime_t timerpk;

void Parking::initialize()
{
    timerpk = par("procTime");
}

void Parking::handleMessage(cMessage *msg)
{
    Plane *myMsg;
    myMsg = dynamic_cast<Plane*>(msg);

    if(msg->isSelfMessage())
    {
        if(!parking_queue.isEmpty())
        {
           EV << "PARKING: Start Take-off\n";
           cObject* obj_plane;
           obj_plane = parking_queue.pop();
           plane = dynamic_cast<Plane*>(obj_plane);
           send(plane, "out_takeoff");
        }
        else
           EV<< "PARKING: The parking_queue is empty\n";
    }

    //Gestione messaggio con info aereo da Pista
    else if(myMsg)
    {

        EV << "PARKING: Aereo aggiunto alla parking_queue\n";
        myMsg->setKind(1);
        parking_queue.insert(myMsg);
        plane = myMsg;

        EV << "PARKING: PRINTING PARKING QUEUE:\n";

        for(cQueue::Iterator iter(parking_queue); !iter.end(); iter++)
        {
                  myMsg =  (Plane*) iter.operator *();
                  EV <<"- ID:" <<myMsg->getId() << "  ENTRY(s):" << myMsg->getEnter() <<"\n";
        }

        //Start timer per tempo parcheggio
        beep = new cMessage("beep");
        scheduleAt(simTime()+timerpk, beep);

    }

}

}; // namespace
