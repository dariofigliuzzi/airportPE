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
cQueue planes_queue;

void Airstrip::initialize()
{
    free_strip = true;
    planes_queue.clear();
}

void Airstrip::handleMessage(cMessage *msg)
{
    Plane* myMsg;
    myMsg = dynamic_cast<Plane*>(msg);
    cObject* obj_plane = nullptr;

    if(myMsg){
        plane = dynamic_cast<Plane*>(msg);
        planes_queue.insert(myMsg);
        EV << "TRACK: PRINTING PLANES QUEUE:\n";

        for(cQueue::Iterator iter(planes_queue,0); !iter.end(); iter++)
        {
            Plane* myMsg =(Plane*) iter.operator *();
            EV <<"- ID:" <<myMsg->getId() << "  ENTRY(s):" << myMsg->getEnter() << "  KIND:" << myMsg->getKind() << "\n"; //0:Landing 1:Takeoff
        }
        manage_planes();
    }

    //selfMessage segnalante partenza/decollo con successo
    if(msg->isSelfMessage())
    {
        obj_plane = planes_queue.front();
        plane = dynamic_cast<Plane*>(obj_plane);

        if(strcmp(msg->getName(), "beep_land") == 0)
        {
            EV << "TRACK: LANDED - " << "ID: " <<plane->getId() << "  ENTRY(s): " << plane->getEnter() <<"\n";
            send(get_info(plane), "out_parking");
            planes_queue.remove(planes_queue.front());
        }

        else if(strcmp(msg->getName(), "beep_takeoff") == 0)
        {
            EV << "TRACK: DEPARTED - " << "ID: " <<plane->getId() << "  ENTRY(s): " << plane->getEnter() <<"\n";
            myMsg = nullptr;
            planes_queue.remove(planes_queue.front());
        }
        free_strip = true;

        if(!planes_queue.isEmpty()) manage_planes();
        EV << "TRACK: Pista liberata\n";
    }

}


//Prende le info dal myMsg della handleMessage. Non posso passare alla send
//direttamente myMsg perchè attualmente risiede nella landing_queue
Plane* Airstrip::get_info(Plane* p) {
    plane = new Plane(nullptr);
    plane->setEnter(p->getEnter());
    plane->setId(p->getId());
    plane->setKind(p->getKind());
    return plane;
}

void Airstrip::manage_planes() {
    cObject* obj = planes_queue.front();
    Plane* tmp = dynamic_cast<Plane*>(obj);

    if(tmp->getKind() && free_strip) {
        free_strip = false;
        EV << "TRACK: <---Taking-off\n";

        //Start timer per decollo
        beep = new cMessage("beep_takeoff");
        timerp = par("procTimeTakeoff");
        scheduleAt(simTime() + timerp, beep);
        cMessage *tmp_msg = new cMessage("OK");
        send(tmp_msg, "out_takeoff");
    }
    else if(!tmp->getKind() && free_strip) {
        free_strip = false;
        EV << "TRACK: --->Landing\n";

        //Start timer per atterraggio
        beep = new cMessage("beep_land");
        timerp = par("procTimeLanding");
        scheduleAt(simTime() + timerp, beep);
        cMessage *tmp_msg = new cMessage("OK");
        send(tmp_msg, "out_landing");
    }
}


}; // namespace
