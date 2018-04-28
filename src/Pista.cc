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
// 

#include "Pista.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Pista);

bool free_strip;
simtime_t timerp;

void Pista::initialize()
{
    beep = new cMessage("beep");
    timerp = par("procTime");
    scheduleAt(simTime() + timerp, beep);
    free_strip = true;

}

void Pista::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        cMessage *tmp_msg = new cMessage("Free");

        if(free_strip) {
             send(tmp_msg, "out_tower");
             //free_strip = false; // disabilitato per testing

             cancelAndDelete(beep);
             beep = new cMessage("beep");
             scheduleAt(simTime()+timerp, beep);
        }
    }
    else {
        EV << "--->Landing\n";
        Plane *myMsg;
        myMsg = check_and_cast<Plane*>(msg);
        EV << "LANDED: " << " - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
        send(myMsg, "out_parking");
    }
}

}; // namespace
