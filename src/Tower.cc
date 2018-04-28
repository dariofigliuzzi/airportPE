/*
 * ------------ TORRE di CONTROLLO ---------------------------------------------------------
 *
 * Codice atto alla gestione delle due code di attarraggio e partenza.
 * Può ricevere messaggi da 3 fonti:
 *      -> Pista: Risposta ad una richiesta di conoscenza pista libera/occupata;
 *      -> landing_queue/takeoff_queue: Ricezione di una richiesta di atterraggio/partenza;
 *
 * Nel caso di richiesta atterraggio/partenza, provvede a rispondere l'esito al mittente.
 *
 * ------------------------------------------------------------------------------------------
 */

#include "Tower.h"

namespace airport {

Define_Module(Tower);

cMessage *temp_msg_land;    //per invio OK a Landing
cMessage *temp_msg_takeoff; //per invio OK a Takeoff
cMessage *req;              //per invio richiesta disponibilità pista a Pista
bool free;

void Tower::initialize()
{
    temp_msg_land = nullptr;
    temp_msg_takeoff = nullptr;
    req = nullptr;
    free = true;
}

void Tower::handleMessage(cMessage *msg)
{
   //Gestione messaggio "Free" da Pista
   if(strcmp(msg->getName(), "Free") == 0)
   {
       free = true;
   }

   //Gestione richieste dalle code di landing e takeoff
   else if(strcmp(msg->getName(), "newplane_land") == 0 || strcmp(msg->getName(), "newplane_takeoff") == 0)
   {
       req = new cMessage("REQ");
       send(req, "out_pista");

       if(free){
           EV << "Sending OK to queues\n";

           if(strcmp(msg->getName(), "newplane_land") == 0)
           {
               free = false;
               temp_msg_land = new cMessage("OK");
               send(temp_msg_land, "out_land");
           }

           else if(strcmp(msg->getName(), "newplane_takeoff") == 0)
           {
               free = false;
               temp_msg_takeoff = new cMessage("OK");
               send(temp_msg_takeoff, "out_takeoff");
           }
       }
   }
}

}; // namespace
