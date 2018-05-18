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
simtime_t timerl;

void Landing::initialize()
{
   plane = nullptr;
   notify = nullptr;
   beep = nullptr;
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
            obj_plane = landing_queue.pop();
            plane = dynamic_cast<Plane*>(obj_plane);
            send(plane, "out_airstrip");
        }

        else EV<< "LANDING: The landing_queue is empty\n";
    }


    //Gestione dei messaggi Plane* da sky
    else if(myMsg)
    {
        EV << "LANDING: Aereo aggiunto alla landing_queue\n";

        myMsg->setEnter(simTime());     //segno a che ore entra nella lista landing_queue
        myMsg->setKind(0);              //setto per sicurezza
        landing_queue.insert(myMsg);

        EV << "LANDING: PRINTING LANDING QUEUE:\n";

        for(cQueue::Iterator iter(landing_queue,0); !iter.end(); iter++)
        {
            myMsg =(Plane*) iter.operator *();
            EV <<"- ID:" <<myMsg->getId() << "  ENTRY(s):" << myMsg->getEnter() <<"\n";
        }


        EV << "LANDING A TOWER: mando a torre info sull'aereo appena arrivato\n";
        send(get_info(myMsg), "out_tower");
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

};
