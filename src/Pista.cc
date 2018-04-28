//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// PORCODDIOdiocane

#include "Pista.h"
#include "Plane_m.h"
//#include "Takeoff.h"

namespace airport {

Define_Module(Pista);

bool free_strip;
simtime_t timerp;
Plane* myMsg;

void Pista::initialize()
{
    beep = nullptr;
    free_strip = true;

}

void Pista::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        if(strcmp(msg->getName(), "beep_land") == 0) {
            EV << "LANDED: " << "Plane - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
            send(myMsg, "out_parking");
        }
        else {
            EV << "TAKED-OFF: " << "Plane - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
            myMsg = nullptr;
        }
        free_strip = true;
    }
    //else if((strcmp(msg->getName(), "REQ_LAND") == 0) || (strcmp(msg->getName(), "REQ_TAKEOFF") == 0)) {
    else if(strcmp(msg->getName(), "REQ") == 0) {
        if(free_strip) {
            cMessage *tmp_msg = new cMessage("Free");
            send(tmp_msg, "out_tower");

        }
    }
    else if(strcmp(msg->getSenderModule()->getFullName(), "takeoff") == 0) {
        free_strip = false;
        EV << "<---Taking-off\n";
        myMsg = check_and_cast<Plane*>(msg);
        //start timer per decollo
        cancelAndDelete(beep);
        beep = new cMessage("beep_takeoff");
        timerp = par("procTime3");
        scheduleAt(simTime() + timerp, beep);

    }
    else {
        //EV << "Nome del modulo: " << msg->getSenderModule()->getFullName() << "\n";
        free_strip = false;
        EV << "--->Landing\n";
        myMsg = check_and_cast<Plane*>(msg);
        //start timer per atterraggio
        cancelAndDelete(beep);
        beep = new cMessage("beep_land");
        timerp = par("procTime2");
        scheduleAt(simTime() + timerp, beep);
    }
}

}; // namespace
