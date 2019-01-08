/*
 * ------------ LANDING --------------------------------------------------------------------
 *
 * Codice atto alla gestione di due tipi di messaggi:
 *      -> ACK/NAK della torre: in tal caso l'aereo da più tempo in attesa atterrerà (se c'è);
 *      -> Aggiunta aereo alla landing_queue;
 *
 * ------------------------------------------------------------------------------------------
 */

#include "Landing.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Landing);

cQueue landing_queue("landing_queue");
simtime_t landing_wt;
simtime_t timer_l;
int count_sky;

void Landing::initialize()
{
   //-----Sky implementation---
   timer_l = par("skyTimer");
   count_sky = 0;
   beep = new cMessage("beep");
   scheduleAt(simTime() + timer_l, beep);
   //--------------------------
   landing_queue.clear();
   plane = nullptr;
   beep = nullptr;
   arrivalSignalLength = registerSignal("arrivalLength");
   arrivalSignalWaiting = registerSignal("arrivalWaiting");
   landing_wt = 0;
}

void Landing::handleMessage(cMessage *msg)
{
    Plane *myMsg;
    myMsg = dynamic_cast<Plane*>(msg);

    //Gestione messaggio OK proveniente dalla Tower
    if(strcmp(msg->getName(), "OK") == 0)
    {
        //se ci sono aerei in attesa di atterrare
        if(!landing_queue.isEmpty())
        {
            EV << "LANDING: Start Landing\n";
            cObject* obj_plane;
            obj_plane = landing_queue.front();
            plane = dynamic_cast<Plane*>(obj_plane);
            landing_queue.remove(obj_plane);

            //emissione segnale contenente lunghezza coda
            emit(arrivalSignalLength, landing_queue.getLength());
            emit(arrivalSignalWaiting, (simTime() - plane->getEnter()));
        }

        else EV<< "LANDING: The landing_queue is empty\n";
    }


    //Gestione dei messaggi Plane* da sky
    else if(strcmp(msg->getName(), "beep") == 0)
    {
        myMsg = plane_creation(count_sky);
        EV << "LANDING: Aereo aggiunto alla landing_queue\n";
        landing_queue.insert(myMsg);

        //emissione segnale con numero di aereo nella coda Landing
        emit(arrivalSignalLength, landing_queue.getLength());

        send(get_info(myMsg), "out_airstrip");

        EV << "LANDING: PRINTING LANDING QUEUE:\n";

        for(cQueue::Iterator iter(landing_queue,0); !iter.end(); iter++)
        {
            myMsg =(Plane*) iter.operator *();
            EV <<"- ID:" <<myMsg->getId() << "  ENTRY(s):" << myMsg->getEnter() <<"\n";
        }

        beep = new cMessage("beep");
        scheduleAt(simTime() + timer_l, beep);
        count_sky++;
    }
}

//Prende le info dal myMsg della handleMessage. Non posso passare alla send
//direttamente myMsg perchè attualmente risiede nella landing_queue
Plane* Landing::get_info(Plane* p) {
    plane = new Plane(nullptr);
    plane->setEnter(p->getEnter());
    plane->setId(p->getId());
    plane->setKind(p->getKind());
    return plane;
}

Plane* Landing::plane_creation(int n)
{
    Plane* plane;
    plane = new Plane(nullptr);
    plane->setId(n);
    plane->setEnter(simTime());
    plane->setKind(0);
    return plane;
}


};
