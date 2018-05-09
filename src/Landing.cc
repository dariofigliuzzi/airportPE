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
    Plane *myMsg;
    myMsg = dynamic_cast<Plane*>(msg);

    //Gestione messaggio OK proveniente dalla Tower
    if(strcmp(msg->getName(), "OK") == 0)
    {
        //se ci sono aerei in attesa di atterrare
        if(!landing_queue.isEmpty())
        {
            EV << "Start Landing\n";
            cObject* obj_plane;
            obj_plane = landing_queue.pop();
            plane = dynamic_cast<Plane*>(obj_plane);
            send(plane, "out_pista");
        }

        else EV<< "The landing_queue is empty\n";
    }

    /*arriva il segnale di pista libera, mando info sull'aereo
      che attende da più tempo(se c'è), così Tower può scegliere
      fra lui e il primo in takeoff_queue*/
    if(strcmp(msg->getName(), "freeTrack") == 0)
    {
        if(landing_queue.isEmpty())
        {
            EV << "Landing: Non ho aereo che vogliono atterrare\n";
            cMessage *tmp_msg = new cMessage("noPlanesLanding");
            send(tmp_msg, "out_tower");
        }

        else
        {
            EV << "Free track: mando a torre info sull'aereo in cima alla coda landing_queue\n";
            cObject* obj_plane = landing_queue.front();
            plane = dynamic_cast<Plane*>(obj_plane);
            Plane *p = new Plane(nullptr);
            simtime_t t = plane->getEnter();
            p->setEnter(t);
            p->setKind(0);
            send(p, "out_tower");
        }
    }

    //Gestione dei messaggi Plane* da sky
    else if(myMsg)
    {

        EV << "Adding plane on landing_queue\n";
        myMsg->setEnter(simTime()); //segno a che ore entra nella lista landing_queue
        EV << myMsg-> getEnter() << "\n";
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
