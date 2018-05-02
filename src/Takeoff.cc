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
int count_to;


void Takeoff::initialize()
{
    count_to = 0;
    plane = nullptr;
    notify = nullptr;
    beep = nullptr;
}

void Takeoff::handleMessage(cMessage *msg)
{
    //Gestione messaggio OK proveniente da Tower
    if(strcmp(msg->getName(), "OK") == 0)
    {

        if(!takeoff_queue.isEmpty())
        {
             EV << "Start Take-off\n";
             cObject* obj_plane;
             obj_plane = takeoff_queue.pop();
             plane = check_and_cast<Plane*>(obj_plane);
             //plane = obj_plane->
             send(plane, "out_pista");
         }

         else
             EV<< "The takeoff_queue is empty\n";
     }

     //arriva il segnale di pista libera, mando info sull'aereo
     //che attende da più tempo(se c'è), così torre può scegliere
     //fra lui e il primo in landing_queue
     if(strcmp(msg->getName(), "freeTrack") == 0)
     {
         if(takeoff_queue.isEmpty())
         {
             EV << "Landing: Non ho aereo che vogliono decollare\n";
             cMessage *tmp_msg = new cMessage("noPlanesDeparting");
             send(tmp_msg, "out_tower");
         }
         else
         {
             EV << "Free track: mando a torre info sull'aereo in cima alla coda takeoff_queue\n";
             cObject* obj_plane = takeoff_queue.front();
             Plane* p = dynamic_cast<Plane*>(obj_plane);
             send(p, "out_tower");
         }
     }

     //Gestione messaggio con info aereo da Parking
     else
     {
         EV << "Adding plane on takeoff_queue\n";
         Plane *myMsg;
         myMsg = dynamic_cast<Plane*>(msg);
         myMsg->setEnter(simTime()); //segno a che ore entra nella lista takeoff_queue
         takeoff_queue.insert(myMsg);
         count_to = 0;
         plane = myMsg;

         EV << "PRINTING TAKE-OFF QUEUE:\n";
         for(cQueue::Iterator iter(takeoff_queue,0); !iter.end(); iter++)
         {
            myMsg =(Plane*) iter();
            EV << count_to++ << " - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
         }

         notify = new cMessage("newplane_takeoff");
         send(notify, "out_tower");
     }
}

}; // namespace
