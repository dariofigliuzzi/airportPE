/*
 * ------------ TORRE di CONTROLLO ---------------------------------------------------------
 *
 * Codice atto alla gestione delle due code di attarraggio e partenza.
 * Può ricevere messaggi da 3 fonti:
 *
 *      -> Airstrip: Risposta ad una richiesta di conoscenza pista libera/occupata;
 *           free_track: mi informa della liberazione della pista, che posso assegnare ad
 *              un altro aereo in partenza/arrivo.
 *
 *      -> landing_queue/takeoff_queue: risposta a richiesta di informazione sullo stato
 *              della coda.
 *
 *      -> landing_queue/takeoff_queue: Ricezione di una richiesta di atterraggio/partenza;
 *
 * Nel caso di richiesta atterraggio/partenza, provvede a rispondere l'esito al mittente.
 *
 * ------------------------------------------------------------------------------------------
 */

#include "Tower.h"
#include "Plane_m.h"


namespace airport {

Define_Module(Tower);

cMessage *temp_msg_land;    //per invio OK a Landing
cMessage *temp_msg_takeoff; //per invio OK a Takeoff
bool free;

//nuovo approccio
cQueue waiting_queue;

void Tower::initialize()
{
    temp_msg_land = nullptr;
    temp_msg_takeoff = nullptr;
    free = true;
}

void Tower::handleMessage(cMessage *msg)
{
   Plane* p;
   p = dynamic_cast<Plane*>(msg);


   //non appena la pista si libera chiamo la send_OK
   //per decidere quale aereo e da quale coda far partite.
   if(strcmp(msg->getName(), "freeTrack") == 0)
   {
       EV << "TOWER: Avviso FREETRACK ricevuto\n";
       free = true;
       send_OK();
       return;
   }

   else if(p)
   {
       waiting_queue.insert(p);
       EV<< "TOWER: Aereo aggiunto nella coda di attesa per pista\n";

       if(free) send_OK();
       return;
   }
}

void Tower::send_OK() {
    cObject* obj_plane = nullptr;
    Plane* tmp_plane;

    if(!waiting_queue.isEmpty())
    {
        obj_plane = waiting_queue.front();
        tmp_plane = dynamic_cast<Plane*>(obj_plane);

        EV << "TOWER: PRINTING WAITING QUEUE:\n";

        for(cQueue::Iterator iter(waiting_queue,0); !iter.end(); iter++)
        {
            Plane* myMsg =(Plane*) iter.operator *();
            EV <<"- ID:" <<myMsg->getId() << "  ENTRY(s):" << myMsg->getEnter() << "  KIND:" << myMsg->getKind() << "\n"; //0:Landing 1:Takeoff
        }

        if(tmp_plane->getKind() == 0) { //aereo in arrivo da landing
            free = false;
            EV << "TOWER: Faccio atterrare in seguito a pista libera\n";
            cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
            send(tmp_msg, "out_land");
            waiting_queue.remove(waiting_queue.front());
            return;
        }
        else if(tmp_plane->getKind() == 1) { //aereo in arrivo da takeoff
            free = false;
            EV << "TOWER: Faccio decollare in seguito a pista libera\n";
            cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
            send(tmp_msg, "out_takeoff");
            waiting_queue.remove(waiting_queue.front());
            return;
        }
    }
    else {
        EV<<"TOWER: Nessun aereo in attesa per la pista\n";
        return;
    }

}

}; // namespace
