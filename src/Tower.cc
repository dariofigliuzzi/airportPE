/*
 * ------------ TORRE di CONTROLLO ---------------------------------------------------------
 *
 * Codice atto alla gestione delle due code di attarraggio e partenza.
 * Pu� ricevere messaggi da 3 fonti:
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
cMessage *req;              //per invio richiesta disponibilit� pista a Pista
bool free;
unsigned int planeCheck;       //tiene conto di quanti fra landing e takeoff hanno mandato le info sull'aereo
unsigned int takeoffPlaneTimer;
unsigned int landingPlaneTimer;
unsigned int numPlanes;

void Tower::initialize()
{
    temp_msg_land = nullptr;
    temp_msg_takeoff = nullptr;
    req = nullptr;
    free = true;
}

void Tower::handleMessage(cMessage *msg)
{
   Plane* p;
   p = dynamic_cast<Plane*>(msg);

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
       cMessage *tmp_msg1 = new cMessage("freeTrack");
       send(tmp_msg1, "out_takeoff");
   }

   if((strcmp(msg->getName(), "noPlanesLanding") == 0) || (strcmp(msg->getName(), "noPlanesDeparting") == 0))
   {
       if(strcmp(msg->getName(), "noPlanesLanding") == 0)
       {
           planeCheck++;
           landingPlaneTimer = 0;
           free = false;

           if(planeCheck == 2)
           {
               if(((takeoffPlaneTimer!=0) && (landingPlaneTimer==0)) || (takeoffPlaneTimer < landingPlaneTimer))
               {
                   EV << "Torre: Faccio decollare in seguito a pista libera\n";
                   numPlanes--;
                   cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                   send(tmp_msg, "out_takeoff");
                   EV << "landingPlaneTimer: " << landingPlaneTimer << " takeoffPlaneTimer: " << takeoffPlaneTimer << "\n";
               }
               planeCheck = 0;
           }
       }
       else if(strcmp(msg->getName(), "noPlanesDeparting") == 0)
       {
           planeCheck++;
           takeoffPlaneTimer = 0;
           free = false;

           if(planeCheck == 2)
           {
               if(((takeoffPlaneTimer==0) && (landingPlaneTimer!=0)) || (takeoffPlaneTimer > landingPlaneTimer))
               {
                  EV << "1Torre: Faccio atterrare in seguito a pista libera\n";
                  numPlanes--;
                  cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                  send(tmp_msg, "out_land");
                  EV << "landingPlaneTimer: " << landingPlaneTimer << " takeoffPlaneTimer: " << takeoffPlaneTimer << "\n";
               }
               planeCheck = 0;
           }
       }
   }

   if(p)
   {
       if(!p->getKind()) //caso aereo proveniente da landing/in landing non ho aerei
       {
           planeCheck++;
           landingPlaneTimer = p->getEnter().dbl() * 1000;

           if(planeCheck == 2)
           {
               free = false;
               if(((takeoffPlaneTimer==0) && (landingPlaneTimer!=0)) || (takeoffPlaneTimer > landingPlaneTimer))
               {
                  EV << "2Torre: Faccio atterrare in seguito a pista libera\n";
                  numPlanes--;
                  cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                  send(tmp_msg, "out_land");
                  EV << "landingPlaneTimer: " << landingPlaneTimer << " takeoffPlaneTimer: " << takeoffPlaneTimer << "\n";
               }

               if(((takeoffPlaneTimer!=0) && (landingPlaneTimer==0)) || (takeoffPlaneTimer < landingPlaneTimer))
               {
                   EV << "Torre: Faccio decollare in seguito a pista libera\n";
                   numPlanes--;
                   cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                   send(tmp_msg, "out_takeoff");
                   EV << "landingPlaneTimer: " << landingPlaneTimer << " takeoffPlaneTimer: " << takeoffPlaneTimer << "\n";
               }
               planeCheck = 0;
           }
       }

       else if(p->getKind()) //caso aereo proveniente da takeoff/in takeoff non ho aerei
       {
           planeCheck++;
           takeoffPlaneTimer = p->getEnter().dbl() * 1000;
           EV << "Timer aereo: " << takeoffPlaneTimer << "\n";

           if(planeCheck == 2)
           {
               free = false;
               if(((takeoffPlaneTimer==0) && (landingPlaneTimer!=0)) || (takeoffPlaneTimer > landingPlaneTimer))
               {
                  EV << "3Torre: Faccio atterrare in seguito a pista libera\n";
                  numPlanes--;
                  cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                  send(tmp_msg, "out_land");
                  EV << "landingPlaneTimer: " << landingPlaneTimer << " takeoffPlaneTimer: " << takeoffPlaneTimer << "\n";
                  }

               if(((takeoffPlaneTimer!=0) && (landingPlaneTimer==0)) || (takeoffPlaneTimer < landingPlaneTimer))
               {
                   EV << "Torre: Faccio decollare in seguito a pista libera\n";
                   numPlanes--;
                   cMessage *tmp_msg = new cMessage("OK");//sfrutto questo messaggio
                   send(tmp_msg, "out_takeoff");
                   EV << "landingPlaneTimer: " << landingPlaneTimer << " takeoffPlaneTimer: " << takeoffPlaneTimer << "\n";
               }

               planeCheck = 0;
          }
       }
   }

   //Gestione richieste dalle code di landing e takeoff
   else if(strcmp(msg->getName(), "newplane_land") == 0 || strcmp(msg->getName(), "newplane_takeoff") == 0)
   {
       numPlanes++;
       EV << "Aerei totali nelle code:" << numPlanes << "\n";

       if(free && !numPlanes){
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
