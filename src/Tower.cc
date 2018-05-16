/*
 * ------------ TORRE di CONTROLLO ---------------------------------------------------------
 *
 * Codice atto alla gestione delle due code di attarraggio e partenza.
 * Può ricevere messaggi da 3 fonti:
 *
 *      -> Pista: Risposta ad una richiesta di conoscenza pista libera/occupata;
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
unsigned int takeoffPlaneTimer;
unsigned int landingPlaneTimer;

//nuovo approccio
cQueue landing_queueT;
cQueue takeoff_queueT;

void Tower::initialize()
{
    temp_msg_land = nullptr;
    temp_msg_takeoff = nullptr;
    free = true;
    landingPlaneTimer = 0;
    takeoffPlaneTimer = 0;
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
   }

   else if(p)
   {
       if(!p->getKind()) {
           landing_queueT.insert(p);
       }
       else if (p->getKind()) {
           takeoff_queueT.insert(p);
       }
       EV<< "TOWER: Aereo aggiunto nella relativa coda\n";

       if(free) send_OK();
   }
}

void Tower::send_OK() {
    cObject* obj_plane = nullptr;
    Plane* tmp_plane;

    //Inizializzo i timer degli aerei (di entrambe le code)
    //in attesa da più tempo .
    if(!landing_queueT.isEmpty())
    {
            obj_plane = landing_queueT.front();
            tmp_plane = dynamic_cast<Plane*>(obj_plane);
            landingPlaneTimer = tmp_plane->getEnter().dbl() * 1000;
    }
    else landingPlaneTimer = 0;

    if(!takeoff_queueT.isEmpty() > 0)
    {
            obj_plane = takeoff_queueT.front();
            tmp_plane = dynamic_cast<Plane*>(obj_plane);
            takeoffPlaneTimer = tmp_plane->getEnter().dbl() * 1000;
    }
    else takeoffPlaneTimer = 0;

    EV << "TOWER: landingPlaneTimer: " << landingPlaneTimer << " takeoffPlaneTimer: " << takeoffPlaneTimer << "\n";

    //Eseguo confronto tra i timer per decidere a chi dare l'OK
    if((takeoff_queueT.isEmpty() && !landing_queueT.isEmpty()) || ((takeoffPlaneTimer > landingPlaneTimer) && landingPlaneTimer != 0))
    {
        free = false;
        EV << "TOWER: Faccio atterrare in seguito a pista libera\n";
        cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
        send(tmp_msg, "out_land");
        landing_queueT.remove(landing_queueT.front());
        landingPlaneTimer = 0;
        return;
    }

    if((landing_queueT.isEmpty() && !takeoff_queueT.isEmpty()) || ((takeoffPlaneTimer < landingPlaneTimer) && takeoffPlaneTimer != 0))
    {
        free = false;
        EV << "TOWER: Faccio decollare in seguito a pista libera\n";
        cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
        send(tmp_msg, "out_takeoff");
        takeoff_queueT.remove(takeoff_queueT.front());
        takeoffPlaneTimer = 0;
        return;
    }

    if(landing_queueT.isEmpty() && takeoff_queueT.isEmpty()) {

        EV<<"TOWER: Nessun aereo in attesa per la pista\n";
        return;
    }
}

}; // namespace
