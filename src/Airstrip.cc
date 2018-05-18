/*
 * ------------ Airstrip ----------------------------------------------------------------------
 *
 * Codice atto alla gestione delle due code di attarraggio e partenza.
 * Può ricevere messaggi da 3 fonti:
 *      -> isSelfMessage: Gestisce decollo/atterraggio effettuato con successo,
 *          in tal caso avvisa tower del suo liberamento;
 *      -> Tower: quando tower richiede lo stato della pista;
 *      -> Takeoff: un aereo in partenza giunge da parking;
 *      -> Landing: un aereo in arrivo giunge da landing;
 *
 * ------------------------------------------------------------------------------------------
 */

#include "Airstrip.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Airstrip);

bool free_strip; //true se pista è libera
simtime_t timerp;
Plane* plane;    //Plane* in cui memorizzare le info aereo proveniente da Takeoff

void Airstrip::initialize()
{
    free_strip = true;
}

void Airstrip::handleMessage(cMessage *msg)
{
    Plane* myMsg;
    myMsg = dynamic_cast<Plane*>(msg);
    if(myMsg) plane = dynamic_cast<Plane*>(msg);

    //selfMessage segnalante partenza/decollo con successo
    if(msg->isSelfMessage())
    {
        if(strcmp(msg->getName(), "beep_land") == 0)
        {
            EV << "TRACK: LANDED - " << "ID: " <<plane->getId() << "  ENTRY(s): " << plane->getEnter() <<"\n";
            send(plane, "out_parking");
        }

        else if(strcmp(msg->getName(), "beep_takeoff") == 0)
        {
            EV << "TRACK: DEPARTED - " << "ID: " <<plane->getId() << "  ENTRY(s): " << plane->getEnter() <<"\n";
            myMsg = nullptr;
        }
        free_strip = true;

        //avviso la torre che la pista è libera
        EV << "TRACK: Pista liberata dopo atterraggio/partenza, avviso torre\n";
        cMessage *tmp_msg = new cMessage("freeTrack");
        send(tmp_msg, "out_tower");
    }


    //Gestione messaggio con info aereo da Takeoff
    else if(plane->getKind())
    {
        free_strip = false;
        EV << "TRACK: <---Taking-off\n";

        //Start timer per decollo
        beep = new cMessage("beep_takeoff");
        timerp = par("procTime3");
        scheduleAt(simTime() + timerp, beep);

    }

    //Gestione messaggio con info aereo da Landing
    else if(!plane->getKind())
    {
        free_strip = false;
        EV << "TRACK: --->Landing\n";

        //Start timer per atterraggio
        beep = new cMessage("beep_land");
        timerp = par("procTime2");
        scheduleAt(simTime() + timerp, beep);
    }
}

}; // namespace
