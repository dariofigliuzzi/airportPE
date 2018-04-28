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
int count;
simtime_t timerl;

void Landing::initialize()
{
   count = 0;
   plane = nullptr;
   notify = nullptr;
   beep = nullptr;
}

void Landing::handleMessage(cMessage *msg)
{
    //Gestione messaggio OK proveniente dalla Tower
    if(strcmp(msg->getName(), "OK") == 0)
    {
        //se ci sono aerei in attesa di atterrare
        if(!landing_queue.isEmpty())
        {
            EV << "Start Landing\n";
            cObject* obj_plane;
            obj_plane = landing_queue.pop();
            plane = check_and_cast<Plane*>(obj_plane);
            //plane = obj_plane->
            send(plane, "out_pista");
        }

        else EV<< "The landing_queue is empty\n";
    }

    //Gestione dei messaggi Plane*
    else
    {

        EV << "Adding plane on landing_queue\n";
        Plane *myMsg;
        myMsg = check_and_cast<Plane*>(msg);
        landing_queue.insert(myMsg);
        count = 0;
        plane = myMsg;
        EV << "PRINTING LANDING QUEUE:\n";

        for(cQueue::Iterator iter(landing_queue,0); !iter.end(); iter++)
        {
            myMsg =(Plane*) iter();
            EV << count++ << " - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
        }

        notify = new cMessage("newplane_land");
        send(notify, "out_tower");
    }
}

};
