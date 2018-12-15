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
    takeoff_queue.clear();
    plane = nullptr;
    notify = nullptr;
    beep = nullptr;

    arrivalSignalId = registerSignal("arrivalId");
    arrivalSignalLength = registerSignal("arrivalLength");
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
             send(plane, "out_airstrip");
             emit(arrivalSignalLength, takeoff_queue.getLength());
             emit(arrivalSignalId, plane->getId());
         }

         else
             EV<< "TAKEOFF: The takeoff_queue is empty\n";

     }


     //Gestione messaggio con info aereo da Parking
     else if(myMsg)
     {
         EV << "TAKEOFF: Aereo aggiunto alla takeoff_queue\n";
         myMsg->setEnter(simTime());        //segno a che ore entra nella lista takeoff_queue
         myMsg->setKind(1);                 //setto per sicurezza, verrà tolto dopo i dovuti controlli
         takeoff_queue.insert(myMsg);

         //emissione segnale contenente lunghezza coda
         emit(arrivalSignalLength, takeoff_queue.getLength());

         EV << "TAKEOFF: PRINTING TAKE-OFF QUEUE:\n";
         for(cQueue::Iterator iter(takeoff_queue,0); !iter.end(); iter++)
         {
            myMsg =(Plane*) iter.operator *();
            EV <<"- ID:" <<myMsg->getId() << "  ENTRY(s):" << myMsg->getEnter() <<"\n";
         }

         EV << "TAKEOFF A TOWER: mando a torre info sull'aereo appena arrivato\n";
         send(get_info(myMsg), "out_tower");
     }
}

//Prende le info dal myMsg della handleMessage. Non posso passare alla send
//direttamente myMsg perchè attualmente risiede nella takeoff_queue
Plane* Takeoff::get_info(Plane* p) {
    plane = new Plane(nullptr);
    plane->setEnter(p->getEnter());
    plane->setId(p->getId());
    plane->setKind(p->getKind());
    return plane;
}

}; // namespace
