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

#include "Sky.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Sky);

simtime_t timer;
int count_s;

void Sky::initialize()
{
    count_s = 0;
    beep = new cMessage("beep");
    plane =nullptr;
    timer = par("procTime");
    scheduleAt(simTime() + timer, beep);
}

void Sky::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        EV << "Wait period is over, sending plane\n";

        msg_creation(count_s);

        send(plane, "out");
        cancelAndDelete(beep);
        beep = new cMessage("beep");
        scheduleAt(simTime()+timer, beep);
        count_s++;
    }
}

void Sky::msg_creation(int n) {

    plane = new Plane(nullptr);
    plane->setId(n);
    plane->setEnter(simTime());
    plane->setKind(0);
}

}; // namespace
