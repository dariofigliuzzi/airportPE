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
#include "Plane_m.h"


namespace airport {

Define_Module(Tower);

cMessage *temp_msg_land;    //per invio OK a Landing
cMessage *temp_msg_takeoff; //per invio OK a Takeoff
cMessage *req;              //per invio richiesta disponibilità pista a Pista
bool free;
unsigned int planeCheck;       //tiene conto di quanti fra landing e takeoff hanno mandato le info sull'aereo
unsigned int takeoffPlaneTimer;
unsigned int landingPlaneTimer;

void Tower::initialize()
{
    temp_msg_land = nullptr;
    temp_msg_takeoff = nullptr;
    req = nullptr;
    free = true;
}

void Tower::handleMessage(cMessage *msg)
{
   Plane* p = dynamic_cast<Plane*>(msg);
   //Gestione messaggio "Free" da Pista
   if(strcmp(msg->getName(), "Free") == 0)
   {
       free = true;
   }

   //non appena la pista si libera chiedo a landing
   //e takeoff di dirmi quali sono i tempi di attesa
   if(strcmp(msg->getName(), "freeTrack") == 0)
   {
       EV << "Tower: avviso Landing e Takeoff di pista libera!\n";
       free = true;
       cMessage *tmp_msg = new cMessage("freeTrack");
       send(tmp_msg, "out_land");
       send(tmp_msg, "out_takeoff");
   }

   if(p || (strcmp(msg->getName(), "noPlanesLanding") == 0) || (strcmp(msg->getName(), "noPlanesDeparting") == 0))
   {
       if(!p->getKind() || (strcmp(msg->getName(), "noPlanesLanding") == 0)) //caso aereo proveniente da landing/in landing non ho aerei
       {
           planeCheck++;
           if(p) landingPlaneTimer = p->getEnter().dbl() * 1000;
           else landingPlaneTimer = 0;

           if(planeCheck == 2)
           {
               if((!takeoffPlaneTimer && landingPlaneTimer) || (takeoffPlaneTimer > landingPlaneTimer))
               {
                  EV << "Torre: Faccio atterrare in seguito a pista libera\n";
                  cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                  send(tmp_msg, "out_land");
                  free = false;
               }

               if((takeoffPlaneTimer && !landingPlaneTimer) || (takeoffPlaneTimer < landingPlaneTimer))
               {
                   EV << "Torre: Faccio decollare in seguito a pista libera\n";
                   cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                   send(tmp_msg, "out_takeoff");
                   free = false;
               }
               planeCheck = 0;
           }
       }

       else if(p->getKind() || (strcmp(msg->getName(), "noPlanesDeparting") == 0)) //caso aereo proveniente da takeoff/in takeoff non ho aerei
       {
           planeCheck++;
           if(p) takeoffPlaneTimer = p->getEnter().dbl() * 1000;
           else takeoffPlaneTimer = 0;

           if(planeCheck == 2)
           {
               if((!takeoffPlaneTimer && landingPlaneTimer) || (takeoffPlaneTimer > landingPlaneTimer))
               {
                  cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                  send(tmp_msg, "out_land");
                     free = false;
                  }

               if((takeoffPlaneTimer && !landingPlaneTimer) || (takeoffPlaneTimer < landingPlaneTimer))
               {
                   cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                   send(tmp_msg, "out_takeoff");
                   free = false;
               }

               planeCheck = 0;
          }
       }
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
