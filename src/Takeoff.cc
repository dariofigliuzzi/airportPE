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

    //Gestione messaggio con info aereo da Parking
     else
     {
         EV << "Adding plane on takeoff_queue\n";
         Plane *myMsg;
         myMsg = check_and_cast<Plane*>(msg);
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
