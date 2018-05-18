/*
 * ------------ TAKEOFF --------------------------------------------------------------------
 *
 * Gestisce i seguenti tipi di messaggi:
 *      -> ACK da Tower: in tal caso, se vi sono aerei che vogliono
 *              partire(lista takeoff_queue != 0), procede ad inviarli;
 *      -> Info da Parking: contenente le informazioni di un aereo
 *              che necessita di partire; verrà inserito nella lista takeoff_queue;
 *
 * ------------------------------------------------------------------------------------------
 */

#include "Takeoff.h"
#include "Plane_m.h"


namespace airport {

Define_Module(Takeoff);

cQueue takeoff_queue("takeoff_queue");


void Takeoff::initialize()
{
    plane = nullptr;
    notify = nullptr;
    beep = nullptr;
}

void Takeoff::handleMessage(cMessage *msg)
{
    Plane *myMsg;
    myMsg = dynamic_cast<Plane*>(msg);

    //Gestione messaggio OK proveniente da Tower
    if(strcmp(msg->getName(), "OK") == 0)
    {

        if(!takeoff_queue.isEmpty())
        {
             EV << "TAKEOFF: Start Take-off\n";
             cObject* obj_plane;
             obj_plane = takeoff_queue.pop();
             plane = dynamic_cast<Plane*>(obj_plane);
             send(plane, "out_pista");
         }

         else
             EV<< "TAKEOFF: The takeoff_queue is empty\n";

     }

     //arriva il segnale di pista libera, mando info sull'aereo
     //che attende da più tempo(se c'è), così torre può scegliere
     //fra lui e il primo in landing_queue
     if(strcmp(msg->getName(), "freeTrack") == 0)
     {
         if(takeoff_queue.isEmpty())
         {
             EV << "TAKEOFF A FREETRACK: Non ho aerei che vogliono decollare\n";
             cMessage *tmp_msg = new cMessage("noPlanesDeparting");
             send(tmp_msg, "out_tower");
         }

         else
         {
             EV << "TAKEOFF AL FREETRACK: mando a torre info sull'aereo in cima alla coda takeoff_queue\n";
             send_info();
         }
     }

     //Gestione messaggio con info aereo da Parking
     else if(myMsg)
     {
         EV << "TAKEOFF: Aereo aggiunto alla takeoff_queue\n";
         myMsg->setEnter(simTime()); //segno a che ore entra nella lista takeoff_queue
         takeoff_queue.insert(myMsg);

         EV << "TAKEOFF: PRINTING TAKE-OFF QUEUE:\n";
         for(cQueue::Iterator iter(takeoff_queue,0); !iter.end(); iter++)
         {
            myMsg =(Plane*) iter.operator *();
            EV <<"- ID:" <<myMsg->getId() << "  ENTRY(s):" << myMsg->getEnter() <<"\n";
         }

         EV << "TAKEOFF A TORRE: mando a torre info sull'aereo appena arrivato\n";
         send_info();
     }
}

void Takeoff::send_info() {
    cObject* obj_plane = takeoff_queue.front();
    plane = dynamic_cast<Plane*>(obj_plane);
    Plane *p = new Plane(nullptr);
    simtime_t t = plane->getEnter();
    p->setEnter(t);
    p->setKind(1);
    send(p, "out_tower");
}

}; // namespace
